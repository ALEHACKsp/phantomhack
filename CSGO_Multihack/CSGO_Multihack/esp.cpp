#include "esp.h"
#include "globals.h"
#include "sdk\include\csgosdk.h"
#include "Hooks.h"
#include "Autowall.h"
#include <map>


vgui::HFont esp_font;
vgui::HFont notify_font;
vgui::HFont defuse_font;
vgui::HFont dropped_weapons_font;

// ESP Context
// This is used so that we dont have to calculate player color and position
// on each individual function over and over


struct
{
	C_BasePlayer* pl;
	bool          is_enemy;
	bool          is_visible;
	Color         clr;
	Vector        head_pos;
	Vector        feet_pos;
	RECT          bbox;
} esp_ctx;

RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = 
	{
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) 
	{
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) 
	{
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++)
	{
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}
//--------------------------------------------------------------------------------
bool ESP::CreateFonts(int fontSize)
{
	esp_font = g_VGuiSurface->CreateFont_();
	defuse_font = g_VGuiSurface->CreateFont_();
	dropped_weapons_font = g_VGuiSurface->CreateFont_();
	notify_font = g_VGuiSurface->CreateFont_();

	g_VGuiSurface->SetFontGlyphSet(esp_font, "Arial", fontSize + 1, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyphSet(defuse_font, "Arial", fontSize + 5, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyphSet(dropped_weapons_font, "Arial", fontSize, 700, 0, 0, FONTFLAG_DROPSHADOW);
	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);
	g_VGuiSurface->SetFontGlyphSet(notify_font, "Arial", (screen_h / 15) + fontSize, 700, 0, 0, FONTFLAG_ANTIALIAS);

	return true;
}
//--------------------------------------------------------------------------------
void ESP::DestroyFonts()
{
	// Is there a way to destroy vgui fonts?
	// TODO: Find out
	
}
//--------------------------------------------------------------------------------
bool ESP::Player::Begin(C_BasePlayer* pl)
{
	esp_ctx.pl = pl;
	esp_ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	if (!pl->IsDormant())
	{
		esp_ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);
		
		if (!esp_ctx.is_enemy && g_Options.esp_enemies_only)
			return false;

		esp_ctx.clr = esp_ctx.is_enemy ? (esp_ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded) : (esp_ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded);


		auto head = pl->GetHitboxPos(HITBOX_HEAD);
		auto origin = pl->m_vecOrigin();

		head.z += 15;

		if (!Math::WorldToScreen(head, esp_ctx.head_pos) ||
			!Math::WorldToScreen(origin, esp_ctx.feet_pos))
			return false;

		auto h = fabs(esp_ctx.head_pos.y - esp_ctx.feet_pos.y);
		auto w = h / 1.65f;

		esp_ctx.bbox.left = static_cast<long>(esp_ctx.feet_pos.x - w * 0.5f);
		esp_ctx.bbox.right = static_cast<long>(esp_ctx.bbox.left + w);
		esp_ctx.bbox.bottom = static_cast<long>(esp_ctx.feet_pos.y);
		esp_ctx.bbox.top = static_cast<long>(esp_ctx.head_pos.y);
		
	}
	else
	{
		esp_ctx.is_visible = false;
		esp_ctx.clr = esp_ctx.is_enemy ? (esp_ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded) : (esp_ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded);
	}
	return true;
}
//--------------------------------------------------------------------------------
void ESP::Player::RenderBox(bool headOnly)
{
	if (headOnly)
	{
		int x1, y1, x2, y2;
		Vector hpos = esp_ctx.pl->GetHitboxPos(HITBOX_HEAD);
		Vector hw2spos;
		Math::WorldToScreen(hpos, hw2spos);
		float size = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
		float dist = size / 8;
		const float scale = 0.70f;

		x1 = int(hw2spos.x - dist * scale);
		y1 = int(hw2spos.y - dist * scale);
		x2 = int(hw2spos.x + dist * scale);
		y2 = int(hw2spos.y + dist * scale);

		g_VGuiSurface->DrawSetColor(esp_ctx.clr);
		g_VGuiSurface->DrawOutlinedRect(x1, y1, x2, y2);
		g_VGuiSurface->DrawSetColor(Color::Black);
		g_VGuiSurface->DrawOutlinedRect(x1 - 1, y1 - 1, x2 + 1, y2 + 1);
		g_VGuiSurface->DrawOutlinedRect(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
	}
	else
	{
		g_VGuiSurface->DrawSetColor(esp_ctx.clr);
		g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left, esp_ctx.bbox.top, esp_ctx.bbox.right, esp_ctx.bbox.bottom);
		g_VGuiSurface->DrawSetColor(Color::Black);
		g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left - 1, esp_ctx.bbox.top - 1, esp_ctx.bbox.right + 1, esp_ctx.bbox.bottom + 1);
		g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left + 1, esp_ctx.bbox.top + 1, esp_ctx.bbox.right - 1, esp_ctx.bbox.bottom - 1);
	}
}

