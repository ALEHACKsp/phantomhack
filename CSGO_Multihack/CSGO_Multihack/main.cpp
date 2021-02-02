#include <Windows.h>
#include <chrono>
#include <thread>
#include <process.h>
#include <tchar.h>
#include <ctime>
#include <strstream>
#include <TlHelp32.h>
#include <Psapi.h>
#include "MemMgr.h"
#include <xstring.h>


#define _DEFINE_OFFSETS_
#define _DEFINE_GLOBALS_
#define CfgRangeProtect(___value,___crpmin,___crpmax) if(___value<___crpmin)___value=___crpmin;else if(___value>___crpmax)___value=___crpmax


#include "offsets.h"
#include "sdk\include\csgosdk.h"
#include "Hooks.h"
#include "md5.h"
#include "ConfigMgr.h"
#include "globals.h"
#include "Aimbot.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3dx9.lib")

void vcc_to_intarr(Color vcc, volatile int col[3])
{
	col[0] = vcc.r();
	col[1] = vcc.g();
	col[2] = vcc.b();
}

Color intarr_to_vcc(volatile int col[3])
{
	Color c;
	c.SetColor(col[0], col[1], col[2], 255);
	return c;
}

void __fastcall SetAplha(Color &c, int a)
{
	c = Color(c.r(), c.g(), c.b(), a);
}

xstringA Amanthul(xstringA s)
{
	xstringA hokage(true);
	char buf[128];
	time_t seconds = time(NULL);
	tm timeinfo, *ti_ptr;
	localtime_s(&timeinfo, &seconds); ti_ptr = &timeinfo;
	strftime(buf, 128, "w53%A%B%d%Y%H%Ms", ti_ptr);
	hokage = md5(md5(buf)) + md5(s);
	return md5(hokage);
}

void SaveConfig();

