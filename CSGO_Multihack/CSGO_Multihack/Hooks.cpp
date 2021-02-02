#include "Hooks.h"
#include "Aimbot.h"
#include "chams.h"
#include "glow.h"
#include "esp.h"
#include "globals.h"
#include "sdk/include/helpers/input.hpp"
#include "sdk/include/helpers/utils.hpp"
#include "GrenadePrediction.h"

void TracingTriggerBot(CUserCmd* cmd);
void ClassicTriggerBot(CUserCmd* cmd);

#define GetRekt(P) (P)


HRESULT WINAPI hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDev);
HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
extern pReset oReset;
extern pDrawIndexedPrimitive oDrawIndexedPrimitive;
extern pEndScene oEndScene;
static D3DDEVICE_CREATION_PARAMETERS s_cparams;

namespace Hooks
{
	vfunc_hook hlclient_hook;
	vfunc_hook vguipanel_hook;
	vfunc_hook vguisurf_hook;
	vfunc_hook mdlrender_hook;
	vfunc_hook clientmode_hook;
	vfunc_hook renderveiw_hook;
	vfunc_hook d3d_hook;

	void Initialize()
	{
		auto dx9api = GetModuleHandleW(L"shaderapidx9.dll");
		g_npDevice = **(IDirect3DDevice9***)(Utils::PatternScan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1); 

		hlclient_hook.setup(g_CHLClient);
		vguipanel_hook.setup(g_VGuiPanel); 
		vguisurf_hook.setup(g_VGuiSurface);
		mdlrender_hook.setup(g_MdlRender);
		clientmode_hook.setup(g_ClientMode);
		renderveiw_hook.setup(g_RenderView);
		d3d_hook.setup(g_npDevice);
		
		d3d_hook.hook_index(index::EndScene, hkEndScene);
		d3d_hook.hook_index(index::Reset, hkReset); 
		d3d_hook.hook_index(index::DrawIndexedPrimitive, hkDrawIndexedPrimitive);
		oDrawIndexedPrimitive = d3d_hook.get_original<pDrawIndexedPrimitive>(index::DrawIndexedPrimitive);
		oEndScene = d3d_hook.get_original<pEndScene>(index::EndScene);
		oReset = d3d_hook.get_original<pReset>(index::Reset);
		hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
		hlclient_hook.hook_index(index::CreateMove, hkCreateMove_Proxy);
		vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);
		vguisurf_hook.hook_index(index::PlaySound, hkPlaySound);
		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);
		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		ESP::CreateFonts(g_Options.esp_font_size);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);
		clientmode_hook.hook_index(index::GetViewModelFOV, hkGetViewModelFOV);
		clientmode_hook.hook_index(index::ShouldDrawViewModel, hkShouldDrawViewModel);
		g_npDevice->GetCreationParameters(&s_cparams);
	}


	void Shutdown()
	{
		hlclient_hook.unhook_all();
		vguipanel_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		clientmode_hook.unhook_all();

		ESP::DestroyFonts();
	}

	inline void ClampMovement(CUserCmd* cmd)
	{
		if (cmd->forwardmove >= 450) cmd->forwardmove = 450;
		if (cmd->sidemove >= 450) cmd->sidemove = 450;
		if (cmd->upmove >= 450) cmd->upmove = 450;
		if (cmd->forwardmove <= -450) cmd->forwardmove = -450;
		if (cmd->sidemove <= -450) cmd->sidemove = -450;
		if (cmd->upmove <= -450) cmd->upmove = -450;
	}
	
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
	{
		auto oCreateMove = hlclient_hook.get_original<CreateMove>(index::CreateMove);

		if (g_DisableAll)
		{
			oCreateMove(g_CHLClient, sequence_number, input_sample_frametime, active);
			return;
		}

		if (g_FakeLagUseKey)
		{
			if (GetAsyncKeyState(g_TriggerHotkey) & 0x8000)
				bSendPacket = true;
			else
				bSendPacket = !g_FakeLagNow;
		}
		else
			bSendPacket = !g_FakeLagNow;

		oCreateMove(g_CHLClient, sequence_number, input_sample_frametime, active);

		auto cmd = g_Input->GetUserCmd(sequence_number); 
		auto verified = g_Input->GetVerifiedCmd(sequence_number);

		if (!cmd)
			return;

		if (!cmd->command_number)
			return;

		CAimbot::Get().Think(cmd);

		if (g_TriggerBotMethod == 0)
			TracingTriggerBot(cmd);
		else
			ClassicTriggerBot(cmd);

		GrenadePrediction::Get().Tick(cmd->buttons);

		if (!Math::SanitizeAngles(cmd->viewangles))
			return;

		ClampMovement(cmd);

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}


	__declspec(naked) void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active)
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::hkCreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}

	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>(index::PaintTraverse);
		
		oPaintTraverse(g_VGuiPanel, panel, forceRepaint, allowForce);

		if (g_DisableAll)
			return;

		if (!panelId)
		{
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel"))
			{
				panelId = panel;
			}
		}
		else if (panelId == panel)
		{
			if (g_EngineClient->IsInGame() && !g_EngineClient->IsTakingScreenshot())
			{
				if (g_Options.ab_display_info)
					ESP::Aimbot::DisplayInfo();

				if (!g_LocalPlayer)
					return;

				if (g_Options.sesp_enabled)
				{
					if (g_Options.sesp_whaim_esp)
						ESP::Sound::IsAimingEnemy();

					if (g_Options.sesp_distance_esp)
						ESP::Sound::EnemyIsNear();

					if (g_Options.sesp_notify_spotted)
						ESP::Player::NotifyWhenSpotted();

				}

				if (g_Options.esp_enabled)
				{
					if (g_Options.esp_GrenadePrediction)
						GrenadePrediction::Get().Render();

					if (g_Options.esp_notify_spotted)
						ESP::Player::NotifyWhenSpotted();

					//if(g_LocalPlayer->IsAlive())
					//	ESP::Player::RenderWall();

					for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
					{
						auto entity = C_BasePlayer::GetPlayerByIndex(i);

						if (!entity)
							continue;

						if (entity->IsDormant())
							continue;

						if (entity == g_LocalPlayer)
							continue;

						if (i < 65 && !entity->IsDormant() && entity->IsAlive())
						{
							if (ESP::Player::Begin(entity))
							{
								if (g_Options.esp_player_snaplines)			ESP::Player::RenderSnapline();
								if (g_Options.esp_player_boxes)				ESP::Player::RenderBox(g_Options.esp_player_boxes_head_only);
								if (g_Options.esp_player_weapons)			ESP::Player::RenderWeapon();
								if (g_Options.esp_player_names)				ESP::Player::RenderName();
								if (g_Options.esp_player_distance)			ESP::Player::RenderDistance();
								if (g_Options.esp_player_health)			ESP::Player::RenderHealth(g_Options.esp_player_boxes_head_only);
								if (g_Options.esp_player_armour)			ESP::Player::RenderArmour(g_Options.esp_player_boxes_head_only);
								if (g_Options.esp_player_bones)				ESP::Player::RenderBones();
								if (g_Options.esp_player_defuse_status)		ESP::Player::RenderDefusing();
							}
						}
						else if (g_Options.esp_dropped_weapons && entity->IsWeapon())
						{
							ESP::Misc::RenderWeapon((C_BaseCombatWeapon*)entity);
						}
						else if (g_Options.esp_defuse_kit && entity->IsDefuseKit())
						{
							ESP::Misc::RenderDefuseKit(entity);
						}
						else if (entity->IsPlantedC4())
						{
							if (g_Options.esp_planted_c4)
								ESP::Misc::RenderPlantedC4(entity);
						}
					}
				}

				if (g_Options.esp_crosshair)
					ESP::Misc::RenderCrosshair();
			}
		}
	}


	void __stdcall hkPlaySound(const char* name)
	{
		static auto oPlaySound = vguisurf_hook.get_original<PlaySound>(index::PlaySound);
		oPlaySound(g_VGuiSurface, name);
	}


	int __stdcall hkDoPostScreenEffects(int a1)
	{
		auto oDoPostScreenEffects = clientmode_hook.get_original<DoPostScreenEffects>(index::DoPostScreenSpaceEffects);

		if (g_DisableAll)
			return oDoPostScreenEffects(g_ClientMode, a1);

		if (g_LocalPlayer && g_Options.glow_enabled)
			Glow::Get().Run();
		
		return oDoPostScreenEffects(g_ClientMode, a1);
	}


	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
	{
		static auto oFrameStageNotify = hlclient_hook.get_original<FrameStageNotify>(index::FrameStageNotify);
		oFrameStageNotify(g_CHLClient, stage);
	}


	void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto oDrawModelExecute = mdlrender_hook.get_original<DrawModelExecute>(index::DrawModelExecute);

		if (g_DisableAll)
		{
			oDrawModelExecute(g_MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
			return;
		}

		Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
		oDrawModelExecute(g_MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
		g_MdlRender->ForcedMaterialOverride(nullptr);
	}

	inline float fsetsign(float f, bool negative)
	{
		if (negative)
		{
			if (f < 0)
				return f;
			else if (f > 0)
				return (f * -1.0f);
		}
		else
		{
			if (f < 0)
				return (f * -1.0f);
			else if (f > 0)
				return f;
		}
		return f;
	}

	static bool s_zoomHideVM = false;

	void __fastcall hkOverrideView(void* ecx, void* edx, CViewSetup* view)
	{
		GetRekt(edx);
		static auto oOverrideView = clientmode_hook.get_original<OverrideView_t>(index::OverrideView);

		if (g_DisableAll)
		{
			oOverrideView(ecx, view);
			return;
		}

		static bool zoom_now = false;
		static float last_fov = 0.0f;
		static float target_fov = 0.0f;
		float current_fov = view->fov;
		static float step = 0.0f;
		static bool reached = true;
		bool scoping = false;
		static bool scoped = false;
		
		float time = g_GlobalVars->curtime;
		static float last_time = 0.0f;
		const float period = 0.0010f;

		if (time > last_time)
			last_time = 0;

		if (g_EngineClient->IsInGame())
			if (g_LocalPlayer)
				scoping = g_LocalPlayer->m_bIsScoped();

		WaitForSingleObject(g_Options.esp_zoom_key_mutex, INFINITE);

		if ((GetAsyncKeyState(g_Options.esp_zoom_key) & 1) && !scoping && !scoped)
		{
			zoom_now = !zoom_now;
			scoped = false;

			if (zoom_now)
			{
				target_fov = g_Options.esp_zoom_fov;
				if(reached)
					last_fov = current_fov;
			}
			else
			{
				target_fov = view->fov;
				if(reached)
					last_fov = g_Options.esp_zoom_fov;
			}

			step = fabs(last_fov - target_fov) / 25.0f;
			reached = g_Options.esp_zoom_instant;
			if (last_fov > target_fov)
				step = fsetsign(step, true);
			if (last_fov < target_fov)
				step = fsetsign(step, false);
			if (fabs(last_fov - target_fov) <= 1.5f)
				reached = true;
		}
		else if (scoping)
		{
			reached = true;
			scoped = true;
			if (g_Options.esp_zoom_dont_when_scope)
				zoom_now = false;
			else
			{
				reached = true;
				zoom_now = true;
			}
		}
		else if (scoped && !scoping)
		{
			scoped = false;
			zoom_now = false;
			reached = true;
		}

		s_zoomHideVM = zoom_now;

		ReleaseMutex(g_Options.esp_zoom_key_mutex);
		
		bool reset = false;

		if (!g_EngineClient->IsInGame())
		{
			zoom_now = false;
			reset = true;
		}
		else if (!g_LocalPlayer)
		{
			zoom_now = false;
			reset = true;
		}
		else if (!g_LocalPlayer->IsAlive())
		{
			zoom_now = false;
			reset = true;
		}
		else
		{
			auto weap = g_LocalPlayer->m_hActiveWeapon();

			if (!weap)
			{
				zoom_now = false;
				reset = true;
			}
			else if(weap->IsDormant())
			{
				zoom_now = false;
				reset = true;
			}
			else if (!weap->IsWeapon())
			{
				zoom_now = false;
				reset = true;
			}
			else if (weap->IsKnife() || weap->IsGrenade())
			{
				zoom_now = false;
				reset = true;
			}
		}

		view->viewmodel_fov = g_Options.esp_zoom_hands_fov;

		if (reset)
		{
			scoped = false;
			GrenadePrediction::Get().View(view);
			oOverrideView(ecx, view);
			return;
		}

		if (!reached && g_Options.esp_zoom_enabled)
		{
			if (time - last_time > period)
			{
				last_fov += step;

				if (last_fov > target_fov)
					step = fsetsign(step, true);
				if (last_fov < target_fov)
					step = fsetsign(step, false);
				if (fabs(last_fov - target_fov) <= 1.5f)
					reached = true;

				last_time = time;
			}

			view->fov = last_fov;
		}
		else
		{
			if (g_Options.esp_zoom_enabled && zoom_now)
				view->fov = g_Options.esp_zoom_fov;
			else
				zoom_now = false;
		}

		GrenadePrediction::Get().View(view);
		oOverrideView(ecx, view);
	}	

	float __fastcall hkGetViewModelFOV(void* ecx, void* edx)
	{
		static auto oGetViewModelFOV = clientmode_hook.get_original<GetViewModelFOV_t>(index::GetViewModelFOV);
		GetRekt(edx);

		if (g_DisableAll)
			return oGetViewModelFOV(ecx);

		oGetViewModelFOV(ecx);
		return g_Options.esp_zoom_hands_fov;
	}
	
	bool __fastcall hkShouldDrawViewModel(void* ecx, void* edx)
	{
		static auto oShouldDrawViewModel = clientmode_hook.get_original<ShouldDrawViewModel_t>(index::ShouldDrawViewModel);

		if(g_DisableAll)
			return oShouldDrawViewModel(ecx);

		GetRekt(edx);
		if (g_Options.esp_hide_viewmodel || (s_zoomHideVM && g_Options.esp_hide_viewmodel_when_zoom))
			return false;
		else
			return oShouldDrawViewModel(ecx);
	}
}