//--------------------------------------------------------------------------------

void ESP::Player::RenderBones()
{
	studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(esp_ctx.pl->GetModel());

	if (!studioHdr)
		return;

	static matrix3x4_t pBoneToWorldOut[128];

	if (esp_ctx.pl->SetupBones(pBoneToWorldOut, 128, 256, 0))
	{
		for (int i = 0; i < studioHdr->numbones; i++)
		{
			mstudiobone_t* pBone = studioHdr->GetBone(i);

			if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
				continue;

			Vector vBonePos1;
			Vector vBonePos2;

			if (g_DebugOverlay->ScreenPosition(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
				continue;

			if (g_DebugOverlay->ScreenPosition(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
				continue;

			if (g_Options.esp_bones_hp_colored)
			{
				auto  hp = esp_ctx.pl->m_iHealth();
				int green = int(hp * 2.55f);
				int red = 255 - green;
				g_VGuiSurface->DrawSetColor(Color(red, green, 0, 255));
			}
			else
				g_VGuiSurface->DrawSetColor(esp_ctx.clr);
			g_VGuiSurface->DrawLine(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y);
		}
	}
}

//--------------------------------------------------------------------------------

inline int GetFloatTail(float f, UINT count)
{
	if (count == 0)
		return 0;
	f = abs(f);
	int i = (int)floor(f);
	f = (f - i) * pow(10, count);
	return (int)floor(f);
}

void ESP::Player::RenderName()
{
	wchar_t buf[128];

	player_info_t info = esp_ctx.pl->GetPlayerInfo();

	if (MultiByteToWideChar(CP_UTF8, 0, info.szName, -1, buf, 128) > 0)
	{
		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
		g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.head_pos.y - th);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));

	}
}

void ESP::Player::RenderDefusing()
{
	bool IsDefuser = false;
	C_PlantedC4* bomb = nullptr;

	for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++)
	{
		auto ent = C_BasePlayer::GetPlayerByIndex(i);

		if (!ent)
			continue;

		if (ent == g_LocalPlayer)
			continue;

		if (ent->IsPlantedC4())
		{
			bomb = (C_PlantedC4*)ent;
			break;
		}
	}

	if (!bomb)
		IsDefuser = false;
	else
	{
		auto defuser = bomb->GetBombDefuser();
		if (!defuser)
			IsDefuser = false;
		else
		{
			if (esp_ctx.pl == defuser)
				IsDefuser = true;
			else
				IsDefuser = false;
		}
	}

	if (!IsDefuser)
		return;

	float time2def = bomb->GetDefuseCountdown();
	float max_defuse_time = 10.000f;
	float defuse_timer = (g_GlobalVars->curtime - time2def) * -1;
	if (defuse_timer < 0.000f)
		defuse_timer = 0.000f;
	
	if (esp_ctx.pl->m_bHasDefuser())
		max_defuse_time = 5.000f;

	if (g_Options.esp_player_defuse_status_type == 0)
	{
		int tw, th;
		xstring defuse_status;
		defuse_status.Format(L"Defusing: %[2]f sec remained", defuse_timer);

		g_VGuiSurface->GetTextSize(esp_font, defuse_status.GetStr().c_str(), tw, th);
		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(Color(255, 0, 0, 255));
		g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.head_pos.y - th*2);
		g_VGuiSurface->DrawPrintText(defuse_status.GetStr().c_str(), defuse_status.GetLen());
	}
	else
	{
		float box_w = (float)fabs(esp_ctx.bbox.left - esp_ctx.bbox.right);

		int w = box_w;
		int h = 10;
		auto width = (((w * defuse_timer) / max_defuse_time));

		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, L"0", tw, th);

		int x = esp_ctx.feet_pos.x - w/2;
		int y = esp_ctx.head_pos.y - th  - (h + 1);
		int x2 = x + w;
		int y2 = y + h;


		g_VGuiSurface->DrawSetColor(Color::Black);
		g_VGuiSurface->DrawFilledRect(x, y, x2, y2);

		g_VGuiSurface->DrawSetColor(g_Options.color_esp_defuse);
		g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x  + width - 2, y2 - 1);
	}
}