void ConfigResetDefaults(bool save = true)
{
	Color tmpCol;
	//Chams
	/*g_ConfigManager.SetValue_Bool("Chams", "Enabled", false);
	g_ConfigManager.SetValue_Bool("Chams", "SeparateColorForInvis", true);
	g_ConfigManager.SetValue_Bool("Chams", "OnlyHead", false);
	g_ConfigManager.SetValue_Bool("Chams", "HeadDifferent", false);

	tmpCol.SetColor(0, 255, 0);
	g_ConfigManager.SetValue_Color("Chams", "GenericColor", tmpCol);
	tmpCol.SetColor(0, 75, 0);
	g_ConfigManager.SetValue_Color("Chams", "OccColor", tmpCol);
	tmpCol.SetColor(255, 0, 0);
	g_ConfigManager.SetValue_Color("Chams", "HeadColor", tmpCol);
	tmpCol.SetColor(75, 0, 0);
	g_ConfigManager.SetValue_Color("Chams", "OccHeadColor", tmpCol);*/

	g_ConfigManager.SetValue_Bool("Chams", "PlayersEnabled", false);
	g_ConfigManager.SetValue_Bool("Chams", "ChamsOnAllies", false);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersIgnoreZ", false);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersGlass", false);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersFlat", false);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersWireframe", false);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsEnabled", false);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsFlat", false);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsGlass", false);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsIgnoreZ", false);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsWireframe", false);

	g_ConfigManager.SetValue_Num<int>("Chams", "PlayersAlpha", 255);
	g_ConfigManager.SetValue_Num<int>("Chams", "ArmsAlpha", 255);

	tmpCol.SetColor(0, 0, 255, 255);
	g_ConfigManager.SetValue_Color("Chams", "ArmsVisibleColor", tmpCol);
	tmpCol.SetColor(0, 0, 70, 255);
	g_ConfigManager.SetValue_Color("Chams", "ArmsOccludedColor", tmpCol);
	tmpCol.SetColor(0, 255, 0, 255);
	g_ConfigManager.SetValue_Color("Chams", "AllyVisibleColor", tmpCol);
	tmpCol.SetColor(255, 0, 0, 255);
	g_ConfigManager.SetValue_Color("Chams", "EnemyVisibleColor", tmpCol);
	tmpCol.SetColor(0, 70, 0, 255);
	g_ConfigManager.SetValue_Color("Chams", "AllyOccludedColor", tmpCol);
	tmpCol.SetColor(70, 0, 0, 255);
	g_ConfigManager.SetValue_Color("Chams", "EnemyOccludedColor", tmpCol);


	//Glow
	g_ConfigManager.SetValue_Bool("Glow", "GlowPlayers", false);
	g_ConfigManager.SetValue_Bool("Glow", "GlowAllies", false);
	g_ConfigManager.SetValue_Bool("Glow", "GlowBombCarrier", false);
	g_ConfigManager.SetValue_Bool("Glow", "GlowBombPlanted", false);
	g_ConfigManager.SetValue_Bool("Glow", "GlowWeapons", false);
	//g_ConfigManager.SetValue_Bool("Glow", "GlowDefuseKits", false);

	tmpCol.SetColor(255, 0, 0, 255);
	g_ConfigManager.SetValue_Color("Glow", "EnemyColor", tmpCol);
	tmpCol.SetColor(0, 255, 0, 255);
	g_ConfigManager.SetValue_Color("Glow", "AllyColor", tmpCol);
	tmpCol.SetColor(255, 80, 100, 255);
	g_ConfigManager.SetValue_Color("Glow", "BombCarrierColor", tmpCol);
	tmpCol.SetColor(0, 0, 255, 255);
	g_ConfigManager.SetValue_Color("Glow", "BombPlantedColor", tmpCol);
	tmpCol.SetColor(255, 255, 255, 255);
	g_ConfigManager.SetValue_Color("Glow", "WeaponColor", tmpCol);

	g_ConfigManager.SetValue_Num<int>("Glow", "Alpha", 255);

	//TriggerBot
	g_ConfigManager.SetValue_Num<int>("TriggerBot", "Mode", 0);
	g_ConfigManager.SetValue_Num<DWORD>("TriggerBot", "HotKey", VK_ALT);
	g_ConfigManager.SetValue_Num<int>("TriggerBot", "PostfireLat", 0);
	g_ConfigManager.SetValue_Num<int>("TriggerBot", "Method", 0);
	g_ConfigManager.SetValue_Bool("TriggerBot", "IgnoreWalls", false);
	g_ConfigManager.SetValue_Bool("TriggerBot", "HeadsOnly", false);
	g_ConfigManager.SetValue_Bool("TriggerBot", "HoldFire", false);


	//ESP
	g_ConfigManager.SetValue_Bool("ESP", "Enabled", false);
	g_ConfigManager.SetValue_Num<int>("ESP", "FontSize", 10);
	g_ConfigManager.SetValue_Bool("ESP", "DefuseKits", false);
	g_ConfigManager.SetValue_Bool("ESP", "DroppedWeapons", false);
	g_ConfigManager.SetValue_Bool("ESP", "Allies", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlantedBomb", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerArmor", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerBoxes", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerBoxesHeadOnly", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerDistance", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerHealth", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerNames", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerSnaplines", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerWeapons", false);
	g_ConfigManager.SetValue_Bool("ESP", "GrenadePrediction", false);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerBones", false);
	g_ConfigManager.SetValue_Bool("ESP", "BonesColoredHP", false);
	g_ConfigManager.SetValue_Bool("ESP", "DefuseIndicator", false);
	g_ConfigManager.SetValue_Num<int>("ESP", "DefuseIndicatorType", 0);
	g_ConfigManager.SetValue_Num<int>("ESP", "NotifySpottedAlpha", 130);
	g_ConfigManager.SetValue_Bool("ESP", "NotifySpotted", false);
	g_ConfigManager.SetValue_Bool("ESP", "AlternativeZoom", false);
	g_ConfigManager.SetValue_Num<DWORD>("ESP", "ZoomKey", 69);
	g_ConfigManager.SetValue_Num<float>("ESP", "ZoomFOV", 65.0f);
	g_ConfigManager.SetValue_Num<float>("ESP", "ViewmodelFOV", 90.0f);
	g_ConfigManager.SetValue_Bool("ESP", "ZoomInstant", false);
	g_ConfigManager.SetValue_Bool("ESP", "ZoomDontWhenScope", false);
	g_ConfigManager.SetValue_Bool("ESP", "HideViewmodel", false);
	g_ConfigManager.SetValue_Bool("ESP", "HideViewmodelWhenZoom", false);

	//AimBot
	g_ConfigManager.SetValue_Num<int>("AimBot", "Mode", CAimbot::MODE_DISABLED);
	g_ConfigManager.SetValue_Num<int>("AimBot", "Method", CAimbot::METHOD_LINEAR);
	g_ConfigManager.SetValue_Num<DWORD>("AimBot", "Hotkey", VK_MBUTTON);
	g_ConfigManager.SetValue_Bool("AimBot", "RandomizeCurve", false);
	g_ConfigManager.SetValue_Bool("AimBot", "DisplayInfo", false);
	g_ConfigManager.SetValue_Bool("AimBot", "OnlyWhenScoped", false);
	g_ConfigManager.SetValue_Bool("AimBot", "RandomHitbox", false);
	g_ConfigManager.SetValue_Bool("AimBot", "HuntPlayer", false);
	g_ConfigManager.SetValue_Bool("AimBot", "DisableMouse", false);
	g_ConfigManager.SetValue_Num<float>("AimBot", "MinReactionDist", 0.0f);
	g_ConfigManager.SetValue_Num<float>("AimBot", "MaxReactionDist", 2000.0f);
	g_ConfigManager.SetValue_Num<float>("AimBot", "Velocity", 1.0f);
	g_ConfigManager.SetValue_Num<float>("AimBot", "VelocityReduction", 0.0f);
	g_ConfigManager.SetValue_Num<float>("AimBot", "Max2DDistanceToCrosshair", 600.0f);
	g_ConfigManager.SetValue_Num<float>("AimBot", "Acceleration", 0.05f); 
	tmpCol.SetColor(0, 255, 0, 255);
	g_ConfigManager.SetValue_Color("AimBot", "InfoColor", tmpCol);


	tmpCol.SetColor(0, 75, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "AllyOccludedColor", tmpCol);
	tmpCol.SetColor(0, 255, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "AllyVisibleColor", tmpCol);
	tmpCol.SetColor(75, 0, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "EnemyOccludedColor", tmpCol);
	tmpCol.SetColor(255, 0, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "EnemyVisibleColor", tmpCol);
	tmpCol.SetColor(0, 255, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "GrenadePredictionColor", tmpCol);
	tmpCol.SetColor(0, 175, 255, 255);
	g_ConfigManager.SetValue_Color("ESP", "PlantedC4Color", tmpCol);
	tmpCol.SetColor(255, 255, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "DefuseColor", tmpCol);
	tmpCol.SetColor(255, 0, 0, 255);
	g_ConfigManager.SetValue_Color("ESP", "NotifyColor", tmpCol);

	//sesp
	g_ConfigManager.SetValue_Bool("SoundESP", "Enabled", false);
	g_ConfigManager.SetValue_Bool("SoundESP", "NotifySpotted", false);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "NotifyPause", 3);
	g_ConfigManager.SetValue_Bool("SoundESP", "EnemyInCrosshair", false);
	g_ConfigManager.SetValue_Bool("SoundESP", "EnemyIsNear", false);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyInCrosshairFreq", 600);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyInCrosshairDur", 200);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyInCrosshairPeriod", 75);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyIsNearFreq", 250);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyIsNearDur", 200);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyIsNearRadius", 1000);

	//Crosshair
	g_ConfigManager.SetValue_Bool("Crosshair", "DrawCustom", false);
	g_ConfigManager.SetValue_Bool("Crosshair", "AlignToSpread", false);
	g_ConfigManager.SetValue_Bool("Crosshair", "HideDefault", false);

	g_ConfigManager.SetValue_Num<int>("Crosshair", "Type", 0);
	g_ConfigManager.SetValue_Bool("Crosshair", "WithDot", false);

	g_ConfigManager.SetValue_Num<int>("Crosshair", "Scale", 2);
	g_ConfigManager.SetValue_Num<int>("Crosshair", "DotScale", 2);
	g_ConfigManager.SetValue_Num<int>("Crosshair", "Thickness", 1);

	tmpCol.SetColor(100, 255, 115, 255);
	g_ConfigManager.SetValue_Color("Crosshair", "Color", tmpCol);

	//Other
	g_ConfigManager.SetValue_Bool("Other", "RadarHack", false);
	g_ConfigManager.SetValue_Bool("Other", "RevealRanks", false);
	g_ConfigManager.SetValue_Bool("Other", "FakeLag", false);
	g_ConfigManager.SetValue_Bool("Other", "FakeLagRandomize", false); 
		g_ConfigManager.SetValue_Bool("Other", "FakeLagStopByTriggerKey", false); 
	g_ConfigManager.SetValue_Num<int>("Other", "FakeLagPeriod", 1000); 
	g_ConfigManager.SetValue_Num<int>("Other", "FakeLagDuration", 1000);
	g_ConfigManager.SetValue_Bool("Other", "NoFlash", false);
	g_ConfigManager.SetValue_Num<float>("Other", "NoFlashAlpha", 0.00f);
	g_ConfigManager.SetValue_Num<DWORD>("Other", "AlertKey", VK_DELETE);

	//...save
	if (save)
	{
		g_ConfigManager.Save();
		g_ConfigManager.SaveAs(g_HackPath + g_ConfigName);
	}
}

