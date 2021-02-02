#pragma once

/*

Type: Namespace
Name: Hooks
Intended for: Hooked Source Engine functions

*/

#include "sdk/include/valve_sdk/csgostructs.hpp"
#include "sdk/include/helpers/vfunc_hook.hpp"
#include "globals.h"
#include <d3d9.h>

namespace index
{
	constexpr auto PaintTraverse = 41;
	constexpr auto CreateMove = 21;
	constexpr auto PlaySound = 82;
	constexpr auto FrameStageNotify = 36;
	constexpr auto DrawModelExecute = 21;
	constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto EndScene = 42;
	constexpr auto Reset = 16;
	constexpr auto DrawIndexedPrimitive = 82;
	constexpr auto SetCrosshairAngle = 29;
	constexpr auto OverrideView = 18;
	constexpr auto GetViewModelFOV = 35;
	constexpr auto ShouldDrawViewModel = 27;
}

namespace Hooks
{
	void Initialize();
	void Shutdown();

	extern vfunc_hook hlclient_hook;
	extern vfunc_hook vguipanel_hook;
	extern vfunc_hook vguisurf_hook;
	extern vfunc_hook mdlrender_hook;
	extern vfunc_hook viewrender_hook;
	extern vfunc_hook renderveiw_hook;
	extern vfunc_hook d3d_hook;

	
	using CreateMove = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
	using PaintTraverse = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
	using FrameStageNotify = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
	using PlaySound = void(__thiscall*)(ISurface*, const char* name);
	using DrawModelExecute = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	using FireEvent = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
	using DoPostScreenEffects = int(__thiscall*)(IClientMode*, int);
	using SetCrosshairAngle = void(__thiscall*)(const QAngle&);
	using OverrideView_t = void(__thiscall*)(void*,CViewSetup* view);
	using GetViewModelFOV_t = float(__thiscall*)(void*);
	using ShouldDrawViewModel_t = bool(__thiscall*)(void*);
	
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active);
	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __stdcall hkPlaySound(const char* name);
	void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage);
	int  __stdcall hkDoPostScreenEffects(int a1);
	void __fastcall hkOverrideView(void* ecx, void* edx, CViewSetup* view);
	float __fastcall hkGetViewModelFOV(void* ecx, void* edx);
	bool __fastcall hkShouldDrawViewModel(void* ecx, void* edx);
}
