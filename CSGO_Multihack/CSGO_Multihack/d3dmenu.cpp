#include "d3d.h"
#include "imgui\imgui.h"
#include "imgui\imgui_internal.h"
#include "imgui\dx9\imgui_impl_dx9.h"
#include "imgui\stb_rect_pack.h"
#include "imgui\stb_textedit.h"
#include "imgui\stb_truetype.h"
#include "imgui\imguial_msgbox.h"
#include <xstring.h>
#include "sdk\include\csgosdk.h"
#include "ConfigMgr.h"
#include "globals.h"


bool gui_bInit = false;
D3DDEVICE_CREATION_PARAMETERS gui_d3dcp{ 0 };
bool ColorPicker(const char* label, ImColor* color);


int cl_mouseenable = 1;
ImGuiContext* igctx;
static bool bhd_last = true;

void ResetAndReinitConfig();
void InitConfig(bool resave = true);
Color intarr_to_vcc(volatile int col[3]);

void SaveConfig()
{
	//Chams
	/*g_ConfigManager.SetValue_Bool("Chams", "Enabled", g_ChamsEnabled);
	g_ConfigManager.SetValue_Bool("Chams", "SeparateColorForInvis", g_ChamsSepInvisCol);
	g_ConfigManager.SetValue_Bool("Chams", "OnlyHead", g_bOnlyHead);
	g_ConfigManager.SetValue_Bool("Chams", "HeadDifferent", g_bHeadDifCol);

	g_ConfigManager.SetValue_Color("Chams", "GenericColor", intarr_to_vcc(g_cGen));
	g_ConfigManager.SetValue_Color("Chams", "OccColor", intarr_to_vcc(g_cInv));
	g_ConfigManager.SetValue_Color("Chams", "HeadColor", intarr_to_vcc(g_cHead));
	g_ConfigManager.SetValue_Color("Chams", "OccHeadColor", intarr_to_vcc(g_cInvHead));*/

	g_ConfigManager.SetValue_Bool("Chams", "PlayersEnabled", g_Options.chams_player_enabled);
	g_ConfigManager.SetValue_Bool("Chams", "ChamsOnAllies", !g_Options.chams_player_enemies_only);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersIgnoreZ", g_Options.chams_player_ignorez);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersGlass", g_Options.chams_player_glass);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersFlat", g_Options.chams_player_flat);
	g_ConfigManager.SetValue_Bool("Chams", "PlayersWireframe", g_Options.chams_player_wireframe);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsEnabled", g_Options.chams_arms_enabled);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsFlat", g_Options.chams_arms_flat);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsGlass", g_Options.chams_arms_glass);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsIgnoreZ", g_Options.chams_arms_ignorez);
	g_ConfigManager.SetValue_Bool("Chams", "ArmsWireframe", g_Options.chams_arms_wireframe);

	g_ConfigManager.SetValue_Num<int>("Chams", "PlayersAlpha", static_cast<int>(g_Options.chams_alpha_p));
	g_ConfigManager.SetValue_Num<int>("Chams", "ArmsAlpha", static_cast<int>(g_Options.chams_alpha_a));

	g_ConfigManager.SetValue_Color("Chams", "ArmsVisibleColor", g_Options.color_chams_arms_visible);
	g_ConfigManager.SetValue_Color("Chams", "ArmsOccludedColor", g_Options.color_chams_arms_occluded);
	g_ConfigManager.SetValue_Color("Chams", "AllyVisibleColor", g_Options.color_chams_player_ally_visible);
	g_ConfigManager.SetValue_Color("Chams", "EnemyVisibleColor", g_Options.color_chams_player_enemy_visible);
	g_ConfigManager.SetValue_Color("Chams", "AllyOccludedColor", g_Options.color_chams_player_ally_occluded);
	g_ConfigManager.SetValue_Color("Chams", "EnemyOccludedColor", g_Options.color_chams_player_enemy_occluded);

	//Glow
	g_ConfigManager.SetValue_Bool("Glow", "GlowPlayers", g_Options.glow_players);
	g_ConfigManager.SetValue_Bool("Glow", "GlowAllies", !g_Options.glow_enemies_only);
	g_ConfigManager.SetValue_Bool("Glow", "GlowBombCarrier", g_Options.glow_c4_carrier);
	g_ConfigManager.SetValue_Bool("Glow", "GlowBombPlanted", g_Options.glow_planted_c4);
	g_ConfigManager.SetValue_Bool("Glow", "GlowWeapons", g_Options.glow_weapons);
	//g_ConfigManager.SetValue_Bool("Glow", "GlowDefuseKits", g_Options.glow_defuse_kits);

	g_ConfigManager.SetValue_Color("Glow", "EnemyColor", g_Options.color_glow_enemy);
	g_ConfigManager.SetValue_Color("Glow", "AllyColor", g_Options.color_glow_ally);
	g_ConfigManager.SetValue_Color("Glow", "BombCarrierColor", g_Options.color_glow_c4_carrier);
	g_ConfigManager.SetValue_Color("Glow", "BombPlantedColor", g_Options.color_glow_planted_c4);
	g_ConfigManager.SetValue_Color("Glow", "WeaponColor", g_Options.color_glow_weapons);
	//g_ConfigManager.SetValue_Color("Glow", "DefuseColor", g_Options.color_glow_defuse);
	g_ConfigManager.SetValue_Num<int>("Glow", "Alpha", static_cast<int>(g_cGlowAlpha));

	//TriggerBot
	g_ConfigManager.SetValue_Num<int>("TriggerBot", "Mode", static_cast<int>(g_TBMode));
	g_ConfigManager.SetValue_Num<DWORD>("TriggerBot", "HotKey", static_cast<DWORD>(g_TriggerHotkey));
	g_ConfigManager.SetValue_Num<int>("TriggerBot", "PostfireLat", static_cast<int>(g_TPostDelay));
	g_ConfigManager.SetValue_Num<int>("TriggerBot", "Method", static_cast<int>(g_TriggerBotMethod));
	g_ConfigManager.SetValue_Bool("TriggerBot", "IgnoreWalls", g_TBIgnoreWalls); 
	g_ConfigManager.SetValue_Bool("TriggerBot", "HeadsOnly", g_TriggerHeadsOnly);
	g_ConfigManager.SetValue_Bool("TriggerBot", "HoldFire", g_TriggerHoldFire);

	//ESP
	g_ConfigManager.SetValue_Bool("ESP", "Enabled", g_Options.esp_enabled);
	g_ConfigManager.SetValue_Num<int>("ESP", "FontSize", g_Options.esp_font_size);
	g_ConfigManager.SetValue_Bool("ESP", "DefuseKits", g_Options.esp_defuse_kit);
	g_ConfigManager.SetValue_Bool("ESP", "DroppedWeapons", g_Options.esp_dropped_weapons);
	g_ConfigManager.SetValue_Bool("ESP", "Allies", !g_Options.esp_enemies_only);
	g_ConfigManager.SetValue_Bool("ESP", "PlantedBomb", g_Options.esp_planted_c4);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerArmor", g_Options.esp_player_armour);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerBoxes", g_Options.esp_player_boxes);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerBoxesHeadOnly", g_Options.esp_player_boxes_head_only);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerDistance", g_Options.esp_player_distance);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerHealth", g_Options.esp_player_health);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerNames", g_Options.esp_player_names);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerSnaplines", g_Options.esp_player_snaplines);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerWeapons", g_Options.esp_player_weapons);
	g_ConfigManager.SetValue_Bool("ESP", "GrenadePrediction", g_Options.esp_GrenadePrediction);
	g_ConfigManager.SetValue_Bool("ESP", "PlayerBones", g_Options.esp_player_bones);
	g_ConfigManager.SetValue_Bool("ESP", "BonesColoredHP", g_Options.esp_bones_hp_colored);
	g_ConfigManager.SetValue_Bool("ESP", "DefuseIndicator", g_Options.esp_player_defuse_status);
	g_ConfigManager.SetValue_Num<int>("ESP", "DefuseIndicatorType", static_cast<int>(g_Options.esp_player_defuse_status_type));
	g_ConfigManager.SetValue_Bool("ESP", "NotifySpotted", g_Options.esp_notify_spotted);
	g_ConfigManager.SetValue_Num<int>("ESP", "NotifySpottedAlpha", static_cast<int>(g_Options.esp_notify_spotted_alpha));
	g_ConfigManager.SetValue_Bool("ESP", "AlternativeZoom", g_Options.esp_zoom_enabled);
	g_ConfigManager.SetValue_Num<DWORD>("ESP", "ZoomKey", g_Options.esp_zoom_key);
	g_ConfigManager.SetValue_Num<float>("ESP", "ZoomFOV", g_Options.esp_zoom_fov);
	g_ConfigManager.SetValue_Num<float>("ESP", "ViewmodelFOV", g_Options.esp_zoom_hands_fov);
	g_ConfigManager.SetValue_Bool("ESP", "ZoomInstant", g_Options.esp_zoom_instant);
	g_ConfigManager.SetValue_Bool("ESP", "ZoomDontWhenScope", g_Options.esp_zoom_dont_when_scope);
	g_ConfigManager.SetValue_Bool("ESP", "HideViewmodel", g_Options.esp_hide_viewmodel);
	g_ConfigManager.SetValue_Bool("ESP", "HideViewmodelWhenZoom", g_Options.esp_hide_viewmodel_when_zoom);

	g_ConfigManager.SetValue_Color("ESP", "AllyOccludedColor", g_Options.color_esp_ally_occluded);
	g_ConfigManager.SetValue_Color("ESP", "AllyVisibleColor", g_Options.color_esp_ally_visible);
	g_ConfigManager.SetValue_Color("ESP", "EnemyOccludedColor", g_Options.color_esp_enemy_occluded);
	g_ConfigManager.SetValue_Color("ESP", "EnemyVisibleColor", g_Options.color_esp_enemy_visible);
	g_ConfigManager.SetValue_Color("ESP", "GrenadePredictionColor", g_Options.color_esp_grenade);
	g_ConfigManager.SetValue_Color("ESP", "PlantedC4Color", g_Options.color_esp_c4);
	g_ConfigManager.SetValue_Color("ESP", "DefuseColor", g_Options.color_esp_defuse);
	g_ConfigManager.SetValue_Color("ESP", "NotifyColor", g_Options.color_esp_notify);


	//sesp
	g_ConfigManager.SetValue_Bool("SoundESP", "Enabled", g_Options.sesp_enabled);
	g_ConfigManager.SetValue_Bool("SoundESP", "NotifySpotted", g_Options.sesp_notify_spotted);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "NoFlashAlpha", g_Options.sesp_notify_spotted_pause);
	g_ConfigManager.SetValue_Bool("SoundESP", "EnemyInCrosshair", g_Options.sesp_whaim_esp);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyInCrosshairFreq", g_Options.sesp_whaim_freq);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyInCrosshairDur", g_Options.sesp_whaim_dur);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyInCrosshairPeriod", g_Options.sesp_whaim_period);
	g_ConfigManager.SetValue_Bool("SoundESP", "EnemyIsNear", g_Options.sesp_distance_esp);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyIsNearFreq", g_Options.sesp_distance_freq);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyIsNearDur", g_Options.sesp_distance_dur);
	g_ConfigManager.SetValue_Num<int>("SoundESP", "EnemyIsNearRadius", g_Options.sesp_distance_radius);

	//AimBot
	g_ConfigManager.SetValue_Num<int>("AimBot", "Mode", g_Options.ab_mode);
	g_ConfigManager.SetValue_Num<int>("AimBot", "Method", g_Options.ab_method);
	g_ConfigManager.SetValue_Num<DWORD>("AimBot", "Hotkey", g_Options.ab_hotkey);
	g_ConfigManager.SetValue_Bool("AimBot", "RandomizeCurve", g_Options.ab_randomize_curve);
	g_ConfigManager.SetValue_Bool("AimBot", "DisplayInfo", g_Options.ab_display_info);
	g_ConfigManager.SetValue_Bool("AimBot", "OnlyWhenScoped", g_Options.ab_aim_only_when_scoped);
	g_ConfigManager.SetValue_Bool("AimBot", "RandomHitbox", g_Options.ab_random_hitbox);
	g_ConfigManager.SetValue_Bool("AimBot", "HuntPlayer", g_Options.ab_hunt_player);
	g_ConfigManager.SetValue_Bool("AimBot", "DisableMouse", g_Options.ab_disable_mouse);
	g_ConfigManager.SetValue_Num<float>("AimBot", "MinReactionDist", g_Options.ab_min_reaction_distance);
	g_ConfigManager.SetValue_Num<float>("AimBot", "MaxReactionDist", g_Options.ab_max_reaction_distance);
	g_ConfigManager.SetValue_Num<float>("AimBot", "Velocity", g_Options.ab_velocity);
	g_ConfigManager.SetValue_Num<float>("AimBot", "VelocityReduction", g_Options.ab_velocity_reduction);
	g_ConfigManager.SetValue_Num<float>("AimBot", "Max2DDistanceToCrosshair", g_Options.ab_max_2Ddistance_to_crosshair);
	g_ConfigManager.SetValue_Color("AimBot", "InfoColor", g_Options.ab_info_color);
	g_ConfigManager.SetValue_Num<float>("AimBot", "Acceleration", static_cast<float>(g_Options.ab_acceleration));

	//Crosshair
	g_ConfigManager.SetValue_Bool("Crosshair", "DrawCustom", g_bCrosshair);
	g_ConfigManager.SetValue_Bool("Crosshair", "AlignToSpread", g_bCrosshairSpread);
	g_ConfigManager.SetValue_Bool("Crosshair", "HideDefault", g_bHideDefault);

	g_ConfigManager.SetValue_Num<int>("Crosshair", "Type", static_cast<int>(g_iCrosshairType));
	g_ConfigManager.SetValue_Bool("Crosshair", "WithDot", g_ExtraDot);

	g_ConfigManager.SetValue_Num<int>("Crosshair", "Scale", static_cast<int>(g_iCrosshairSize));
	g_ConfigManager.SetValue_Num<int>("Crosshair", "DotScale", static_cast<int>(g_iDotScale));
	g_ConfigManager.SetValue_Num<int>("Crosshair", "Thickness", static_cast<int>(g_iCrosshairThickness));

	Color tmpCol; tmpCol.SetColor(g_cCrosshair[0], g_cCrosshair[1], g_cCrosshair[2], g_cCrosshair[3]);
	g_ConfigManager.SetValue_Color("Crosshair", "Color", tmpCol);

	//Other
	g_ConfigManager.SetValue_Bool("Other", "RadarHack", g_RH);
	g_ConfigManager.SetValue_Bool("Other", "RevealRanks", g_RevealRanks);
	g_ConfigManager.SetValue_Bool("Other", "FakeLag", g_FakeLag);
	g_ConfigManager.SetValue_Bool("Other", "FakeLagRandomize", g_FakeLagRandomize); 
	g_ConfigManager.SetValue_Bool("Other", "FakeLagStopByTriggerKey", g_FakeLagUseKey);
	g_ConfigManager.SetValue_Num<int>("Other", "FakeLagPeriod", static_cast<int>(g_FakeLagPeriod));
	g_ConfigManager.SetValue_Num<int>("Other", "FakeLagDuration", static_cast<int>(g_FakeLagDuration));
	g_ConfigManager.SetValue_Bool("Other", "NoFlash", g_NF);
	g_ConfigManager.SetValue_Num<float>("Other", "NoFlashAlpha", static_cast<float>(g_FlashAlpha));
	g_ConfigManager.SetValue_Num<DWORD>("Other", "AlertKey", static_cast<DWORD>(g_dwAlertKey));

	//...save
	g_ConfigManager.Save();
}