void InitConfig(bool resave = true)
{
	if (!g_ConfigManager.Load(g_HackPath + g_ConfigName))
	{
		ConfigResetDefaults();

		if (!g_ConfigManager.Load(g_HackPath + g_ConfigName))
		{
			MessageBox(NULL, TEXT("Error creating config."), TEXT("Error"), MB_OK | MB_ICONERROR);
			exit(0);
			return;
		}
	}
		
	Color tmpCol;

	//Load glow
	g_Options.glow_enabled = true;
	g_Options.glow_players = g_ConfigManager.GetValue_Bool("Glow", "GlowPlayers", false);
	g_Options.glow_enemies_only = !g_ConfigManager.GetValue_Bool("Glow", "GlowAllies", true);
	g_Options.glow_c4_carrier = g_ConfigManager.GetValue_Bool("Glow", "GlowBombCarrier", false);
	g_Options.glow_planted_c4 = g_ConfigManager.GetValue_Bool("Glow", "GlowBombPlanted", false);
	g_Options.glow_weapons = g_ConfigManager.GetValue_Bool("Glow", "GlowWeapons", false);
	//g_Options.glow_defuse_kits = g_ConfigManager.GetValue_Bool("Glow", "GlowDefuseKits", false); 
	g_Options.glow_chickens = false;
	g_Options.glow_defuse_kits = false;

	g_cGlowAlpha = g_ConfigManager.GetValue_Num<int>("Glow", "Alpha", 255);
	tmpCol.SetColor(255, 0, 0, g_cGlowAlpha);
	g_Options.color_glow_enemy = g_ConfigManager.GetValue_Color("Glow", "EnemyColor", tmpCol);
	SetAplha(g_Options.color_glow_enemy, g_cGlowAlpha);

	tmpCol.SetColor(0, 255, 0, g_cGlowAlpha);
	g_Options.color_glow_ally = g_ConfigManager.GetValue_Color("Glow", "AllyColor", tmpCol);
	SetAplha(g_Options.color_glow_ally, g_cGlowAlpha);

	tmpCol.SetColor(0, 0, 255, g_cGlowAlpha);
	g_Options.color_glow_planted_c4 = g_ConfigManager.GetValue_Color("Glow", "BombPlantedColor", tmpCol);
	SetAplha(g_Options.color_glow_planted_c4, g_cGlowAlpha);

	tmpCol.SetColor(255, 80, 100, g_cGlowAlpha);
	g_Options.color_glow_c4_carrier = g_ConfigManager.GetValue_Color("Glow", "BombCarrierColor", tmpCol);
	SetAplha(g_Options.color_glow_c4_carrier, g_cGlowAlpha);

	tmpCol.SetColor(255, 255, 255, g_cGlowAlpha);
	g_Options.color_glow_weapons = g_ConfigManager.GetValue_Color("Glow", "WeaponColor", tmpCol);
	SetAplha(g_Options.color_glow_weapons, g_cGlowAlpha);
	

	//Load chams

	/*g_ChamsEnabled = g_ConfigManager.GetValue_Bool("Chams", "Enabled", false);
	g_ChamsSepInvisCol = g_ConfigManager.GetValue_Bool("Chams", "SeparateColorForInvis", true);
	g_bOnlyHead = g_ConfigManager.GetValue_Bool("Chams", "OnlyHead", false);
	g_bHeadDifCol = g_ConfigManager.GetValue_Bool("Chams", "HeadDifferent", false);

	tmpCol.SetColor(0, 255, 0);
	vcc_to_intarr(g_ConfigManager.GetValue_Color("Chams", "GenericColor", tmpCol), g_cGen);
	tmpCol.SetColor(0, 75, 0);
	vcc_to_intarr(g_ConfigManager.GetValue_Color("Chams", "OccColor", tmpCol), g_cInv);
	tmpCol.SetColor(255, 0, 0);
	vcc_to_intarr(g_ConfigManager.GetValue_Color("Chams", "HeadColor", tmpCol), g_cHead);
	tmpCol.SetColor(75, 0, 0);
	vcc_to_intarr(g_ConfigManager.GetValue_Color("Chams", "OccHeadColor", tmpCol), g_cInvHead);*/

	g_Options.chams_player_enabled = g_ConfigManager.GetValue_Bool("Chams", "PlayersEnabled", false);
	g_Options.chams_player_enemies_only = !g_ConfigManager.GetValue_Bool("Chams", "ChamsOnAllies", false);
	g_Options.chams_player_ignorez = g_ConfigManager.GetValue_Bool("Chams", "PlayersIgnoreZ", false);
	g_Options.chams_player_glass = g_ConfigManager.GetValue_Bool("Chams", "PlayersGlass", false);
	g_Options.chams_player_flat = g_ConfigManager.GetValue_Bool("Chams", "PlayersFlat", false);
	g_Options.chams_player_wireframe = g_ConfigManager.GetValue_Bool("Chams", "PlayersWireframe", false);
	g_Options.chams_arms_enabled = g_ConfigManager.GetValue_Bool("Chams", "ArmsEnabled", false);
	g_Options.chams_arms_flat = g_ConfigManager.GetValue_Bool("Chams", "ArmsFlat", false);
	g_Options.chams_arms_glass = g_ConfigManager.GetValue_Bool("Chams", "ArmsGlass", false);
	g_Options.chams_arms_ignorez = g_ConfigManager.GetValue_Bool("Chams", "ArmsIgnoreZ", false);
	g_Options.chams_arms_wireframe = g_ConfigManager.GetValue_Bool("Chams", "ArmsWireframe", false);

	g_Options.chams_alpha_p = g_ConfigManager.GetValue_Num<int>("Chams", "PlayersAlpha", 255);
	CfgRangeProtect(g_Options.chams_alpha_p, 0, 255);
	g_Options.chams_alpha_a = g_ConfigManager.GetValue_Num<int>("Chams", "ArmsAlpha", 255);
	CfgRangeProtect(g_Options.chams_alpha_a, 0, 255);

	tmpCol.SetColor(0, 0, 255, g_Options.chams_alpha_a);
	g_Options.color_chams_arms_visible = g_ConfigManager.GetValue_Color("Chams", "ArmsVisibleColor", tmpCol);
	SetAplha(g_Options.color_chams_arms_visible, g_Options.chams_alpha_a);

	tmpCol.SetColor(0, 0, 70, g_Options.chams_alpha_a);
	g_Options.color_chams_arms_occluded = g_ConfigManager.GetValue_Color("Chams", "ArmsOccludedColor", tmpCol);
	SetAplha(g_Options.color_chams_arms_occluded, g_Options.chams_alpha_a);

	tmpCol.SetColor(0, 255, 0, g_Options.chams_alpha_p);
	g_Options.color_chams_player_ally_visible = g_ConfigManager.GetValue_Color("Chams", "AllyVisibleColor", tmpCol);
	SetAplha(g_Options.color_chams_player_ally_visible, g_Options.chams_alpha_p);

	tmpCol.SetColor(255, 0, 0, g_Options.chams_alpha_p);
	g_Options.color_chams_player_enemy_visible = g_ConfigManager.GetValue_Color("Chams", "EnemyVisibleColor", tmpCol);
	SetAplha(g_Options.color_chams_player_enemy_visible, g_Options.chams_alpha_p);

	tmpCol.SetColor(0, 70, 0, g_Options.chams_alpha_p);
	g_Options.color_chams_player_ally_occluded = g_ConfigManager.GetValue_Color("Chams", "AllyOccludedColor", tmpCol);
	SetAplha(g_Options.color_chams_player_ally_occluded, g_Options.chams_alpha_p);

	tmpCol.SetColor(70, 0, 0, g_Options.chams_alpha_p);
	g_Options.color_chams_player_enemy_occluded = g_ConfigManager.GetValue_Color("Chams", "EnemyOccludedColor", tmpCol);
	SetAplha(g_Options.color_chams_player_enemy_occluded, g_Options.chams_alpha_p);
	

	//Load triggerbot
	g_TBMode = g_ConfigManager.GetValue_Num<int>("TriggerBot", "Mode", 0);
	if (g_TBMode > 2 || g_TBMode < 0)
		g_TBMode = 0;
	g_TriggerHotkey = g_ConfigManager.GetValue_Num<DWORD>("TriggerBot", "HotKey", VK_ALT);
	g_TPostDelay = g_ConfigManager.GetValue_Num<int>("TriggerBot", "PostfireLat", 0);
	CfgRangeProtect(g_TPostDelay, 0, 1000);
	g_TriggerBotMethod = g_ConfigManager.GetValue_Num<int>("TriggerBot", "Method", 0);
	CfgRangeProtect(g_TriggerBotMethod, 0, 1);
	g_TBIgnoreWalls = g_ConfigManager.GetValue_Bool("TriggerBot", "IgnoreWalls", false);
	g_TriggerHeadsOnly = g_ConfigManager.GetValue_Bool("TriggerBot", "HeadsOnly", false);
	g_TriggerHoldFire = g_ConfigManager.GetValue_Bool("TriggerBot", "HoldFire", false);

	//Load esp
	g_Options.esp_enabled = g_ConfigManager.GetValue_Bool("ESP", "Enabled", false);
	g_Options.esp_crosshair = false;
	g_Options.esp_font_size = g_ConfigManager.GetValue_Num<int>("ESP", "FontSize", 10);
	CfgRangeProtect(g_Options.esp_font_size, 10, 40);
	g_Options.esp_defuse_kit = g_ConfigManager.GetValue_Bool("ESP", "DefuseKits", false);
	g_Options.esp_dropped_weapons = g_ConfigManager.GetValue_Bool("ESP", "DroppedWeapons", false);
	g_Options.esp_enemies_only = !g_ConfigManager.GetValue_Bool("ESP", "Allies", false);
	g_Options.esp_planted_c4 = g_ConfigManager.GetValue_Bool("ESP", "PlantedBomb", false);
	g_Options.esp_player_armour = g_ConfigManager.GetValue_Bool("ESP", "PlayerArmor", false);
	g_Options.esp_player_boxes = g_ConfigManager.GetValue_Bool("ESP", "PlayerBoxes", false); 
	g_Options.esp_player_boxes_head_only = g_ConfigManager.GetValue_Bool("ESP", "PlayerBoxesHeadOnly", false);
	g_Options.esp_player_distance = g_ConfigManager.GetValue_Bool("ESP", "PlayerDistance", false);
	g_Options.esp_player_health = g_ConfigManager.GetValue_Bool("ESP", "PlayerHealth", false);
	g_Options.esp_player_names = g_ConfigManager.GetValue_Bool("ESP", "PlayerNames", false);
	g_Options.esp_player_snaplines = g_ConfigManager.GetValue_Bool("ESP", "PlayerSnaplines", false);
	g_Options.esp_player_weapons = g_ConfigManager.GetValue_Bool("ESP", "PlayerWeapons", false);
	g_Options.esp_GrenadePrediction = g_ConfigManager.GetValue_Bool("ESP", "GrenadePrediction", false);
	g_Options.esp_player_bones = g_ConfigManager.GetValue_Bool("ESP", "PlayerBones", false);
	g_Options.esp_bones_hp_colored = g_ConfigManager.GetValue_Bool("ESP", "BonesColoredHP", false);
	g_Options.esp_player_defuse_status = g_ConfigManager.GetValue_Bool("ESP", "DefuseIndicator", false);
	g_Options.esp_player_defuse_status_type = g_ConfigManager.GetValue_Num<int>("ESP", "DefuseIndicatorType", 0);
	CfgRangeProtect(g_Options.esp_player_defuse_status_type, 0, 1);
	g_Options.esp_notify_spotted = g_ConfigManager.GetValue_Bool("ESP", "NotifySpotted", false);
	g_Options.esp_notify_spotted_alpha = g_ConfigManager.GetValue_Num<int>("ESP", "NotifySpottedAlpha", 255);
	CfgRangeProtect(g_Options.esp_notify_spotted_alpha, 0, 255);
	g_Options.esp_zoom_enabled = g_ConfigManager.GetValue_Bool("ESP", "AlternativeZoom", false);
	g_Options.esp_zoom_key = g_ConfigManager.GetValue_Num<DWORD>("ESP", "ZoomKey", 69);
	g_Options.esp_zoom_fov = g_ConfigManager.GetValue_Num<float>("ESP", "ZoomFOV", 65.0f);
	CfgRangeProtect(g_Options.esp_zoom_fov, 1.0f, 160.0f);
	g_Options.esp_zoom_hands_fov = g_ConfigManager.GetValue_Num<float>("ESP", "ViewmodelFOV", 90.0f);
	CfgRangeProtect(g_Options.esp_zoom_hands_fov, 0.0f, 160.0f);
	g_Options.esp_zoom_instant = g_ConfigManager.GetValue_Bool("ESP", "ZoomInstant", false);
	g_Options.esp_zoom_dont_when_scope = g_ConfigManager.GetValue_Bool("ESP", "ZoomDontWhenScope", false);
	g_Options.esp_hide_viewmodel = g_ConfigManager.GetValue_Bool("ESP", "HideViewmodel", false);
	g_Options.esp_hide_viewmodel_when_zoom = g_ConfigManager.GetValue_Bool("ESP", "HideViewmodelWhenZoom", false);
	
	//sesp
	g_Options.sesp_enabled = g_ConfigManager.GetValue_Bool("SoundESP", "Enabled", false);
	g_Options.sesp_notify_spotted = g_ConfigManager.GetValue_Bool("SoundESP", "NotifySpotted", false);
	g_Options.sesp_notify_spotted_pause = g_ConfigManager.GetValue_Num<int>("SoundESP", "NotifyPause", 3);
	CfgRangeProtect(g_Options.sesp_notify_spotted_pause, 0, 15);
	g_Options.sesp_whaim_esp = g_ConfigManager.GetValue_Bool("SoundESP", "EnemyInCrosshair", false);
	g_Options.sesp_distance_esp = g_ConfigManager.GetValue_Bool("SoundESP", "EnemyIsNear", false);
	g_Options.sesp_whaim_freq = g_ConfigManager.GetValue_Num<int>("SoundESP", "EnemyInCrosshairFreq", 600);
	CfgRangeProtect(g_Options.sesp_whaim_freq, 50, 750);
	g_Options.sesp_whaim_dur = g_ConfigManager.GetValue_Num<int>("SoundESP", "EnemyInCrosshairDur", 200);
	CfgRangeProtect(g_Options.sesp_whaim_dur, 50, 750);
	g_Options.sesp_whaim_period = g_ConfigManager.GetValue_Num<int>("SoundESP", "EnemyInCrosshairPeriod", 75);
	CfgRangeProtect(g_Options.sesp_whaim_period, 0, 1000);
	g_Options.sesp_distance_freq = g_ConfigManager.GetValue_Num<int>("SoundESP", "EnemyIsNearFreq", 250);
	CfgRangeProtect(g_Options.sesp_distance_freq, 50, 750);
	g_Options.sesp_distance_dur = g_ConfigManager.GetValue_Num<int>("SoundESP", "EnemyIsNearDur", 200);
	CfgRangeProtect(g_Options.sesp_distance_dur, 50, 750);
	g_Options.sesp_distance_radius = g_ConfigManager.GetValue_Num<int>("SoundESP", "EnemyIsNearRadius", 1000);
	CfgRangeProtect(g_Options.sesp_distance_radius, 90, 1500);

	//AimBot
	g_Options.ab_mode = g_ConfigManager.GetValue_Num<int>("AimBot", "Mode", CAimbot::MODE_DISABLED);
	CfgRangeProtect(g_Options.ab_mode, 0, 3);
	g_Options.ab_method = g_ConfigManager.GetValue_Num<int>("AimBot", "Method", CAimbot::METHOD_LINEAR);
	CfgRangeProtect(g_Options.ab_method, 0, 2);
	g_Options.ab_hotkey = g_ConfigManager.GetValue_Num<DWORD>("AimBot", "Hotkey", VK_MBUTTON);
	g_Options.ab_randomize_curve = g_ConfigManager.GetValue_Bool("AimBot", "RandomizeCurve", false);
	g_Options.ab_display_info = g_ConfigManager.GetValue_Bool("AimBot", "DisplayInfo", false);
	g_Options.ab_aim_only_when_scoped = g_ConfigManager.GetValue_Bool("AimBot", "OnlyWhenScoped", false);
	g_Options.ab_random_hitbox = g_ConfigManager.GetValue_Bool("AimBot", "RandomHitbox", false);
	g_Options.ab_hunt_player = g_ConfigManager.GetValue_Bool("AimBot", "HuntPlayer", false);
	g_Options.ab_disable_mouse = g_ConfigManager.GetValue_Bool("AimBot", "DisableMouse", false);
	g_Options.ab_min_reaction_distance = g_ConfigManager.GetValue_Num<float>("AimBot", "MinReactionDist", 0.0f);
	CfgRangeProtect(g_Options.ab_min_reaction_distance, 0.0f, 450.0f);
	g_Options.ab_max_reaction_distance = g_ConfigManager.GetValue_Num<float>("AimBot", "MaxReactionDist", 2000.0f);
	CfgRangeProtect(g_Options.ab_max_reaction_distance, 800.0f, 10000.0f);
	g_Options.ab_velocity = g_ConfigManager.GetValue_Num<float>("AimBot", "Velocity", 1.0f);
	CfgRangeProtect(g_Options.ab_velocity, 0.10f, 2.0f);
	g_Options.ab_velocity_reduction = g_ConfigManager.GetValue_Num<float>("AimBot", "VelocityReduction", 0.0f);
	CfgRangeProtect(g_Options.ab_velocity_reduction, 0.0f, 0.75f);
	g_Options.ab_max_2Ddistance_to_crosshair = g_ConfigManager.GetValue_Num<float>("AimBot", "Max2DDistanceToCrosshair", 600.0f);
	CfgRangeProtect(g_Options.ab_max_2Ddistance_to_crosshair, 25.0f, 2000.0f);
	g_Options.ab_acceleration = g_ConfigManager.GetValue_Num<float>("AimBot", "Acceleration", 0.05f); 
	CfgRangeProtect(g_Options.ab_acceleration, 0.01f, 0.30f);
	tmpCol.SetColor(0, 255, 0, 255);
	g_Options.ab_info_color = g_ConfigManager.GetValue_Color("AimBot", "InfoColor", tmpCol);

	tmpCol.SetColor(0, 75, 0, 255);
	g_Options.color_esp_ally_occluded = g_ConfigManager.GetValue_Color("ESP", "AllyOccludedColor", tmpCol);
	tmpCol.SetColor(0, 255, 0, 255);
	g_Options.color_esp_ally_visible = g_ConfigManager.GetValue_Color("ESP", "AllyVisibleColor", tmpCol);
	tmpCol.SetColor(75, 0, 0, 255);
	g_Options.color_esp_enemy_occluded = g_ConfigManager.GetValue_Color("ESP", "EnemyOccludedColor", tmpCol);
	tmpCol.SetColor(255, 0, 0, 255);
	g_Options.color_esp_enemy_visible = g_ConfigManager.GetValue_Color("ESP", "EnemyVisibleColor", tmpCol);
	tmpCol.SetColor(0, 255, 0, 255);
	g_Options.color_esp_grenade = g_ConfigManager.GetValue_Color("ESP", "GrenadePredictionColor", tmpCol);
	tmpCol.SetColor(0, 175, 255, 255);
	g_Options.color_esp_c4 = g_ConfigManager.GetValue_Color("ESP", "PlantedC4Color", tmpCol);
	tmpCol.SetColor(255, 255, 0, 255); 
	g_Options.color_esp_defuse = g_ConfigManager.GetValue_Color("ESP", "DefuseColor", tmpCol);
	tmpCol.SetColor(255, 255, 255, 255);
	g_Options.color_esp_notify = g_ConfigManager.GetValue_Color("ESP", "NotifyColor", tmpCol);
	g_Options.color_esp_weapons = g_Options.color_glow_weapons;
	tmpCol.SetColor(0, 0, 0);
	g_Options.color_esp_crosshair = tmpCol;
	
	//Load crosshair
	g_bCrosshair = g_ConfigManager.GetValue_Bool("Crosshair", "DrawCustom", false);
	g_bCrosshairSpread = g_ConfigManager.GetValue_Bool("Crosshair", "AlignToSpread", false);
	g_bHideDefault = g_ConfigManager.GetValue_Bool("Crosshair", "HideDefault", false);

	g_iCrosshairType = g_ConfigManager.GetValue_Num<int>("Crosshair", "Type", 0);
	if (g_iCrosshairType > 4 || g_iCrosshairType < 0)
		g_iCrosshairType = 0;
	g_ExtraDot = g_ConfigManager.GetValue_Bool("Crosshair", "WithDot", false);

	g_iCrosshairSize = g_ConfigManager.GetValue_Num<int>("Crosshair", "Scale", 2);
	CfgRangeProtect(g_iCrosshairSize, 2, 30);
	g_iDotScale = g_ConfigManager.GetValue_Num<int>("Crosshair", "DotScale", 2);
	CfgRangeProtect(g_iDotScale, 2, 5);
	g_iCrosshairThickness = g_ConfigManager.GetValue_Num<int>("Crosshair", "Thickness", 1);
	CfgRangeProtect(g_iCrosshairThickness, 1, 5);

	tmpCol.SetColor(100, 255, 115, 255);
	Color CrossColor = g_ConfigManager.GetValue_Color("Crosshair", "Color", tmpCol);
	g_cCrosshair[0] = CrossColor.r();
	g_cCrosshair[1] = CrossColor.g();
	g_cCrosshair[2] = CrossColor.b();
	g_cCrosshair[3] = CrossColor.a();

	//Load other
	g_RH = g_ConfigManager.GetValue_Bool("Other", "RadarHack", false);
	g_RevealRanks = g_ConfigManager.GetValue_Bool("Other", "RevealRanks", false);
	g_FakeLag = g_ConfigManager.GetValue_Bool("Other", "FakeLag", false);
	g_FakeLagRandomize = g_ConfigManager.GetValue_Bool("Other", "FakeLagRandomize", false); 
	g_FakeLagUseKey = g_ConfigManager.GetValue_Bool("Other", "FakeLagStopByTriggerKey", false);
	g_FakeLagPeriod = g_ConfigManager.GetValue_Num<int>("Other", "FakeLagPeriod", 500);
	CfgRangeProtect(g_FakeLagPeriod, 125, 2000);
	g_FakeLagDuration = g_ConfigManager.GetValue_Num<int>("Other", "FakeLagDuration", 500);
	CfgRangeProtect(g_FakeLagDuration, 125, 2000);
	g_NF = g_ConfigManager.GetValue_Bool("Other", "NoFlash", false);
	g_FlashAlpha = g_ConfigManager.GetValue_Num<float>("Other", "NoFlashAlpha", 0.00f);
	CfgRangeProtect(g_FlashAlpha, 0.0f, 255.0f);
	g_dwAlertKey = g_ConfigManager.GetValue_Num<DWORD>("Other", "AlertKey", VK_DELETE);

	if(resave)
		SaveConfig();
}