void ESP::Player::RenderDistance()
{
	float distance = esp_ctx.pl->m_vecOrigin().DistTo(g_LocalPlayer->m_vecOrigin());
	xstring sdist(false);
	sdist.Format(L"%[2]f", distance);

	int tw, th;
	g_VGuiSurface->GetTextSize(esp_font, sdist.GetStr().c_str(), tw, th);
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
	g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.feet_pos.y + th);
	g_VGuiSurface->DrawPrintText(sdist.GetStr().c_str(), sdist.GetLen());
}

//--------------------------------------------------------------------------------
void ESP::Player::RenderHealth(bool headOnly)
{
	auto  hp = esp_ctx.pl->m_iHealth();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 8;

	int h = box_h;
	if (headOnly)
		h = h - (floor(h / 3) * 2);
	auto height = (((h * hp) / 100));

	int green = int(hp * 2.55f);
	int red = 255 - green;

	int x = esp_ctx.bbox.left - off;
	int y = esp_ctx.bbox.top;
	int w = 5;

	int dx = floor((esp_ctx.bbox.right - esp_ctx.bbox.left) / 3);

	if (headOnly)
		x = x + dx;

	int x2 = x + w;
	int y2 = y + h;


	g_VGuiSurface->DrawSetColor(Color::Black);
	g_VGuiSurface->DrawFilledRect(x, y, x2, y2);

	g_VGuiSurface->DrawSetColor(Color(red, green, 0, 255));
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x2 - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
void ESP::Player::RenderArmour(bool headOnly)
{
	auto  armour = esp_ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 4;

	int h = box_h;
	if (headOnly)
		h = h - (floor(h / 3) * 2);

	auto height = (((h * armour) / 100));

	int x = esp_ctx.bbox.right + off;
	int y = esp_ctx.bbox.top;
	int w = 5;

	int dx = floor((esp_ctx.bbox.right - esp_ctx.bbox.left) / 3);

	if (headOnly)
		x = x - dx;

	int x2 = x + w;
	int y2 = y + h;


	g_VGuiSurface->DrawSetColor(Color::Black);
	g_VGuiSurface->DrawFilledRect(x, y, x2, y2);

	g_VGuiSurface->DrawSetColor(Color(120, 120, 255, 255));
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x2 - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
void ESP::Player::RenderWeapon()
{
	wchar_t buf[80];

	auto weapon = esp_ctx.pl->m_hActiveWeapon();

	if (!weapon)
		return; 

	if (weapon->IsDormant())
		return;

	if (!weapon->IsWeapon())
		return;


	if (MultiByteToWideChar(CP_UTF8, 0, weapon->GetCSWeaponData()->szWeaponName + 7, -1, buf, 80) > 0) {
		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
		g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.feet_pos.y);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}
//--------------------------------------------------------------------------------
void ESP::Player::RenderSnapline()
{
	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);

	g_VGuiSurface->DrawSetColor(esp_ctx.clr);

	g_VGuiSurface->DrawLine(
		screen_w / 2,
		screen_h,
		esp_ctx.feet_pos.x,
		esp_ctx.feet_pos.y);
}
//--------------------------------------------------------------------------------

void __fastcall DrawTexture(const char* name, int x, int y, Color &color , unsigned int side)
{
	int id = g_VGuiSurface->DrawGetTextureId(name);

	if (id == -1)
	{
		id = g_VGuiSurface->CreateNewTextureID();
		g_VGuiSurface->DrawSetTextureFile(id, name, 0, false);
	}

	int w, h;
	if (side == 0)
		g_VGuiSurface->DrawGetTextureSize(id, w, h);
	else
	{
		w = side;
		h = side;
	}

	g_VGuiSurface->DrawSetTexture(id);
	g_VGuiSurface->DrawSetColor(color);
	g_VGuiSurface->DrawTexturedRect(x, y, x + w, y + h);
}

void ESP::Player::NotifyWhenSpotted()
{
	bool spotted = false;
	static bool last = false;
	int alpha = g_Options.esp_notify_spotted_alpha;
	static int s_alpha = 0;
	static int s_alpha_step = 1;

	const float tick = 0.0010f;
	const float s_period = float(g_Options.sesp_notify_spotted_pause);
	static float vlast_time = 0.0f;
	static float slast_time = 0.0f; 
	float time = g_GlobalVars->curtime;

	if (vlast_time > time)
		vlast_time = 0.0f;

	if (slast_time > time)
		slast_time = 0.0f;
	
	if (!g_LocalPlayer->IsAlive())
		return;

	for (int j = 1; j < 65; j++)
	{
		C_BasePlayer* pl = (C_BasePlayer*)g_EntityList->GetClientEntity(j);

		if (!pl)
			continue;

		if (!pl->IsPlayer())
			continue;

		if (pl->IsDormant())
			continue;

		if(!pl->IsAlive())
			continue;

		if (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
			spotted = false;
		else
		{
			for (int i = 0; i < HITBOX_MAX; i++)
			{
				spotted = pl->CanSeePlayer(g_LocalPlayer, i);
				if (spotted)
					break;
			}

			if (spotted)
				break;
		}
	}

	xstring status(false);
	Color col;
	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);
	const char* lpTex = "vgui/hud/eye";
	const wchar_t* lpwStatus = L"You are visible to an enemy!";
	const int texResolution = 256;

	if (alpha > 0)
	{
		if (time - vlast_time > tick)
		{
			if (alpha + s_alpha >= 255 || s_alpha >= 35)
				s_alpha_step = -1;
			if(alpha + s_alpha <= 1 || s_alpha <= -35)
				s_alpha_step = 1;

			s_alpha += s_alpha_step;
			vlast_time = time;
		}
	}
	else
		s_alpha = 0;
	
	alpha += s_alpha;

	if (alpha < 0)
		alpha = 0;
	if(alpha > 255)
		alpha = 255;

	if (g_Options.esp_notify_spotted && spotted)
	{
		int tex_x, tex_y;
		tex_x = (screen_w / 2) - (texResolution / 2);
		tex_y = (screen_h / 2) + ((screen_h / 13) / 2) - (texResolution / 6);
		col = Color(g_Options.color_esp_notify.r(), g_Options.color_esp_notify.g(), 
		g_Options.color_esp_notify.b(), alpha);
		DrawTexture(lpTex, tex_x, tex_y, col, texResolution);

		//Uncomment for text notification under picture
		/*int tw, th;
		g_VGuiSurface->GetTextSize(notify_font, lpwStatus, tw, th);

		int txt_x, txt_y;
		txt_x = (screen_w / 2) - (tw / 2);
		txt_y = tex_y + (texResolution / 2) + th;
		g_VGuiSurface->DrawSetTextFont(notify_font);
		g_VGuiSurface->DrawSetTextColor(col);
		g_VGuiSurface->DrawSetTextPos(txt_x, txt_y);
		g_VGuiSurface->DrawPrintText(lpwStatus, wcslen(lpwStatus));*/
	}

	if ((last != spotted) && spotted  && g_Options.sesp_enabled && g_Options.sesp_notify_spotted)
	{
		if (time - slast_time > s_period)
		{
			if (g_sndNoticeSize > 0)
			{
				PlaySoundA(NULL, NULL, 0);
				PlaySoundA((LPCSTR)g_sndNotice, NULL, SND_ASYNC | SND_MEMORY);
			}
			else
				Hooks::hkPlaySound("UI/beep22.wav");
			slast_time = time;
		}
	}

	last = spotted;
}

//--------------------------------------------------------------------------------
void ESP::Misc::RenderCrosshair()
{
	int w, h;

	g_EngineClient->GetScreenSize(w, h);

	g_VGuiSurface->DrawSetColor(g_Options.color_esp_crosshair);

	int cx = w / 2;
	int cy = h / 2;

	g_VGuiSurface->DrawLine(cx - 25, cy, cx + 25, cy);
	g_VGuiSurface->DrawLine(cx, cy - 25, cx, cy + 25);
}
//--------------------------------------------------------------------------------
void ESP::Misc::RenderWeapon(C_BaseCombatWeapon* ent)
{
	wchar_t buf[80];
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	if (!ent || ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	g_VGuiSurface->DrawSetColor(g_Options.color_esp_weapons);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.left, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);

	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);

	if (MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, 80) > 0) {
		int w = bbox.right - bbox.left;
		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(g_Options.color_esp_weapons);
		g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}