inline LONG VKToLParam(DWORD vkCode);
void __fastcall VCLtoRGB(Color &col, int rgb[3])
{
	rgb[0] = col.r();
	rgb[1] = col.g();
	rgb[2] = col.b();
}

ImGuiAl::MsgBox msgbox_save, msgbox_reload, msgbox_reset;

inline void DrawMenuContent(D3D9DevPtr lpD3D9Dev)
{	
	static const char* buttons[] = { "Yes", "No", NULL };
	msgbox_save.Init("Save config?", 0, "Are you really sure you want to save current config to the file?", buttons);
	msgbox_reset.Init("Reset all settings?", 0, "This action will reset all current settings and config file to default values. Are you shure you really want to loose all config data and reset your hack settings?", buttons);
	msgbox_reload.Init("Reload config?", 0, "Reload config settings from config.cfg file?", buttons);

	/*static bool bChamsEnabled;
	static bool bChamsSepInvisCol;
	static bool bHeadDifCol;
	static bool	bOnlyHead;
	static int cGen[3];
	static int cInv[3];
	static int cHead[3];
	static int cInvHead[3];*/
	static bool bChamsPEnabled;
	static bool bChamsPAlly;
	static bool bChamsPIgnoreZ;
	static bool bChamsPGlass;
	static bool bChamsPFlat;
	static bool bChamsPWF;
	static bool bChamsAEnabled;
	static bool bChamsAFlat;
	static bool bChamsAGlass;
	static bool bChamsAIgnoreZ;
	static bool bChamsAWF;
	static int  cChamsAllyVis[3];
	static int  cChamsAllyOcc[3];
	static int  cChamsEnemyVis[3];
	static int  cChamsEnemyOcc[3];
	static int  cChamsArmsVis[3];
	static int  cChamsArmsOcc[3];
	static int  iChamsAlphaP;
	static int  iChamsAlphaA;

	static bool bGlwEnabled;
	static bool bGlowAlly;
	static bool bGlowC4Carr;
	static bool bGlowPlantedBomb;
	static bool bGlowWeapons;
	static int  cGlowEnemy[3];
	static int  cGlowAlly[3];
	static int  cGlowC4[3];
	static int  cGlowC4Carr[3];
	static int  cGlowWeap[3];
	static int  iGlAlpha;

	static int iTriggerMode;
	static int iTriggerPostLat;
	static int iTriggerMethod;
	static bool bTriggerIgnoreWalls;
	static bool bTriggerHeadsOnly;
	static bool bTriggerHoldFire;

	static bool  bNoFlash;
	static float fFlashAlpha;
	static bool  bRadarHack;

	static bool bCrosshair;
	static int  iCrosshairSize;
	static int  cCrosshair[4];
	static bool bCrosshairSpread;
	static int  iCrosshairType;
	static int  iCrosshairThickness;
	static bool bHideDefault;
	static bool bExtraDot;
	static int  iDotScale;

	static bool bESP_Enabled;
	static bool bESP_DefuseKits;
	static bool bESP_DroppedWeapons;
	static bool bESP_Allies;
	static bool bESP_PlantedBomb;
	static bool bESP_PlayerArmor;
	static bool bESP_PlayerBoxes;
	static bool bESP_PlayerBoxesHO;
	static bool bESP_PlayerDistance;
	static bool bESP_PlayerHealth;
	static bool bESP_PlayerNames;
	static bool bESP_PlayerSnaplines;
	static bool bESP_PlayerWeapons;
	static bool bESP_GrenPred;
	static bool bESP_Bones;
	static bool bESP_BonesHP;
	static int  iESP_DefuseStatusType;
	static bool bESP_DefuseStatusEnabled;
	static bool bESP_NotifySpotted;
	static int  iESP_NotifyAlpha;
	static bool bESP_ZoomEnabled;
	static float fESP_ZoomFOV;
	static DWORD dwESP_ZoomKey;
	static float fESP_ViewmodelFOV;
	static bool bESP_ZoomInstant;
	static bool bESP_ZoomDS;
	static bool bESP_HideVM;
	static bool bESP_HideVMWhenZoom;

	static bool bSESP_Enabled;
	static bool bSESP_NotifySpotted;
	static int  iSESP_NotifyPause;
	static bool bSESP_WHAim;
	static char isSESP_WHAimFreq[256];
	static char isSESP_WHAimDur[256];
	static int  iSESP_WHAimPeriod;
	static bool bSESP_DistanceEsp;
	static char isSESP_DistanceEspFreq[256];
	static char isSESP_DistanceEspDur[256];
	static int  iSESP_DistanceEspRadius;
	
	static int cESPAlly[3];
	static int cESPAllyOcc[3];
	static int cESPEnemy[3];
	static int cESPEnemyOcc[3];
	static int cESPGrenade[3];
	static int cESPC4[3];
	static int cESPDefs[3];
	static int cESPNotify[3];

	static int iESPFontSize;

	static int iAB_Mode;
	static int iAB_Method;
	static DWORD dwAB_Key;
	static bool bAB_RandomizeCurve;
	static bool bAB_DisplayInfo;
	static bool bAB_OnlyScoped;
	static bool bAB_RandomHitbox;
	static bool bAB_HuntPlayer;
	static char xAB_PlayerName[256];
	static bool bAB_DisableMouse;
	static float fAB_MinReact;
	static float fAB_MaxReact;
	static float fAB_MaxCross;
	static float fAB_Velocity;
	static float fAB_VelocityRed;
	static float fAB_Accel;
	static int cAB_InfoColor[3];

	static bool bRevealRanks;
	static bool bFakeLag;
	static bool bFakeLagRandomize;
	static bool bFakeLagUseKey;

	static int iFakeLagPeriod;
	static int iFakeLagDuration;

	static DWORD dwAlertKey;


	/*WaitForSingleObject(hWHMutex, INFINITE);
	bChamsEnabled = static_cast<bool>(g_ChamsEnabled);
	bChamsSepInvisCol = static_cast<bool>(g_ChamsSepInvisCol);
	bOnlyHead = static_cast<bool>(g_bOnlyHead);
	bHeadDifCol = static_cast<bool>(g_bHeadDifCol);
	cGen[0] = g_cGen[0];
	cGen[1] = g_cGen[1];
	cGen[2] = g_cGen[2];
	cInv[0] = g_cInv[0];
	cInv[1] = g_cInv[1];
	cInv[2] = g_cInv[2];
	cHead[0] = g_cHead[0];
	cHead[1] = g_cHead[1];
	cHead[2] = g_cHead[2];
	cInvHead[0] = g_cInvHead[0];
	cInvHead[1] = g_cInvHead[1];
	cInvHead[2] = g_cInvHead[2];
	ReleaseMutex(hWHMutex);*/

	bChamsPEnabled = static_cast<bool>(g_Options.chams_player_enabled);
	bChamsPAlly = static_cast<bool>(!g_Options.chams_player_enemies_only);
	bChamsPIgnoreZ = static_cast<bool>(g_Options.chams_player_ignorez);
	bChamsPGlass = static_cast<bool>(g_Options.chams_player_glass);
	bChamsPFlat = static_cast<bool>(g_Options.chams_player_flat);
	bChamsPWF = static_cast<bool>(g_Options.chams_player_wireframe);
	bChamsAEnabled = static_cast<bool>(g_Options.chams_arms_enabled);
	bChamsAFlat = static_cast<bool>(g_Options.chams_arms_flat);
	bChamsAGlass = static_cast<bool>(g_Options.chams_arms_glass);
	bChamsAIgnoreZ = static_cast<bool>(g_Options.chams_arms_ignorez);
	bChamsAWF = static_cast<bool>(g_Options.chams_arms_wireframe);
	VCLtoRGB(g_Options.color_chams_player_ally_visible, cChamsAllyVis);
	VCLtoRGB(g_Options.color_chams_player_ally_occluded, cChamsAllyOcc);
	VCLtoRGB(g_Options.color_chams_player_enemy_visible, cChamsEnemyVis);
	VCLtoRGB(g_Options.color_chams_player_enemy_occluded, cChamsEnemyOcc);
	VCLtoRGB(g_Options.color_chams_arms_visible, cChamsArmsVis);
	VCLtoRGB(g_Options.color_chams_arms_occluded, cChamsArmsOcc);
	iChamsAlphaP = g_Options.chams_alpha_p;
	iChamsAlphaA = g_Options.chams_alpha_a;

	bGlwEnabled = static_cast<bool>(g_Options.glow_players);
	bGlowAlly = static_cast<bool>(!g_Options.glow_enemies_only);
	bGlowC4Carr = static_cast<bool>(g_Options.glow_c4_carrier);
	bGlowPlantedBomb = static_cast<bool>(g_Options.glow_planted_c4);
	bGlowWeapons = static_cast<bool>(g_Options.glow_weapons);
	//bGlowDefuses = static_cast<bool>(g_Options.glow_defuse_kits);
	VCLtoRGB(g_Options.color_glow_enemy, cGlowEnemy);
	VCLtoRGB(g_Options.color_glow_ally, cGlowAlly);
	VCLtoRGB(g_Options.color_glow_planted_c4, cGlowC4);
	VCLtoRGB(g_Options.color_glow_c4_carrier, cGlowC4Carr);
	VCLtoRGB(g_Options.color_glow_weapons, cGlowWeap);
	//VCLtoRGB(g_Options.color_glow_defuse, cGlowDefs);
	iGlAlpha = g_cGlowAlpha;

	iTriggerMode = static_cast<int>(g_TBMode);
	iTriggerPostLat = static_cast<int>(g_TPostDelay);
	iTriggerMethod = static_cast<int>(g_TriggerBotMethod);
	bTriggerIgnoreWalls = static_cast<bool>(g_TBIgnoreWalls);
	bTriggerHeadsOnly = static_cast<bool>(g_TriggerHeadsOnly);
	bTriggerHoldFire = static_cast<bool>(g_TriggerHoldFire);
	
	char lpBuf[32];

	if (!g_bKeyEditMode)
	{
		if (g_TriggerHotkey == VK_RBUTTON)
			g_TrigKeyText = L"RMB";
		else if (g_TriggerHotkey == VK_MBUTTON)
			g_TrigKeyText = L"MMB";
		else
		{
			GetKeyNameTextA(VKToLParam(g_TriggerHotkey), lpBuf, 32);
			g_TrigKeyText.Format(L"%zs.a", lpBuf);
		}
	}
	else
		g_TrigKeyText = L"[Press any key...]";

	WaitForSingleObject(hFlashMutex, INFINITE);
	bNoFlash = static_cast<bool>(g_NF);
	fFlashAlpha = g_FlashAlpha;
	ReleaseMutex(hFlashMutex);

	WaitForSingleObject(hRadarMutex, INFINITE);
	bRadarHack = static_cast<bool>(g_RH);
	ReleaseMutex(hRadarMutex);

	WaitForSingleObject(hCrossMutex, INFINITE);
	bCrosshair = static_cast<bool>(g_bCrosshair);
	iCrosshairSize = g_iCrosshairSize;
	cCrosshair[0] = g_cCrosshair[0];
	cCrosshair[1] = g_cCrosshair[1];
	cCrosshair[2] = g_cCrosshair[2];
	cCrosshair[3] = g_cCrosshair[3];
	bCrosshairSpread = static_cast<bool>(g_bCrosshairSpread);
	iCrosshairType = g_iCrosshairType;
	iCrosshairThickness = g_iCrosshairThickness;
	bHideDefault = g_bHideDefault;
	bExtraDot = g_ExtraDot;
	iDotScale = g_iDotScale;
	ReleaseMutex(hCrossMutex);

	bESP_Enabled = static_cast<bool>(g_Options.esp_enabled);
	bESP_DefuseKits = static_cast<bool>(g_Options.esp_defuse_kit);
	bESP_DroppedWeapons = static_cast<bool>(g_Options.esp_dropped_weapons);
	bESP_Allies = static_cast<bool>(!g_Options.esp_enemies_only);
	bESP_PlantedBomb = static_cast<bool>(g_Options.esp_planted_c4);
	bESP_PlayerArmor = static_cast<bool>(g_Options.esp_player_armour);
	bESP_PlayerBoxes = static_cast<bool>(g_Options.esp_player_boxes);
	bESP_PlayerBoxesHO = static_cast<bool>(g_Options.esp_player_boxes_head_only);
	bESP_PlayerDistance = static_cast<bool>(g_Options.esp_player_distance);
	bESP_PlayerHealth = static_cast<bool>(g_Options.esp_player_health);
	bESP_PlayerNames = static_cast<bool>(g_Options.esp_player_names);
	bESP_PlayerSnaplines = static_cast<bool>(g_Options.esp_player_snaplines);
	bESP_PlayerWeapons = static_cast<bool>(g_Options.esp_player_weapons);
	bESP_GrenPred = static_cast<bool>(g_Options.esp_GrenadePrediction);
	bESP_Bones = static_cast<bool>(g_Options.esp_player_bones);
	bESP_BonesHP = static_cast<bool>(g_Options.esp_bones_hp_colored);
	iESP_DefuseStatusType = static_cast<int>(g_Options.esp_player_defuse_status_type);
	bESP_DefuseStatusEnabled = static_cast<bool>(g_Options.esp_player_defuse_status);
	bESP_NotifySpotted = static_cast<bool>(g_Options.esp_notify_spotted);
	iESP_NotifyAlpha = static_cast<int>(g_Options.esp_notify_spotted_alpha);
	bESP_ZoomEnabled = static_cast<bool>(g_Options.esp_zoom_enabled);
	fESP_ZoomFOV = static_cast<float>(g_Options.esp_zoom_fov);
	dwESP_ZoomKey = static_cast<DWORD>(g_Options.esp_zoom_key);
	fESP_ViewmodelFOV = static_cast<float>(g_Options.esp_zoom_hands_fov);
	bESP_ZoomInstant = static_cast<bool>(g_Options.esp_zoom_instant);
	bESP_ZoomDS = static_cast<bool>(g_Options.esp_zoom_dont_when_scope);
	bESP_HideVM = static_cast<bool>(g_Options.esp_hide_viewmodel);
	bESP_HideVMWhenZoom = static_cast<bool>(g_Options.esp_hide_viewmodel_when_zoom);


	if (!g_Options.esp_zoom_key_edit_mode)
	{
		if (g_Options.esp_zoom_key == VK_RBUTTON)
			g_Options.esp_zoom_key_text = "RMB";
		else if(g_Options.esp_zoom_key == VK_MBUTTON)
			g_Options.esp_zoom_key_text = "MMB";
		else
		{
			GetKeyNameTextA(VKToLParam(g_Options.esp_zoom_key), lpBuf, 32);
			g_Options.esp_zoom_key_text.Format("%zs.a", lpBuf);
		}
	}
	else
		g_Options.esp_zoom_key_text = "[Press any key...]";


	bSESP_Enabled = static_cast<bool>(g_Options.sesp_enabled);
	bSESP_NotifySpotted = static_cast<bool>(g_Options.sesp_notify_spotted);
	iSESP_NotifyPause = static_cast<int>(g_Options.sesp_notify_spotted_pause);
	bSESP_WHAim = static_cast<bool>(g_Options.sesp_whaim_esp);
	iSESP_WHAimPeriod = static_cast<int>(g_Options.sesp_whaim_period);
	wsprintfA(isSESP_WHAimFreq, "%i", g_Options.sesp_whaim_freq);
	wsprintfA(isSESP_WHAimDur, "%i", g_Options.sesp_whaim_dur);
	bSESP_DistanceEsp = static_cast<bool>(g_Options.sesp_distance_esp);
	iSESP_DistanceEspRadius = static_cast<int>(g_Options.sesp_distance_radius);
	wsprintfA(isSESP_DistanceEspDur, "%i", g_Options.sesp_distance_dur);
	wsprintfA(isSESP_DistanceEspFreq, "%i", g_Options.sesp_distance_freq);
	
	VCLtoRGB(g_Options.color_esp_ally_visible, cESPAlly);
	VCLtoRGB(g_Options.color_esp_ally_occluded, cESPAllyOcc);
	VCLtoRGB(g_Options.color_esp_enemy_visible, cESPEnemy);
	VCLtoRGB(g_Options.color_esp_enemy_occluded, cESPEnemyOcc);
	VCLtoRGB(g_Options.color_esp_grenade, cESPGrenade);
	VCLtoRGB(g_Options.color_esp_c4, cESPC4);
	VCLtoRGB(g_Options.color_esp_defuse, cESPDefs); 
	VCLtoRGB(g_Options.color_esp_notify, cESPNotify);

	iESPFontSize = static_cast<int>(g_Options.esp_font_size);

	iAB_Mode = static_cast<int>(g_Options.ab_mode);
	iAB_Method = static_cast<int>(g_Options.ab_method);
	dwAB_Key = static_cast<DWORD>(g_Options.ab_hotkey);
	bAB_RandomizeCurve = static_cast<bool>(g_Options.ab_randomize_curve);
	bAB_DisplayInfo = static_cast<bool>(g_Options.ab_display_info);
	bAB_OnlyScoped = static_cast<bool>(g_Options.ab_aim_only_when_scoped);
	bAB_RandomHitbox = static_cast<bool>(g_Options.ab_random_hitbox);
	bAB_HuntPlayer = static_cast<bool>(g_Options.ab_hunt_player);
	strcpy_s(xAB_PlayerName, 256, g_Options.ab_filter_name.GetStr().c_str());
	bAB_DisableMouse = static_cast<bool>(g_Options.ab_disable_mouse);
	fAB_MinReact = static_cast<float>(g_Options.ab_min_reaction_distance);
	fAB_MaxReact = static_cast<float>(g_Options.ab_max_reaction_distance);
	fAB_MaxCross = static_cast<float>(g_Options.ab_max_2Ddistance_to_crosshair);
	fAB_Velocity = static_cast<float>(g_Options.ab_velocity);
	fAB_VelocityRed = static_cast<float>(g_Options.ab_velocity_reduction);
	fAB_Accel = static_cast<float>(g_Options.ab_acceleration);
	VCLtoRGB(g_Options.ab_info_color, cAB_InfoColor);

	if (!g_Options.ab_key_edit_mode)
	{
		if (g_Options.ab_hotkey == VK_RBUTTON)
			g_Options.ab_key_text = "RMB";
		else if (g_Options.ab_hotkey == VK_MBUTTON)
			g_Options.ab_key_text = "MMB";
		else
		{
			GetKeyNameTextA(VKToLParam(g_Options.ab_hotkey), lpBuf, 32);
			g_Options.ab_key_text.Format("%zs.a", lpBuf);
		}
	}
	else
		g_Options.ab_key_text = "[Press any key...]";

	bRevealRanks = static_cast<bool>(g_RevealRanks);
	bFakeLag = static_cast<bool>(g_FakeLag);
	bFakeLagRandomize = static_cast<bool>(g_FakeLagRandomize);
	bFakeLagUseKey = static_cast<bool>(g_FakeLagUseKey);

	WaitForSingleObject(hFakeLagMutex, INFINITE);
	iFakeLagPeriod = static_cast<int>(g_FakeLagPeriod);
	iFakeLagDuration = static_cast<int>(g_FakeLagDuration);
	ReleaseMutex(hFakeLagMutex);

	WaitForSingleObject(g_hAlertKeyMutex, INFINITE);
	dwAlertKey = static_cast<DWORD>(g_dwAlertKey);

	if (!g_bAlertKeyEditMode)
	{
		if (g_dwAlertKey == VK_RBUTTON)
			g_sAlertKeyText = "RMB";
		else if (g_dwAlertKey == VK_MBUTTON)
			g_sAlertKeyText = "MMB";
		else
		{
			GetKeyNameTextA(VKToLParam(g_dwAlertKey), lpBuf, 32);
			g_sAlertKeyText.Format("%zs.a", lpBuf);
		}
	}
	else
		g_sAlertKeyText = "[Press any key...]";

	ReleaseMutex(g_hAlertKeyMutex);



	static char conCmd[2048] = "";
	std::string flt_per, flt_dur;

	static int tab = 0;
	static int esptab = 0;
	const static int tabsn = 7;
	static char** tabs;
	tabs = new char*[tabsn];

	tabs[0] = "Chams";
	tabs[1] = "Glow";
	tabs[2] = "TriggerBot";
	tabs[3] = "AimBot";
	tabs[4] = "Crosshair";
	tabs[5] = "ESP";
	tabs[6] = "Other";

	
	ImGui::MyTabLabels(tabsn, const_cast<const char**>(tabs), tab);
	delete[] tabs;

	ImGui::BeginChild("Content", ImVec2(MENU_W - 19, MENU_H - 85), true);
	switch (tab)
	{
		case 0:	//Chams
			/*ImGui::TextColored(ImVec4(1.0f, 0, 0, 1.0f), "Works only on official maps.");
			ImGui::Text("Options:");
			ImGui::Checkbox("Enabled", &bChamsEnabled);
			ImGui::SameLine(175);
			ImGui::Checkbox("Separate color for invisible", &bChamsSepInvisCol);
			ImGui::SameLine(425);
			ImGui::Checkbox("Only head", &bOnlyHead);
			ImGui::Checkbox("Draw heads with a different color", &bHeadDifCol);
			ImGui::Text("Colors:");
			ImGui::ColorEdit3INT("Generic", cGen);
			ImGui::ColorEdit3INT("Occluded", cInv);
			ImGui::ColorEdit3INT("Generic head", cHead);
			ImGui::ColorEdit3INT("Occluded head", cInvHead);*/

			ImGui::Text("Options:");
			if (ImGui::Checkbox("Players Enabled", &bChamsPEnabled, "Apply chams to players.\nStains players with selected color and applies selected effects."))
				g_Options.chams_player_enabled = bChamsPEnabled;
			ImGui::SameLine(175);
			if(ImGui::Checkbox("Color Allies", &bChamsPAlly, "Apply to an allies too."))
				g_Options.chams_player_enemies_only = !bChamsPAlly;
			ImGui::SameLine(425);
			if(ImGui::Checkbox("Players Ignore Z", &bChamsPIgnoreZ, "Actualy a WallHack mode."))
				g_Options.chams_player_ignorez = bChamsPIgnoreZ;

			if (ImGui::Checkbox("Players Flat", &bChamsPFlat, "Make players look flat."))
				g_Options.chams_player_flat = bChamsPFlat;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Players Glass", &bChamsPGlass, "Players will become semi-transparent.\nWill override alpha value."))
				g_Options.chams_player_glass = bChamsPGlass;
			ImGui::SameLine(425);
			if (ImGui::Checkbox("Players Wireframe", &bChamsPWF, "Draw players as colored vertex grid."))
				g_Options.chams_player_wireframe = bChamsPWF;

			if (ImGui::Checkbox("Arms Enabled", &bChamsAEnabled, "Apply chams to first person view hands."))
				g_Options.chams_arms_enabled = bChamsAEnabled;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Arms Ignore Z", &bChamsAIgnoreZ))
				g_Options.chams_arms_ignorez = bChamsAIgnoreZ;
			ImGui::SameLine(425);
			if (ImGui::Checkbox("Arms Flat", &bChamsAFlat))
				g_Options.chams_arms_flat = bChamsAFlat;

			if (ImGui::Checkbox("Arms Glass", &bChamsAGlass))
				g_Options.chams_arms_glass = bChamsAGlass;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Arms Wireframe", &bChamsAWF))
				g_Options.chams_arms_wireframe = bChamsAWF;

			ImGui::Text("Colors:");

			if (ImGui::ColorEdit3INT("Enemies visible", cChamsEnemyVis))
			{
				g_Options.color_chams_player_enemy_visible.SetColor(cChamsEnemyVis[0], cChamsEnemyVis[1], cChamsEnemyVis[2], iChamsAlphaP);
			}

			if (ImGui::ColorEdit3INT("Enemies occluded", cChamsEnemyOcc))
			{
				g_Options.color_chams_player_enemy_occluded.SetColor(cChamsEnemyOcc[0], cChamsEnemyOcc[1], cChamsEnemyOcc[2], iChamsAlphaP);
			}

			if (ImGui::ColorEdit3INT("Allies visible", cChamsAllyVis))
			{
				g_Options.color_chams_player_ally_visible.SetColor(cChamsAllyVis[0], cChamsAllyVis[1], cChamsAllyVis[2], iChamsAlphaP);
			}

			if (ImGui::ColorEdit3INT("Allies occluded", cChamsAllyOcc))
			{
				g_Options.color_chams_player_ally_occluded.SetColor(cChamsAllyOcc[0], cChamsAllyOcc[1], cChamsAllyOcc[2], iChamsAlphaP);
			}

			if (ImGui::ColorEdit3INT("Arms visible", cChamsArmsVis))
			{
				g_Options.color_chams_arms_visible.SetColor(cChamsArmsVis[0], cChamsArmsVis[1], cChamsArmsVis[2], iChamsAlphaA);
			}

			if (ImGui::ColorEdit3INT("Arms occluded", cChamsArmsOcc))
			{
				g_Options.color_chams_arms_occluded.SetColor(cChamsArmsOcc[0], cChamsArmsOcc[1], cChamsArmsOcc[2], iChamsAlphaA);
			}

			if (ImGui::SliderInt("Players Alpha", &iChamsAlphaP, 0, 255))
			{
				g_Options.color_chams_player_enemy_visible.SetColor(cChamsEnemyVis[0], cChamsEnemyVis[1], cChamsEnemyVis[2], iChamsAlphaP);
				g_Options.color_chams_player_enemy_occluded.SetColor(cChamsEnemyOcc[0], cChamsEnemyOcc[1], cChamsEnemyOcc[2], iChamsAlphaP);
				g_Options.color_chams_player_ally_visible.SetColor(cChamsAllyVis[0], cChamsAllyVis[1], cChamsAllyVis[2], iChamsAlphaP);
				g_Options.color_chams_player_ally_occluded.SetColor(cChamsAllyOcc[0], cChamsAllyOcc[1], cChamsAllyOcc[2], iChamsAlphaP);
				g_Options.chams_alpha_p = iChamsAlphaP;
			}

			if (ImGui::SliderInt("Arms Alpha", &iChamsAlphaA, 0, 255))
			{
				g_Options.color_chams_arms_visible.SetColor(cChamsArmsVis[0], cChamsArmsVis[1], cChamsArmsVis[2], iChamsAlphaA);
				g_Options.color_chams_arms_occluded.SetColor(cChamsArmsOcc[0], cChamsArmsOcc[1], cChamsArmsOcc[2], iChamsAlphaA);
				g_Options.chams_alpha_a = iChamsAlphaA;
			}

			break;
		case 1:	//Glow
			ImGui::Text("Options:");
			if(ImGui::Checkbox("Glow Players", &bGlwEnabled, "Apply glow effect to players.\nThis is another implementation of WallHack."))
				g_Options.glow_players = bGlwEnabled;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Glow Allies", &bGlowAlly))
				g_Options.glow_enemies_only = !bGlowAlly;
			ImGui::SameLine(375);
			if (ImGui::Checkbox("Glow C4 Carrier", &bGlowC4Carr))
				g_Options.glow_c4_carrier = bGlowC4Carr;
			if (ImGui::Checkbox("Glow Planted C4", &bGlowPlantedBomb))
				g_Options.glow_planted_c4 = bGlowPlantedBomb;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Glow Weapons", &bGlowWeapons))
				g_Options.glow_weapons = bGlowWeapons;
			/*ImGui::SameLine(375);
			if (ImGui::Checkbox("Glow Defuse Kits", &bGlowDefuses))
				g_Options.glow_defuse_kits = bGlowDefuses;*/
			ImGui::Text("Colors:");
			if (ImGui::ColorEdit3INT("Enemy", cGlowEnemy))
				g_Options.color_glow_enemy.SetColor(cGlowEnemy[0], cGlowEnemy[1], cGlowEnemy[2], iGlAlpha);
			if (ImGui::ColorEdit3INT("Ally", cGlowAlly))
				g_Options.color_glow_ally.SetColor(cGlowAlly[0], cGlowAlly[1], cGlowAlly[2], iGlAlpha);
			if (ImGui::ColorEdit3INT("Bomb carrier", cGlowC4Carr))
				g_Options.color_glow_c4_carrier.SetColor(cGlowC4Carr[0], cGlowC4Carr[1], cGlowC4Carr[2], iGlAlpha);
			if (ImGui::ColorEdit3INT("Planted bomb", cGlowC4))
			{
				g_Options.color_glow_planted_c4.SetColor(cGlowC4[0], cGlowC4[1], cGlowC4[2], iGlAlpha);
				g_Options.color_esp_c4.SetColor(cGlowC4[0], cGlowC4[1], cGlowC4[2], 255);
			}
			if (ImGui::ColorEdit3INT("Weapons", cGlowWeap))
			{
				g_Options.color_glow_weapons.SetColor(cGlowWeap[0], cGlowWeap[1], cGlowWeap[2], iGlAlpha);
				g_Options.color_esp_weapons.SetColor(cGlowWeap[0], cGlowWeap[1], cGlowWeap[2], 255);
			}
			/*if (ImGui::ColorEdit3INT("Defuse kits", cGlowDefs))
			{
				g_Options.color_glow_defuse.SetColor(cGlowDefs[0], cGlowDefs[1], cGlowDefs[2], iGlAlpha);
				g_Options.color_esp_defuse.SetColor(cGlowDefs[0], cGlowDefs[1], cGlowDefs[2], 255);
			}*/
			if (ImGui::SliderInt("Alpha", &iGlAlpha, 0, 255))
			{
				g_Options.color_glow_enemy.SetColor(cGlowEnemy[0], cGlowEnemy[1], cGlowEnemy[2], iGlAlpha);
				g_Options.color_glow_ally.SetColor(cGlowAlly[0], cGlowAlly[1], cGlowAlly[2], iGlAlpha);
				g_Options.color_glow_planted_c4.SetColor(cGlowC4[0], cGlowC4[1], cGlowC4[2], iGlAlpha);
				g_Options.color_glow_c4_carrier.SetColor(cGlowC4Carr[0], cGlowC4Carr[1], cGlowC4Carr[2], iGlAlpha);
				g_Options.color_glow_weapons.SetColor(cGlowWeap[0], cGlowWeap[1], cGlowWeap[2], iGlAlpha);
				//g_Options.color_glow_defuse.SetColor(cGlowDefs[0], cGlowDefs[1], cGlowDefs[2], iGlAlpha);
				g_cGlowAlpha = iGlAlpha;
			}
			break;
		case 2: //TriggerBot
			ImGui::Text("Method:");
			if (ImGui::RadioButtonEx("Ray Tracing", &iTriggerMethod, 0, "Advantages:\n+Works on any distances.\n+Can ignore walls.\n+Can react on heads only.\n+Not affected by smoke.\n\nDisadvantages:\n-Little bit slower."))
			{
				g_TriggerBotMethod = iTriggerMethod;
			}
			ImGui::SameLine(175);
			if (ImGui::RadioButtonEx("Checking enemy in crosshair", &iTriggerMethod, 1, "Advantages:\n+Little bit faster.\n\nDisadvantages:\n-Limited working distance.\n-Sometimes does not trigger.\n-Blocked by smoke.\n-Cannot ignore walls.\n-HeadOnly mode is not possible."))
			{
				g_TriggerBotMethod = iTriggerMethod;
			}
			ImGui::Text("Options:");
			if (ImGui::Checkbox("Hold fire", &bTriggerHoldFire, "When you are aiming an enemy your fire will be toggle holded.\nIf disabled you will fire single shots."))
				g_TriggerHoldFire = bTriggerHoldFire;
			if (iTriggerMethod == 0)
			{
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Ignore walls", &bTriggerIgnoreWalls, "Enable this if you want to ignore any obstacles."))
					g_TBIgnoreWalls = bTriggerIgnoreWalls;
				ImGui::SameLine(425);
				if (ImGui::Checkbox("Heads only", &bTriggerHeadsOnly, "Will trigger only when aiming head."))
					g_TriggerHeadsOnly = bTriggerHeadsOnly;
			}
			ImGui::Text("Mode:");
			if (ImGui::RadioButton("Disabled", &iTriggerMode, 0))
			{
				g_TBMode = iTriggerMode;
			}
			ImGui::SameLine(175);
			if (ImGui::RadioButtonEx("Managed", &iTriggerMode, 1, "Triggers only when you are holding trigger key."))
			{
				g_TBMode = iTriggerMode;
			}
			ImGui::SameLine(425);
			if (ImGui::RadioButtonEx("Automatic", &iTriggerMode, 2, "Trigger always."))
			{
				g_TBMode = iTriggerMode;
			}
			ImGui::Text("");
			ImGui::Text("Options:");
			if (ImGui::SliderInt("Post-fire latency (ms)", &iTriggerPostLat, 0, 1000))
			{
				g_TPostDelay = iTriggerPostLat;
			}
			ImGui::NewLine();
			ImGui::Text("Managed-mode key: ");
			ImGui::SameLine(140);
			if (ImGui::Button(g_TrigKeyText.GetStr_a().c_str(), ImVec2(135, 25)))
			{
				g_bKeyEditMode = !g_bKeyEditMode;
				g_Options.esp_zoom_key_edit_mode = false;
				g_Options.ab_key_edit_mode = false;
				g_bAlertKeyEditMode = false;
			}
			break;
		case 3: //AimBot
			ImGui::Text("Mode:");
			if (ImGui::RadioButton("Disabled", &iAB_Mode, CAimbot::MODE_DISABLED))
			{
				g_Options.ab_mode = iAB_Mode;
			}
			ImGui::SameLine(175);
			if (ImGui::RadioButtonEx("One-shot", &iAB_Mode, CAimbot::MODE_ONESHOT, "Press aim key to turn on. Will search for target and aim\nuntill next shot.\nAfter shot will be set into idle state."))
			{
				g_Options.ab_mode = iAB_Mode;
			}
			ImGui::SameLine(375);
			if (ImGui::RadioButtonEx("Managed", &iAB_Mode, CAimbot::MODE_MANAGED, "Aiming only when aim key is held."))
			{
				g_Options.ab_mode = iAB_Mode;
			}
			if (ImGui::RadioButtonEx("Toggle", &iAB_Mode, CAimbot::MODE_TOGGLE, "Aiming always.\nPress aim key to switch aiming/idle states."))
			{
				g_Options.ab_mode = iAB_Mode;
			}

			ImGui::NewLine();
			ImGui::Text("Method:");
			if (ImGui::RadioButtonEx("Linear", &iAB_Method, CAimbot::METHOD_LINEAR, "Aiming route to the target will be linear."))
			{
				g_Options.ab_method = iAB_Method;
			}
			ImGui::SameLine(175);
			if (ImGui::RadioButtonEx("Curved", &iAB_Method, CAimbot::METHOD_CURVED, "Aiming route to the target will be curved.\nHuman-like aiming."))
			{
				g_Options.ab_method = iAB_Method;
			}
			ImGui::SameLine(375);
			if (ImGui::RadioButtonEx("Rage", &iAB_Method, CAimbot::METHOD_RAGE, "Instant aiming targed. Aim will travel from target to target instantly.\nThis mode was created to counter-hacking another hackers.\nUsing this could cause alot of reports and overwatch ban."))
			{
				g_Options.ab_method = iAB_Method;
			}
			ImGui::NewLine();
			ImGui::Text("Options:");
			if (ImGui::Checkbox("Randomize move", &bAB_RandomizeCurve, "Linear and curved movement will be randomized."))
				g_Options.ab_randomize_curve = bAB_RandomizeCurve;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Random hitbox", &bAB_RandomHitbox, "Chooses random hitbox on target lock.\nIf disabled: Hitbox is always 'Head'."))
				g_Options.ab_random_hitbox = bAB_RandomHitbox;
			ImGui::SameLine(375);
			if (ImGui::Checkbox("Display info", &bAB_DisplayInfo, "Show current aimbot status in the top-right corner of the screen."))
				g_Options.ab_display_info = bAB_DisplayInfo;

			if (ImGui::Checkbox("Only scoped", &bAB_OnlyScoped, "Aim only when scoped."))
				g_Options.ab_aim_only_when_scoped = bAB_OnlyScoped;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Disable mouse", &bAB_DisableMouse, "Deny mouse input while aiming."))
				g_Options.ab_disable_mouse = bAB_DisableMouse;
			ImGui::SameLine(375);
			if (ImGui::Checkbox("Player hunt mode", &bAB_HuntPlayer, "You are aiming specific player only.\nYou can enter his name or part of it when enabled.\nAll players containing next string in their names will be aimed."))
				g_Options.ab_hunt_player = bAB_HuntPlayer;
			
			if (bAB_HuntPlayer)
			{
				ImGui::NewLine();

				if (ImGui::InputText("Name of target to hunt", xAB_PlayerName, 256))
				{
					g_Options.ab_filter_name = xAB_PlayerName;
				}
				
			}

			ImGui::NewLine();
			ImGui::Text("Aim key: ");
			ImGui::SameLine(140);
			if (ImGui::Button(g_Options.ab_key_text.GetStr().c_str(), ImVec2(135, 25)))
			{
				g_bKeyEditMode = false;
				g_Options.esp_zoom_key_edit_mode = false;
				g_Options.ab_key_edit_mode = !g_Options.ab_key_edit_mode;
				g_bAlertKeyEditMode = false;
			}

			ImGui::NewLine();
			ImGui::Text("Values:");
			if (ImGui::SliderFloat("Min reaction distance", &fAB_MinReact, 0.0f, 450.0f))
			{
				g_Options.ab_min_reaction_distance = fAB_MinReact;
			}
			if (ImGui::SliderFloat("Max reaction distance", &fAB_MaxReact, 800.0f, 10000.0f))
			{
				g_Options.ab_max_reaction_distance = fAB_MaxReact;
			}
			if (ImGui::SliderFloat("Max crosshair radius", &fAB_MaxCross, 25.0f, 1500.0f))
			{
				g_Options.ab_max_2Ddistance_to_crosshair = fAB_MaxCross;
			}
			if (ImGui::SliderFloat("Velocity", &fAB_Velocity, 0.10f, 2.0f))
			{
				g_Options.ab_velocity = fAB_Velocity;
			}
			if (ImGui::SliderFloat("Velocity reduction", &fAB_VelocityRed, 0.0f, 0.75f))
			{
				g_Options.ab_velocity_reduction = fAB_VelocityRed;
			}
			if (ImGui::SliderFloat("Acceleration", &fAB_Accel, 0.01f, 0.30f))
			{
				g_Options.ab_acceleration = fAB_Accel;
			}
			if (ImGui::ColorEdit3INT("Information color", cAB_InfoColor))
				g_Options.ab_info_color.SetColor(cAB_InfoColor[0], cAB_InfoColor[1], cAB_InfoColor[2], 255);

			break;

		case 4: //Crosshair
			ImGui::Text("Options:");
			ImGui::Checkbox("Use ESP Crosshair", &bCrosshair, "Draws custom crosshair over original.");
			ImGui::SameLine(175);
			ImGui::Checkbox("Align to spread", &bCrosshairSpread, "Your firing spread will be applied to crosshair position.");
			ImGui::SameLine(375);
			ImGui::Checkbox("Hide default crosshair", &bHideDefault);
			ImGui::Text("Type:");
			ImGui::RadioButton("Dot", &iCrosshairType, 0);
			ImGui::SameLine(175);
			ImGui::RadioButton("Square", &iCrosshairType, 1);
			ImGui::SameLine(375);
			ImGui::RadioButton("Circle", &iCrosshairType, 2);
			ImGui::RadioButton("Outlined circle", &iCrosshairType, 3);
			ImGui::SameLine(175);
			ImGui::RadioButton("Cross", &iCrosshairType, 4);
			if(iCrosshairType == 1 || iCrosshairType == 3)
				ImGui::Checkbox("With dot in the center", &bExtraDot);
			ImGui::Text("Sizes:");
			ImGui::SliderInt("Scale", &iCrosshairSize, 2, 30);
			if ((iCrosshairType == 1 || iCrosshairType == 3) && bExtraDot)
				ImGui::SliderInt("Dot scale", &iDotScale, 2, 5);
			if((iCrosshairType == 1 || iCrosshairType == 3 || iCrosshairType == 4))
				ImGui::SliderInt("Line thickness (px)", &iCrosshairThickness, 1, 5);
			ImGui::Text("Color:");
			ImGui::ColorEdit4INT("", cCrosshair);

			break;

		case 5: //ESP 
			
			tabs = new char*[2];
			tabs[0] = "Visual ESP";
			tabs[1] = "Sound ESP";
			ImGui::MyTabLabels(2, const_cast<const char**>(tabs), esptab);
			delete[] tabs;

			switch (esptab)
			{
			case 0:	//Visual ESP
				ImGui::Text("Options:");
				if (ImGui::Checkbox("ESP Enabled", &bESP_Enabled))
					g_Options.esp_enabled = bESP_Enabled;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Allies", &bESP_Allies, "Apply ESP on allies too."))
					g_Options.esp_enemies_only = !bESP_Allies;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("Defuse kits", &bESP_DefuseKits))
					g_Options.esp_defuse_kit = bESP_DefuseKits;

				if (ImGui::Checkbox("Dropped weapons", &bESP_DroppedWeapons))
					g_Options.esp_dropped_weapons = bESP_DroppedWeapons;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Planted bomb", &bESP_PlantedBomb))
					g_Options.esp_planted_c4 = bESP_PlantedBomb;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("Player armor", &bESP_PlayerArmor))
					g_Options.esp_player_armour = bESP_PlayerArmor;

				if (ImGui::Checkbox("Player boxes", &bESP_PlayerBoxes))
					g_Options.esp_player_boxes = bESP_PlayerBoxes;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Player distance", &bESP_PlayerDistance))
					g_Options.esp_player_distance = bESP_PlayerDistance;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("Player HP", &bESP_PlayerHealth))
					g_Options.esp_player_health = bESP_PlayerHealth;

				if (ImGui::Checkbox("Player names", &bESP_PlayerNames))
					g_Options.esp_player_names = bESP_PlayerNames;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Player snaplines", &bESP_PlayerSnaplines))
					g_Options.esp_player_snaplines = bESP_PlayerSnaplines;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("Player weapons", &bESP_PlayerWeapons))
					g_Options.esp_player_weapons = bESP_PlayerWeapons;

				if (ImGui::Checkbox("Boxes head only", &bESP_PlayerBoxesHO, "ESP Box will be on player's head only.\nAlso HP and Armor bars will become smaller."))
					g_Options.esp_player_boxes_head_only = bESP_PlayerBoxesHO;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Grenade prediction", &bESP_GrenPred, "Draws trajectory and landing point of current grenade\nwhen fire (or alt. fire) is held."))
					g_Options.esp_GrenadePrediction = bESP_GrenPred;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("Player Bones", &bESP_Bones, "Draws skeleton of player."))
					g_Options.esp_player_bones = bESP_Bones;

				if (ImGui::Checkbox("Bones HP colored", &bESP_BonesHP, "Color of player's bones will depend\non amount of his health."))
					g_Options.esp_bones_hp_colored = bESP_BonesHP;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Defuse Indicator", &bESP_DefuseStatusEnabled, "When enemy CT defuses a bomb it shows defuse\nprogress over his head."))
					g_Options.esp_player_defuse_status = bESP_DefuseStatusEnabled;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("Notify when spotted", &bESP_NotifySpotted, "When any part of your player model becomes visible to \nan enemy you will be notified by the indicator under\nyour crosshair."))
					g_Options.esp_notify_spotted = bESP_NotifySpotted;

				if (ImGui::Checkbox("Hide Viewmodel", &bESP_HideVM, "Hide hands and weapon out of sight."))
					g_Options.esp_hide_viewmodel = bESP_HideVM;

				if (!bESP_HideVM)
				{
					if (ImGui::SliderFloat("Viewmodel FOV", &fESP_ViewmodelFOV, 0.0f, 160.0f))
					{
						g_Options.esp_zoom_hands_fov = fESP_ViewmodelFOV;
					}
				}
				ImGui::NewLine(); 
				ImGui::Text("Alternative zoom:");
				if (ImGui::Checkbox("Zoom Enabled", &bESP_ZoomEnabled, ""))
				{
					g_Options.esp_zoom_enabled = bESP_ZoomEnabled;
					if (!bESP_ZoomEnabled)
						g_Options.esp_zoom_key_edit_mode = false;
				}
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Instant zoom", &bESP_ZoomInstant))
					g_Options.esp_zoom_instant = bESP_ZoomInstant;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("No zoom when scoping", &bESP_ZoomDS, "Use weapon scope instead of alt. zoom."))
					g_Options.esp_zoom_dont_when_scope = bESP_ZoomDS;

				if (ImGui::Checkbox("Hide VM on zoom", &bESP_HideVMWhenZoom, "Hide view model while zoomed."))
					g_Options.esp_hide_viewmodel_when_zoom = bESP_HideVMWhenZoom;

				if (ImGui::SliderFloat("Zoom FOV", &fESP_ZoomFOV, 1.0f, 160.0f))
						g_Options.esp_zoom_fov = fESP_ZoomFOV;

					ImGui::NewLine();
					ImGui::Text("Zoom key: ");
					ImGui::SameLine(140);
					if (ImGui::Button(g_Options.esp_zoom_key_text.GetStr().c_str(), ImVec2(135, 25)))
					{
						g_bKeyEditMode = false;
						g_Options.ab_key_edit_mode = false;
						g_Options.esp_zoom_key_edit_mode = !g_Options.esp_zoom_key_edit_mode;
						g_bAlertKeyEditMode = false;
					}
			

				if (bESP_DefuseStatusEnabled)
				{
					ImGui::NewLine();
					ImGui::Text("Defuse indication type:");
					if (ImGui::RadioButton("Time above Player", &iESP_DefuseStatusType, 0))
						g_Options.esp_player_defuse_status_type = iESP_DefuseStatusType;
					ImGui::SameLine(175);
					if (ImGui::RadioButton("Progress bar above Player", &iESP_DefuseStatusType, 1))
						g_Options.esp_player_defuse_status_type = iESP_DefuseStatusType;
				}

				ImGui::NewLine();
				ImGui::Text("Font:");
				if (ImGui::SliderInt("Font size", &iESPFontSize, 10, 40))
				{
					g_Options.esp_font_size = iESPFontSize;
					ESP::DestroyFonts();
					ESP::CreateFonts(g_Options.esp_font_size);
				}
				ImGui::NewLine();
				ImGui::Text("Colors:");
				if (ImGui::ColorEdit3INT("Enemy visible", cESPEnemy))
					g_Options.color_esp_enemy_visible.SetColor(cESPEnemy[0], cESPEnemy[1], cESPEnemy[2], 255);
				if (ImGui::ColorEdit3INT("Ally visible", cESPAlly))
					g_Options.color_esp_ally_visible.SetColor(cESPAlly[0], cESPAlly[1], cESPAlly[2], 255);
				if (ImGui::ColorEdit3INT("Enemy occluded", cESPEnemyOcc))
					g_Options.color_esp_enemy_occluded.SetColor(cESPEnemyOcc[0], cESPEnemyOcc[1], cESPEnemyOcc[2], 255);
				if (ImGui::ColorEdit3INT("Ally occluded", cESPAllyOcc))
					g_Options.color_esp_ally_occluded.SetColor(cESPAllyOcc[0], cESPAllyOcc[1], cESPAllyOcc[2], 255);
				if (ImGui::ColorEdit3INT("Prediction trace", cESPGrenade))
					g_Options.color_esp_grenade.SetColor(cESPGrenade[0], cESPGrenade[1], cESPGrenade[2], 255);
				if (ImGui::ColorEdit3INT("Defuse", cESPDefs))
				{
					g_Options.color_esp_defuse.SetColor(cESPDefs[0], cESPDefs[1], cESPDefs[2], 255);
				}
				if (ImGui::ColorEdit3INT("Planted C4", cESPC4))
					g_Options.color_esp_c4.SetColor(cESPC4[0], cESPC4[1], cESPC4[2], 255);
				if (ImGui::ColorEdit3INT("Spotted Notification", cESPNotify))
				{
					g_Options.color_esp_notify.SetColor(cESPNotify[0], cESPNotify[1], cESPNotify[2], 255);
				}

				if (ImGui::SliderInt("Notification Alpha", &iESP_NotifyAlpha, 0, 255))
				{
					g_Options.esp_notify_spotted_alpha = iESP_NotifyAlpha;
				}
				break;
			case 1:	//Sound ESP
				ImGui::Text("Options:");
				if (ImGui::Checkbox("Enabled", &bSESP_Enabled))
					g_Options.sesp_enabled = bSESP_Enabled;
				ImGui::SameLine(175);
				if (ImGui::Checkbox("Notify when spotted", &bSESP_NotifySpotted, "Play a special sound when any part of your\nplayer model becomes visible to an enemy.\n"))
					g_Options.sesp_notify_spotted = bSESP_NotifySpotted;
				ImGui::SameLine(375);
				if (ImGui::Checkbox("CrosshairBeep WH", &bSESP_WHAim, "Beeps when your crosshair is aiming an enemy.\nIt does not matter through the wall or not."))
					g_Options.sesp_whaim_esp = bSESP_WHAim;
			
				if (ImGui::Checkbox("Enemy is near", &bSESP_DistanceEsp, "When at least one enemy is near it will beep.\nThe closer the enemy the faster it beeps."))
					g_Options.sesp_distance_esp = bSESP_DistanceEsp;

				ImGui::Text("Scaled values:");
				if (ImGui::SliderInt("Notify period (sec)", &iSESP_NotifyPause, 0, 15))
					g_Options.sesp_notify_spotted_pause = iSESP_NotifyPause;
				if (ImGui::SliderInt("CrosshairBeep Period (ms)", &iSESP_WHAimPeriod, 0, 1000))
					g_Options.sesp_whaim_period = iSESP_WHAimPeriod;
				if (ImGui::SliderInt("'Enemy is Near' radius", &iSESP_DistanceEspRadius, 90, 1500))
					g_Options.sesp_distance_radius = iSESP_DistanceEspRadius;
				ImGui::Text("Beep settings:");
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[Frequency] [Duration]");
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "The allowed range of values is: from 50 to 750.");

				//--------------------------------------
				int iSESP_WHAimFreq, iSESP_WHAimDur;
				if (ImGui::InputTextEx("SESP_WHAimFreq", isSESP_WHAimFreq, 256, ImVec2(100, 19), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_NoLabel))
				{
					iSESP_WHAimFreq = atoi(isSESP_WHAimFreq);
					if (iSESP_WHAimFreq < 50)
						iSESP_WHAimFreq = 50;
					if (iSESP_WHAimFreq > 750)
						iSESP_WHAimFreq = 750;
					wsprintfA(isSESP_WHAimFreq, "%i", iSESP_WHAimFreq);
					g_Options.sesp_whaim_freq = iSESP_WHAimFreq;
				}
				ImGui::SameLine(110);
				if (ImGui::InputTextEx("SESP_WHAimDur", isSESP_WHAimDur, 256, ImVec2(100, 19), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_NoLabel))
				{
					iSESP_WHAimDur = atoi(isSESP_WHAimDur);
					if (iSESP_WHAimDur < 50)
						iSESP_WHAimDur = 50;
					if (iSESP_WHAimDur > 750)
						iSESP_WHAimDur = 750;
					wsprintfA(isSESP_WHAimDur, "%i", iSESP_WHAimDur);
					g_Options.sesp_whaim_dur = iSESP_WHAimDur;
				}
				ImGui::SameLine(212);
				if (ImGui::Button("Test", ImVec2(50, 19), "Test1"))
				{
					iSESP_WHAimFreq = atoi(isSESP_WHAimFreq);
					iSESP_WHAimDur = atoi(isSESP_WHAimDur);
					ESP::Sound::BeepAsync(iSESP_WHAimFreq, iSESP_WHAimDur, 0, 75);
				}
				ImGui::SameLine(265);
				ImGui::Text("CrosshairBeep WH");

				//--------------------------------------
				int iSESP_DistanceEspFreq, iSESP_DistanceEspDur;
				if (ImGui::InputTextEx("SESP_DistanceEspFreq", isSESP_DistanceEspFreq, 256, ImVec2(100, 19), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_NoLabel))
				{
					iSESP_DistanceEspFreq = atoi(isSESP_DistanceEspFreq);
					if (iSESP_DistanceEspFreq < 50)
						iSESP_DistanceEspFreq = 50;
					if (iSESP_DistanceEspFreq > 750)
						iSESP_DistanceEspFreq = 750;
					wsprintfA(isSESP_DistanceEspFreq, "%i", iSESP_DistanceEspFreq);
					g_Options.sesp_distance_freq = iSESP_DistanceEspFreq;
				}
				ImGui::SameLine(110);
				if (ImGui::InputTextEx("SESP_DistanceEspDur", isSESP_DistanceEspDur, 256, ImVec2(100, 19), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_NoLabel))
				{
					iSESP_DistanceEspDur = atoi(isSESP_DistanceEspDur);
					if (iSESP_DistanceEspDur < 50)
						iSESP_DistanceEspDur = 50;
					if (iSESP_DistanceEspDur > 750)
						iSESP_DistanceEspDur = 750;
					wsprintfA(isSESP_DistanceEspDur, "%i", iSESP_DistanceEspDur);
					g_Options.sesp_distance_dur = iSESP_DistanceEspDur;
				}
				ImGui::SameLine(212);
				if (ImGui::Button("Test", ImVec2(50, 19), "Test2"))
				{
					iSESP_DistanceEspFreq = atoi(isSESP_DistanceEspFreq);
					iSESP_DistanceEspDur = atoi(isSESP_DistanceEspDur);
					ESP::Sound::BeepAsync(iSESP_DistanceEspFreq, iSESP_DistanceEspDur, 1, 75);
				}
				ImGui::SameLine(265);
				ImGui::Text("Enemy is near");

				break;
			default:
				__assume(0);
			}
			break;

		case 6: //Other
			ImGui::Text("Options:");
			if (ImGui::Checkbox("RadarHack", &bRadarHack))
			{
				WaitForSingleObject(hRadarMutex, INFINITE);
				g_RH = bRadarHack;
				ReleaseMutex(hRadarMutex);
			}
			ImGui::SameLine(175);
			if (ImGui::Checkbox("Reveal ranks", &bRevealRanks))
				g_RevealRanks = bRevealRanks;
			ImGui::SameLine(375);
			if (ImGui::Checkbox("NoFlash", &bNoFlash))
			{
				WaitForSingleObject(hFlashMutex, INFINITE);
				g_NF = bNoFlash;
				ReleaseMutex(hFlashMutex);
			}
			if (ImGui::SliderFloat("Flash Alpha", &fFlashAlpha, 0.0f, 255.0f, "%.2f"))
			{
				WaitForSingleObject(hFlashMutex, INFINITE);
				g_FlashAlpha = fFlashAlpha;
				ReleaseMutex(hFlashMutex);
			}
			ImGui::Text("FakeLag:");
			if (ImGui::Checkbox("Enable FakeLag", &bFakeLag))
				g_FakeLag = bFakeLag;
			ImGui::SameLine(175);
			if (ImGui::Checkbox("FakeLag Randomize time", &bFakeLagRandomize, "Period and duration will be random.\nYou can set max value of it."))
				g_FakeLagRandomize = bFakeLagRandomize;
			if (ImGui::Checkbox("Trigger hotkey to un-lag", &bFakeLagUseKey, "When you are holding TriggerBot hotkey (No matter enabled it or not)\nthis will immediately interrupt fake lags."))
				g_FakeLagUseKey = bFakeLagUseKey;
			if (bFakeLagRandomize)
			{
				flt_per = "FakeLag max period (ms)";
				flt_dur = "FakeLag max duration (ms)";
			}
			else
			{
				flt_per = "FakeLag period (ms)";
				flt_dur = "FakeLag duration (ms)";
			}
			if (ImGui::SliderInt(flt_per.c_str(), &iFakeLagPeriod, 125, 2000))
			{
				WaitForSingleObject(hFakeLagMutex, INFINITE);
				g_FakeLagPeriod = iFakeLagPeriod;
				ReleaseMutex(hFakeLagMutex);
			}
			if (ImGui::SliderInt(flt_dur.c_str(), &iFakeLagDuration, 125, 2000))
			{
				WaitForSingleObject(hFakeLagMutex, INFINITE);
				g_FakeLagDuration = iFakeLagDuration;
				ReleaseMutex(hFakeLagMutex);
			}			

			ImGui::NewLine();
			ImGui::TextColored(ImVec4(1.0f, 0, 0, 1.0f), "Alert key:");
			ImGui::SameLine(140);
			if (ImGui::Button(g_sAlertKeyText.GetStr().c_str(), ImVec2(135, 25)))
			{
				g_bKeyEditMode = false;
				g_Options.ab_key_edit_mode = false;
				g_Options.esp_zoom_key_edit_mode = false;;
				g_bAlertKeyEditMode = !g_bAlertKeyEditMode;
			}
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Pressing this key immediately stops all hacks.");
			ImGui::Text("PhantomHack state:");
			ImGui::SameLine(160);
			if(g_DisableAll)
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "SUSPENDED");
			else
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "RUNNING");

			break;
		default:
			__assume(0);
	}

	
	
	ImGui::EndChild();

	if (ImGui::Button("Save config", ImVec2(100, 25)))
		ImGui::OpenPopup("Save config?");
	ImGui::SameLine(110);
	if (ImGui::Button("Reset all settings", ImVec2(170, 25)))
		ImGui::OpenPopup("Reset all settings?");
	ImGui::SameLine(282);
	if (ImGui::Button("Reload from file", ImVec2(170, 25)))
		ImGui::OpenPopup("Reload config?");

	int save_selected = msgbox_save.Draw();
	int reset_selected = msgbox_reset.Draw();
	int reload_selected = msgbox_reload.Draw();

	switch (save_selected)
	{
		case 1:
			SaveConfig();
			break;
		case 2:
		case 0:
			break;
	}

	switch (reset_selected)
	{
		case 1:
			ResetAndReinitConfig();
			ESP::DestroyFonts();
			ESP::CreateFonts(g_Options.esp_font_size);
			return;
			break;
		case 2:
		case 0:
			break;
	}

	switch (reload_selected)
	{
		case 1:
			InitConfig();
			ESP::DestroyFonts();
			ESP::CreateFonts(g_Options.esp_font_size);
			return;
			break;
		case 2:
		case 0:
			break;
	}

	if (bhd_last != bHideDefault)
	{
		bhd_last = bHideDefault;
		if(bHideDefault)
			g_EngineClient->ExecuteClientCmd("cl_crosshairalpha 0");
		else
			g_EngineClient->ExecuteClientCmd("cl_crosshairalpha 255");
	}

	WaitForSingleObject(hCrossMutex, INFINITE);
	g_bCrosshair = bCrosshair;
	g_iCrosshairSize = iCrosshairSize;
	for (size_t i = 0; i < 4; i++)
		g_cCrosshair[i] = cCrosshair[i];
	g_bCrosshairSpread = bCrosshairSpread;
	g_iCrosshairType = iCrosshairType;
	g_iCrosshairThickness = iCrosshairThickness;
	g_bHideDefault = bHideDefault;
	g_ExtraDot = bExtraDot;
	g_iDotScale = iDotScale;
	ReleaseMutex(hCrossMutex);
}