void ResetAndReinitConfig()
{
	ConfigResetDefaults();
	InitConfig();
}

void InitOffsets()
{
	off_bSpotted = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_bSpotted");
	off_dwLocalPlayer = Utils::PatternScanEx(L"client.dll", "A3 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 59 C3 6A", PS_METHOD_BOTH, 0x1 /*PatternOffset*/, 0x10 /*AddressOffset*/);
	off_EntityList = Utils::PatternScanEx(L"client.dll", "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", PS_METHOD_BOTH, 0x1 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_iTeam = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_iTeamNum");
	off_oDormant = Utils::PatternScanEx(L"client.dll", "88 9E ? ? ? ? E8 ? ? ? ? 53 8D 8E ? ? ? ? E8 ? ? ? ? 8B 06 8B CE 53 FF 90 ? ? ? ? 8B 46 64 0F B6 CB 5E 5B 66 89 0C C5 ? ? ? ? 5D C2 04 00",
		PS_METHOD_READ, 0x2 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_glowObject = Utils::PatternScanEx(L"client.dll", "A1 ? ? ? ? A8 01 75 4B", PS_METHOD_BOTH, 0x1 /*PatternOffset*/, 0x4 /*AddressOffset*/);
	off_glowIndex = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashDuration") + 0x18;
	off_bSpottedMask = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_bSpottedByMask");
	off_iHealth = NetvarSys::Get().GetOffset("DT_BasePlayer", "m_iHealth");
	off_iCrossHairID = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_bHasDefuser") + 0x5C;
	off_Attack = Utils::PatternScanEx(L"client.dll", "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04", PS_METHOD_BOTH, 0x2 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_EntityLoopDistance = 0x10; //Default
	off_FlashMaxAlpha = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	off_FlashDuration = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashDuration");
	off_EnginePointer = Utils::PatternScanEx(L"engine.dll", "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0", PS_METHOD_BOTH, 0x1 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_ClientState_Map = Utils::PatternScanEx(L"engine.dll", "05 ? ? ? ? C3 CC CC CC CC CC CC CC A1", PS_METHOD_READ, 0x1 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_PunchAngle = NetvarSys::Get().GetOffset("DT_BasePlayer", "m_aimPunchAngle");
	off_dwViewAngles = Utils::PatternScanEx(L"engine.dll", "F3 0F 11 80 ? ? ? ? D9 46 04 D9 05", PS_METHOD_READ, 0x4 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_aimPunchAngle = NetvarSys::Get().GetOffset("DT_BasePlayer", "m_aimPunchAngle");
	off_viewPunchAngle = NetvarSys::Get().GetOffset("DT_BasePlayer", "m_viewPunchAngle");
	off_aimPunchAngleVel = NetvarSys::Get().GetOffset("DT_BasePlayer", "m_aimPunchAngleVel");
	off_cmd = Utils::PatternScanEx(L"engine.dll", "55 8B EC 8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 0C A1 ? ? ? ? 35 ? ? ? ? EB 0B", PS_METHOD_BOTH, 0x1 /*PatternOffset*/, 0x0 /*AddressOffset*/);
	off_eyeAngles = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_angEyeAngles");
}

/*
bool CheckActivation(std::string left, std::string right)
{
	std::string sample = md5(md5(left) + left + left + md5(left) + md5(left));
	if (sample == right)
		return true;
	else 
		return false;
}*/

void __stdcall HacksLoopThread(LPVOID lpThreadParameter)
{
	bool hided = true;
	while (true)
	{
		WaitForSingleObject(g_hAlertKeyMutex, INFINITE);
		if (GetAsyncKeyState(g_dwAlertKey) & 1)
		{
			g_DisableAll = !g_DisableAll;
			if(g_DisableAll)
				hided = false;
		}
		ReleaseMutex(g_hAlertKeyMutex);

		if(!g_DisableAll)
		{
			RadarHack();
			NoFlash();
			if (g_bHideDefault && !hided)
			{
				g_EngineClient->ExecuteClientCmd("cl_crosshairalpha 0");
				hided = true;
			}
		}
		else
		{
			if (g_bHideDefault && hided)
			{
				g_EngineClient->ExecuteClientCmd("cl_crosshairalpha 255");
				hided = false;
			}
		}
		Sleep(100);
	}
}

void __stdcall InitThread(LPVOID lpThreadParameter)
{
	
	HANDLE CorrectApp = NULL;

	CorrectApp = GetModuleHandle(TEXT("client.dll"));
	if (!CorrectApp)
	{
		MessageBox(NULL, TEXT("Incorrect application."), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	CorrectApp = GetModuleHandle(TEXT("engine.dll"));
	if (!CorrectApp)
	{
		MessageBox(NULL, TEXT("Incorrect application."), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}
	g_CorrectProcess = true;

#ifndef _DEBUG
	HANDLE hPipe = NULL;
	char bufPipe[1024];
	DWORD dwRead = 0;
	bool SuccessActivation = true;
	xstringA bsign(true), esign(true), delimiter(true);


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\FESPH"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	if (!hPipe)
	{
		MessageBox(NULL, TEXT("Critical error 0x021"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	bool stop = false;
	while (hPipe)
	{
		if (stop)
			break;
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, bufPipe, sizeof(bufPipe) - 1, &dwRead, NULL) != FALSE)
			{
				bufPipe[dwRead] = '\0';
				if (strlen(bufPipe) > 1)
					stop = true;
				break;
			}
		}
	}
	DisconnectNamedPipe(hPipe);

	xstringA RecievedData(bufPipe, true);
	xstringA left(true), right(true);
	bsign = "*#@dsfd@heaven$ad#gsd$";
	esign = "($@$)chaoticF3";
	delimiter = "ef37fjd10g25fd01x45[chaotic+2]null";

	if (RecievedData.GetStr().find(bsign) == std::string::npos)
		SuccessActivation = false;
	if (RecievedData.GetStr().find(esign) == std::string::npos)
		SuccessActivation = false;
	if (RecievedData.GetStr().find(delimiter) == std::string::npos)
		SuccessActivation = false;

	size_t ac_begin = RecievedData.GetStr().find(bsign);
	size_t ac_end = RecievedData.GetStr().find(esign);
	size_t ac_delimiter = RecievedData.GetStr().find(delimiter);
	size_t ac_delimiter_end = ac_delimiter + delimiter.GetLen();


	if (ac_begin != 0)
		SuccessActivation = false;

	if (ac_delimiter >= ac_end)
		SuccessActivation = false;

	if (!SuccessActivation)
	{
		MessageBox(NULL, TEXT("Incorrect usage of application. Injection failed."), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	size_t left_len = 32;
	size_t right_len = ac_end - ac_delimiter_end;
	left = RecievedData.GetStr().substr(bsign.GetLen(), left_len);
	right = RecievedData.GetStr().substr(ac_delimiter_end, right_len);

	if (left == Amanthul(right))
		SuccessActivation = true;
	else
		SuccessActivation = false;

	//SuccessActivation = CheckActivation(left, right);

	if (!SuccessActivation)
	{
		exit(0);
		return;
	}

	std::istringstream ss(right.GetStr());
	ss >> std::hex >> g_ActivatorProcID;
	g_hActivatorProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, TRUE, g_ActivatorProcID);
	if (!g_hActivatorProc)
	{
		exit(0);
		return;
	}
	char str[512];
	GetModuleFileNameExA(g_hActivatorProc, NULL, str, 512);
	g_HackPath = str;
	size_t _off = 0, cutindex = 0;
	while (true)
	{
		if (g_HackPath.find("\\", _off) != std::string::npos)
		{
			cutindex = g_HackPath.find("\\", _off);
			_off = cutindex + 1;
		}
		else
			break;
	}
	g_HackPath = g_HackPath.substr(0, _off);
#else

	g_HackPath = "D:\\ph\\";

#endif


	InitConfig();

	Interfaces::Initialize();
	Interfaces::Dump();
	NetvarSys::Get().Initialize();
	NetvarSys::Get().Dump();
	InputSys::Get().Initialize();
	Hooks::Initialize(); 
	
	InitOffsets();
	CAimbot::Get().Init();

	const LPTSTR sndNoticePath = _T("csgo\\sound\\UI\\spotted.wav");
	HANDLE hSndNotice = CreateFile(sndNoticePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hSndNotice || hSndNotice == (HANDLE)0xffffffff)
		g_sndNoticeSize = 0;
	else
	{
		DWORD dw = 0;
		g_sndNoticeSize = GetFileSize(hSndNotice, NULL);
		g_sndNotice = new BYTE[g_sndNoticeSize];
		ReadFile(hSndNotice, g_sndNotice, g_sndNoticeSize, &dw, NULL);
		CloseHandle(hSndNotice);
	}


	if (g_bHideDefault)
		g_EngineClient->ExecuteClientCmd("cl_crosshairalpha 0");
	else
		g_EngineClient->ExecuteClientCmd("cl_crosshairalpha 255");

	hTriggerHotkeyMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hTriggerHotkeyMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x0B"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	hFlashMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hFlashMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x0E"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}


	hCrossMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hCrossMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x0F"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}


	hRadarMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hRadarMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x10"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}


	hMenuMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hMenuMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x12"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	hFakeLagMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hFakeLagMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x13"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	g_Options.esp_zoom_key_mutex = CreateMutex(NULL, FALSE, NULL);
	if (!g_Options.esp_zoom_key_mutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x1F"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	g_Options.ab_key_mutex = CreateMutex(NULL, FALSE, NULL);
	if (!g_Options.ab_key_mutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x20"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}

	g_hAlertKeyMutex = CreateMutex(NULL, FALSE, NULL);
	if (!g_hAlertKeyMutex)
	{
		MessageBox(NULL, TEXT("Critical error 0x21"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}
	
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HacksLoopThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FakeLagThread, 0, 0, 0);

	oWndProc = (WNDPROC)SetWindowLongPtr(FindWindow(NULL, TEXT("Counter-Strike: Global Offensive")), GWL_WNDPROC, (LONG)(LONG_PTR)hkWndProc);
	if (!oWndProc)
	{
		MessageBox(NULL, TEXT("Critical error 0x09"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
		return;
	}


	g_EngineClient->ExecuteClientCmd("echo PhantomHack has loaded successfully. Press INSERT for menu.");
	Beep(0x1F4u, 0x12Cu);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitThread, 0, 0, 0);
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		Utils::DetachConsole();
		break;
	}
	return TRUE;
}