//--------------------------------------------------------------------------------
void ESP::Misc::RenderDefuseKit(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	g_VGuiSurface->DrawSetColor(g_Options.color_esp_defuse);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);

	const wchar_t* buf = L"Defuse Kit";

	int w = bbox.right - bbox.left;
	int tw, th;
	g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
	g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
	g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
}
//--------------------------------------------------------------------------------
void ESP::Misc::RenderPlantedC4(C_BaseEntity* ent)
{
	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	g_VGuiSurface->DrawSetColor(g_Options.color_esp_c4);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);

	xstring title;
	xstring timer;
	C_PlantedC4* bomb = (C_PlantedC4*)ent;
	float time2blow = bomb->GetBombTime();
	float time2defuse = bomb->GetDefuseCountdown();
	float bomb_timer = (g_GlobalVars->curtime - time2blow) * -1;
	float def_timer = (g_GlobalVars->curtime - time2defuse) * -1;

	if (bomb_timer < 0.000f)
		bomb_timer = 0.000f;

	if (def_timer < 0.000f)
		def_timer = 0.000f;

	Color titleColor;
	if (!bomb->IsDefusing())
	{
		title = L"Bomb";
		titleColor = esp_ctx.clr;
	}
	else
	{
		title.Format(L"Bomb [DEFUSING: %[2]f sec]", def_timer);
		titleColor = g_Options.color_esp_defuse;
	}

	if(!bomb->IsBombDefused())
		timer.Format(L"Time to explosion: %[2]f sec", bomb_timer);
	else
		timer = L"[DEFUSED]";

	int w = bbox.right - bbox.left;
	int tw, th, tw2, th2;
	g_VGuiSurface->GetTextSize(esp_font, title.GetStr().c_str(), tw, th);
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(titleColor);
	g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
	g_VGuiSurface->DrawPrintText(title.GetStr().c_str(), title.GetLen());

	g_VGuiSurface->GetTextSize(esp_font, timer.GetStr().c_str(), tw2, th2);
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(g_Options.color_esp_c4);
	g_VGuiSurface->DrawSetTextPos(((bbox.left + w * 0.5f) - tw2 * 0.5f), bbox.bottom + 1 + th);
	g_VGuiSurface->DrawPrintText(timer.GetStr().c_str(), timer.GetLen());
}
//
void ESP::Misc::deprecated_GrenadePrediction(Color color)
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	auto activeWeapon = g_LocalPlayer->m_hActiveWeapon();
	if (!activeWeapon)
		return;

	if (activeWeapon->IsDormant())
		return;

	if (!activeWeapon->IsWeapon())
		return;

	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	QAngle qViewAngles;
	g_EngineClient->GetViewAngles(qViewAngles);
	Math::QAngleToVector(qViewAngles, vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	QAngle qThrow;
	Math::VectorToQAngle(vThrow, qThrow);
	Math::AngleVectors(qThrow, vThrow2);
	Math::QAngleToVector(qThrow, vThrow);

	Vector vEye = g_LocalPlayer->GetEyePos();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + g_LocalPlayer->m_vecVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + g_LocalPlayer->m_vecVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + g_LocalPlayer->m_vecVelocity()[2];

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = g_LocalPlayer;

		ray.Init(vStart, vPos);
		g_EngineTrace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit())
		{
			vThrow2 = tr.plane.normal * -2.0f * Math::DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2*tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));
		}

		Vector vOutStart, vOutEnd;
		if (activeWeapon->IsGrenade())
		{
			if (Math::WorldToScreen(vStart, vOutStart), Math::WorldToScreen(vPos, vOutEnd))
			{
				g_VGuiSurface->DrawSetColor(color);
				g_VGuiSurface->DrawLine(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y);
			}

			vStart = vPos;
			vThrow2.z -= fGravity*tr.fraction*fStep;
		}
	}
}