void __fastcall DrawMenu(D3D9DevPtr lpD3D9Dev)
{
	if (!gui_bInit)
	{
		ImGuiIO& gui_io = ImGui::GetIO();
		gui_io.DeltaTime = 1.0f / 60.0f;
		lpD3D9Dev->GetCreationParameters(&gui_d3dcp);
		gui_io.Fonts->AddFontDefault();
		ImGui_ImplDX9_Init(gui_d3dcp.hFocusWindow, lpD3D9Dev);
		ImGuiStyle& style = ImGui::GetStyle();
		
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.9f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.70f, 0.50f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.08f, 0.12f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.00f, 0.15f, 0.56f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.30f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.6f, 0.6f, 0.90f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.7f, 0.7f, 1.00f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.85f, 1.00f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.55f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.16f, 0.40f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.22f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.31f, 0.31f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.9f, 0.0f, 1.00f);
		style.WindowRounding = 0.0f;
		style.ChildWindowRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		gui_bInit = true;
	}



	WaitForSingleObject(hMenuMutex, INFINITE);
	if (g_bMenuVisible)
	{
		DWORD dwOld_D3DRS_COLORWRITEENABLE;
		lpD3D9Dev->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
		lpD3D9Dev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		ReleaseMutex(hMenuMutex);
		ImGui_ImplDX9_NewFrame();
		ImGui::SetNextWindowSize(ImVec2(MENU_W, MENU_H));
		std::string Title = "PhantomHack v" + g_Version;
		ImGui::Begin(Title.c_str(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginGroup();
		ImGui::GetIO().MouseDrawCursor = true;
		if (cl_mouseenable == 1)
		{
			cl_mouseenable = 0;
			g_EngineClient->ExecuteClientCmd("cl_mouseenable 0");
		}
		DrawMenuContent(lpD3D9Dev);
		ImGui::EndGroup();
		ImGui::End();
		ImGui::Render();
		lpD3D9Dev->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	}
	else
	{
		ReleaseMutex(hMenuMutex);
		ImGui::GetIO().MouseDrawCursor = false;
		if (cl_mouseenable == 0)
		{
			cl_mouseenable = 1;
			g_EngineClient->ExecuteClientCmd("cl_mouseenable 1");	
		}
		SetCursor(NULL);
	}

	

}