//--------------------------------------------------------------------------------

typedef std::map<int, bool> BeepChannels_t;

struct BeepData
{
	DWORD freq;
	DWORD dur;
	int chan;
	int hold;
	BeepChannels_t* bc;
};

void __stdcall BeepThread(LPVOID lpArg)
{
	BeepData* bd = reinterpret_cast<BeepData*>(lpArg);
	int channel = bd->chan;
	if(channel < 0)
		Beep(bd->freq, bd->dur);
	else
	{
		bool AlreadyBeeping = bd->bc->operator[](channel);
		if (AlreadyBeeping)
		{
			delete bd;
			return;
		}
		bd->bc->operator[](channel) = true;
		Beep(bd->freq, bd->dur);
		Sleep(bd->hold);
		bd->bc->operator[](channel) = false;
	}
	delete bd;
}

void __fastcall ESP::Sound::BeepAsync(DWORD freq, DWORD dur, int channel, int holdTime)
{
	static BeepChannels_t s_Channels;
	BeepData* bd = new BeepData;
	bd->freq = freq;
	bd->dur = dur;
	bd->chan = channel;
	bd->bc = &s_Channels;

	if (holdTime < 0)
		bd->hold = 0;
	else
		bd->hold = holdTime;
	
	if (channel >= 0 && s_Channels.find(channel) == s_Channels.end())
		s_Channels[channel] = false;

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)BeepThread, bd, 0, 0);
}

//--------------------------------------------------------------------------------

void ESP::Sound::IsAimingEnemy()
{
	C_BasePlayer* pl = nullptr;
	static float last_beep = 0.0f;
	float period = float(g_Options.sesp_whaim_period / 1000.0f);

	if (last_beep > g_GlobalVars->curtime)
		last_beep = 0.0f;

	if (g_GlobalVars->curtime - last_beep <= period)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;

	Vector vTraceStart = g_LocalPlayer->GetEyePos();
	Vector vTraceForward;
	QAngle va;
	g_EngineClient->GetViewAngles(va);
	QAngle qViewAngles = va;// +g_LocalPlayer->m_aimPunchAngle() * 2.0f;
	Math::AngleVectors(qViewAngles, vTraceForward);
	vTraceForward = vTraceForward.Normalized();
	Vector vTraceEnd = vTraceStart + (vTraceForward * 8192.0f);
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;
	ray.Init(vTraceStart, vTraceEnd);
	trace_t tr;


	for (size_t i = 1; i < 65; i++)
	{
		C_BasePlayer* tmp = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (!tmp)
			continue;

		if (!tmp->IsPlayer())
			continue;

		if (!tmp->IsAlive() || tmp->IsDormant())
			continue;

		if (tmp->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
			continue;

		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, tmp, &tr);

		if (tr.DidHit())
		{
			if (tr.hit_entity == tmp)
			{
				pl = tmp;
				break;
			}
		}
	}


	if (!pl)
		return;

	if (tr.hitbox < 0 || tr.hitbox >= HITBOX_MAX)
		return;

	DWORD freq, dur;
	int channel;
	freq = g_Options.sesp_whaim_freq;
	dur = g_Options.sesp_whaim_dur;
	channel = 0;

	BeepAsync(freq, dur, channel);

	last_beep = g_GlobalVars->curtime;
}

//--------------------------------------------------------------------------------

void ESP::Sound::EnemyIsNear()
{
	if (!g_LocalPlayer->IsAlive())
		return;

	Vector localPos = g_LocalPlayer->m_vecOrigin();
	Vector enemyPos;
	float lastDist = float(g_Options.sesp_distance_radius);
	float dist;
	bool found = false;

	static float last_beep = 0.0f;
	float time = g_GlobalVars->curtime;
	float period = 1.10f;

	if (last_beep > time)
		last_beep = 0.0f;

	for (size_t i = 1; i < 65; i++)
	{
		C_BasePlayer* pl = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

		if (!pl)
			continue;

		if (!pl->IsPlayer())
			continue;

		if (pl == g_LocalPlayer)
			continue;

		if (pl->IsDormant())
			continue;

		if (!pl->IsAlive())
			continue;

		if (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
			continue;

		enemyPos = pl->m_vecOrigin();
		dist = localPos.DistTo(enemyPos);
		if (dist < lastDist)
		{
			lastDist = dist;
			found = true;
		}
	}

	if (!found)
		return;

	period = lastDist / 1000.0f;

	if (time - last_beep > period)
	{
		DWORD freq, dur;
		int channel;
		freq = g_Options.sesp_distance_freq;
		dur = g_Options.sesp_distance_dur;
		channel = 1;

		BeepAsync(freq, dur, channel);
		last_beep = time;
	}
}

void ESP::Aimbot::DisplayInfo()
{
	xstring target, state, mode;

	target.Format(L"Aimbot target: %zs.a", CAimbot::Get().GetTargetName().c_str());
	state.Format(L"State: %zs.a", CAimbot::Get().GetState().c_str());

	switch (g_Options.ab_mode)
	{
		case CAimbot::MODE_DISABLED:
			mode = L"Mode: Disabled";
			break;

		case CAimbot::MODE_ONESHOT:
			mode = L"Mode: One-shot";
			break;

		case CAimbot::MODE_MANAGED:
			mode = L"Mode: Managed";
			break;

		case CAimbot::MODE_TOGGLE:
			mode = L"Mode: Toggle";
			break;

		default:
			__assume(0);
	}

	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);

	int tw, tw2, th, txt_x, txt_y;
	g_VGuiSurface->GetTextSize(esp_font, target.GetStr().c_str(), tw, th);
	g_VGuiSurface->GetTextSize(esp_font, state.GetStr().c_str(), tw2, th);

	if (tw2 > tw)
		tw = tw2;
	
	txt_y = 5 + th;
	txt_x = screen_w - tw - 50;

	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(g_Options.ab_info_color);
	g_VGuiSurface->DrawSetTextPos(txt_x, txt_y);
	g_VGuiSurface->DrawPrintText(target.GetStr().c_str(), target.GetLen());

	txt_y += th;
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(g_Options.ab_info_color);
	g_VGuiSurface->DrawSetTextPos(txt_x, txt_y);
	g_VGuiSurface->DrawPrintText(mode.GetStr().c_str(), mode.GetLen());

	txt_y += th;
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(g_Options.ab_info_color);
	g_VGuiSurface->DrawSetTextPos(txt_x, txt_y);
	g_VGuiSurface->DrawPrintText(state.GetStr().c_str(), state.GetLen());
}

void ESP::Player::RenderWall()
{
	int MidX;
	int MidY;
	g_EngineClient->GetScreenSize(MidX, MidY);
	xstring out;

	int damage;
	if (CanWallbang(damage))
	{
		g_VGuiSurface->DrawSetColor(Color(0, 255, 0));
		g_VGuiSurface->DrawOutlinedCircle(MidX / 2, MidY / 2, 10, 10);

		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextPos(MidX / 2, MidY / 2 + 6);
		g_VGuiSurface->DrawSetTextColor(Color(255, 255, 255, 255));
		out.Format(L"DMG: %i", damage);
		g_VGuiSurface->DrawPrintText(out.GetStr().c_str(), out.GetLen());
	}
	else
	{

		g_VGuiSurface->DrawSetTextColor(Color(255, 0, 0));
		g_VGuiSurface->DrawOutlinedCircle(MidX / 2, MidY / 2, 10, 10);

		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextPos(MidX / 2, MidY / 2 + 6);
		g_VGuiSurface->DrawSetTextColor(Color(255, 255, 255, 255));
		out = L"DMG: 0";
		g_VGuiSurface->DrawPrintText(out.GetStr().c_str(), out.GetLen());
	}
}