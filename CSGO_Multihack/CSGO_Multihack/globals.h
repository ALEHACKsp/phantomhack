#pragma once

#include "ConfigMgr.h"
#include "Aimbot.h"
#include <xstring.h>
#include "d3d.h"
#include <string>
#include "chams.h"
#include "esp.h"
#include "glow.h"
#include "sdk/include/valve_sdk/misc/Color.hpp"

#define VK_ALT						18
#define VK_LALT						164
#define CSGO_ATTACK					5
#define CSGO_STOP_ATTACK			4
#define CSGO_TEAM_CT				3
#define CSGO_TEAM_T					2
#define MENU_H						440
#define MENU_W						620

typedef BYTE* LPSNDBUF;


#ifdef _DEFINE_GLOBALS_

LRESULT __stdcall hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __stdcall FakeLagThread(LPVOID lpThreadParameter);
void RadarHack();
void NoFlash();

CConfigMgr g_ConfigManager;

xstringA g_Version = "1.20";
DWORD g_ActivatorProcID = 0;
HANDLE g_hActivatorProc = NULL;
std::string g_HackPath = "";
std::string g_ConfigName = "config.cfg";
WNDPROC oWndProc = NULL;
HANDLE hTriggerHotkeyMutex = NULL;
HANDLE hRadarMutex = NULL;
HANDLE hCrossMutex = NULL;
HANDLE hFlashMutex = NULL;
HANDLE hMenuMutex = NULL;
HANDLE hFakeLagMutex = NULL;
LPDIRECT3DDEVICE9 g_npDevice = NULL;
xstring g_TrigKeyText = TEXT("Alt");
volatile int g_TBMode = 0;
volatile float g_FlashAlpha = 0.0f;
volatile int g_TPostDelay = 0;
volatile DWORD g_TriggerHotkey = VK_ALT;
volatile int g_iCrosshairSize = 2;
volatile int g_cGlowAlpha = 255;
volatile int g_iCrosshairType = 0;
volatile int g_iCrosshairThickness = 1;
volatile int g_iDotScale = 2;
volatile int g_FakeLagDuration = 500;
volatile int g_FakeLagPeriod = 500;
volatile int g_TriggerBotMethod = 0;
volatile bool g_TBIgnoreWalls = false;
volatile bool g_TriggerHeadsOnly = false;
volatile bool g_ExtraDot = false;
volatile bool g_ChamsEnabled = false;
volatile bool g_ChamsSepInvisCol = true;
volatile bool g_bOnlyHead = false;
volatile bool g_bHeadDifCol = true;
bool g_CorrectProcess = false;
volatile bool g_D3DWH_HeadsOnly = false;
volatile bool g_bHideDefault = false;
volatile bool g_bKeyEditMode = false;
volatile bool g_bMenuVisible = false;
volatile bool g_bCrosshair = true;
volatile bool g_bCrosshairSpread = false;
volatile bool g_TriggerHoldFire = false;
volatile bool g_RH = false;
volatile bool g_NF = false;
volatile bool g_RevealRanks = false;
volatile bool g_FakeLag = false;
volatile bool g_FakeLagRandomize = false;
volatile bool g_FakeLagNow = false;
volatile bool g_FakeLagUseKey = false;
volatile int g_cGen[3] = { 0, 255, 0 };
volatile int g_cInv[3] = { 0, 150, 0 };
volatile int g_cHead[3] = { 255, 0, 0 };
volatile int g_cInvHead[3] = { 150, 0, 0 };
volatile int g_cCrosshair[4] = { 255, 0, 0, 255 };
xstring g_sDebugInfo;
volatile int g_AimBot_Mode = CAimbot::MODE_DISABLED;
volatile DWORD g_AimBot_Key = VK_MBUTTON;
LPSNDBUF g_sndNotice = nullptr;
size_t g_sndNoticeSize = 0;
volatile bool g_DisableAll = false;
volatile DWORD g_dwAlertKey = VK_DELETE;
HANDLE g_hAlertKeyMutex = NULL;
xstringA g_sAlertKeyText = "";
volatile bool g_bAlertKeyEditMode = false;


#else

extern xstringA g_Version;
extern CConfigMgr g_ConfigManager;
extern DWORD g_ActivatorProcID;
extern HANDLE g_hActivatorProc;
extern std::string g_HackPath;
extern std::string g_ConfigName;
extern WNDPROC oWndProc;
extern HANDLE hTriggerHotkeyMutex;
extern HANDLE hRadarMutex;
extern HANDLE hCrossMutex;
extern HANDLE hFlashMutex;
extern HANDLE hMenuMutex;
extern HANDLE hFakeLagMutex;
extern LPDIRECT3DDEVICE9 g_npDevice;
extern xstring g_TrigKeyText;
extern volatile float g_FlashAlpha;
extern volatile DWORD g_TriggerHotkey;
extern volatile int g_TBMode;
extern volatile int g_iCrosshairSize;
extern volatile int g_TPostDelay;
extern volatile int g_iCrosshairType;
extern volatile int g_iCrosshairThickness;
extern volatile int g_iDotScale;
extern volatile int g_FakeLagDuration;
extern volatile int g_FakeLagPeriod;
extern volatile int g_TriggerBotMethod;
extern volatile bool g_TBIgnoreWalls;
extern volatile bool g_TriggerHeadsOnly;
extern volatile bool g_RH;
extern volatile bool g_NF;
extern volatile bool g_ExtraDot;
extern volatile bool g_ChamsEnabled;
extern volatile bool g_ChamsSepInvisCol;
extern volatile bool g_bOnlyHead;
extern volatile bool g_bHeadDifCol;
extern volatile bool g_bMenuVisible;
extern volatile bool g_bCrosshair;
extern volatile bool g_bCrosshairSpread;
extern volatile bool g_D3DWH_HeadsOnly;
extern volatile bool g_bHideDefault;
extern volatile bool g_bKeyEditMode;
extern volatile bool g_FakeLag;
extern volatile bool g_FakeLagNow;
extern volatile bool g_FakeLagRandomize;
extern volatile bool g_FakeLagUseKey;
extern volatile bool g_TriggerHoldFire;
extern volatile bool g_DisableAll;
extern bool g_CorrectProcess;
extern volatile int g_cGen[3];
extern volatile int g_cInv[3];
extern volatile int g_cHead[3];
extern volatile int g_cInvHead[3];
extern volatile int g_cCrosshair[4];
extern volatile int g_cGlowAlpha;
extern volatile bool g_RevealRanks;
extern HANDLE g_hEventD3D;
extern xstring g_sDebugInfo;
extern volatile int g_AimBot_Mode;
extern volatile DWORD g_AimBot_Key;
extern LPSNDBUF g_sndNotice;
extern size_t g_sndNoticeSize;
extern volatile DWORD g_dwAlertKey;
extern volatile bool g_bAlertKeyEditMode;
extern HANDLE g_hAlertKeyMutex;
extern xstringA g_sAlertKeyText;

#endif

#pragma once

#define OPTION(type, var, val) type var = val

class Config
{
public:
	// 
	// ESP
	// 
	OPTION(bool, esp_enabled, false);
	OPTION(int, esp_font_size, 10);
	OPTION(bool, esp_enemies_only, false);
	OPTION(bool, esp_GrenadePrediction, false);
	OPTION(bool, esp_player_boxes, false);
	OPTION(bool, esp_player_boxes_head_only, false);
	OPTION(bool, esp_player_names, false);
	OPTION(bool, esp_player_distance, false);
	OPTION(bool, esp_player_health, false);
	OPTION(bool, esp_player_armour, false);
	OPTION(bool, esp_player_weapons, false);
	OPTION(bool, esp_player_snaplines, false);
	OPTION(bool, esp_crosshair, false);
	OPTION(bool, esp_dropped_weapons, false);
	OPTION(bool, esp_defuse_kit, false);
	OPTION(bool, esp_planted_c4, false);
	OPTION(bool, esp_bones_hp_colored, false);
	OPTION(bool, esp_player_bones, false); 
	OPTION(bool, esp_player_defuse_status, false);
	OPTION(int, esp_player_defuse_status_type, 0);
	OPTION(bool, esp_notify_spotted, false);
	OPTION(int, esp_notify_spotted_alpha, 130);
	OPTION(bool, esp_zoom_enabled, false);
	OPTION(bool, esp_zoom_key_edit_mode, false);
	OPTION(HANDLE, esp_zoom_key_mutex, NULL);
	OPTION(float, esp_zoom_fov, 65);
	OPTION(DWORD, esp_zoom_key, 69);
	OPTION(xstringA, esp_zoom_key_text, "");
	OPTION(bool, esp_zoom_instant, false);
	OPTION(bool, esp_zoom_dont_when_scope, false);
	OPTION(float, esp_zoom_hands_fov, 90.0f);
	OPTION(bool, esp_hide_viewmodel, false);
	OPTION(bool, esp_hide_viewmodel_when_zoom, false);

	// 
	// SoundESP
	// 
	OPTION(bool, sesp_enabled, false);
	OPTION(bool, sesp_notify_spotted, false); 
	OPTION(bool, sesp_distance_esp, false);
	OPTION(bool, sesp_whaim_esp, false);
	OPTION(int, sesp_notify_spotted_pause, 3);
	OPTION(int, sesp_distance_freq, 250);
	OPTION(int, sesp_distance_dur, 200);
	OPTION(int, sesp_distance_radius, 1000);
	OPTION(int, sesp_whaim_freq, 600);
	OPTION(int, sesp_whaim_dur, 200);
	OPTION(int, sesp_whaim_period, 200);

	// 
	// GLOW
	// 
	OPTION(bool, glow_enabled, false);
	OPTION(bool, glow_enemies_only, true);
	OPTION(bool, glow_players, false);
	OPTION(bool, glow_chickens, false);
	OPTION(bool, glow_c4_carrier, false);
	OPTION(bool, glow_planted_c4, false);
	OPTION(bool, glow_defuse_kits, false);
	OPTION(bool, glow_weapons, false);
	OPTION(bool, glow_spotted_another_color, false);

	// 
	// AIMBOT
	// 
	OPTION(int, ab_mode, CAimbot::MODE_DISABLED);
	OPTION(int, ab_method, CAimbot::METHOD_LINEAR);
	OPTION(bool, ab_randomize_curve, false);
	OPTION(bool, ab_display_info, false);
	OPTION(bool, ab_aim_only_when_scoped, false);
	OPTION(bool, ab_random_hitbox, false);
	OPTION(bool, ab_hunt_player, false);
	OPTION(bool, ab_disable_mouse, false);
	OPTION(DWORD, ab_hotkey, VK_MBUTTON);
	OPTION(float, ab_min_reaction_distance, 0.0f);
	OPTION(float, ab_max_reaction_distance, 2000.0f);
	OPTION(float, ab_max_2Ddistance_to_crosshair, 600.0f);
	OPTION(float, ab_velocity, 1.0f);
	OPTION(float, ab_velocity_reduction, 0.0f);
	OPTION(float, ab_acceleration, 0.05f);
	OPTION(xstringA, ab_filter_name, "");
	OPTION(HANDLE, ab_key_mutex, NULL); 
	OPTION(bool, ab_key_edit_mode, false);
	OPTION(xstringA, ab_key_text, "");
	OPTION(Color, ab_info_color, Color(0,255,0,255));

	//
	// CHAMS
	//
	OPTION(bool, chams_player_enabled, false);
	OPTION(bool, chams_player_enemies_only, false);
	OPTION(bool, chams_player_wireframe, false);
	OPTION(bool, chams_player_flat, false);
	OPTION(bool, chams_player_ignorez, false);
	OPTION(bool, chams_player_glass, false);
	OPTION(bool, chams_arms_enabled, false);
	OPTION(bool, chams_arms_wireframe, false);
	OPTION(bool, chams_arms_flat, false);
	OPTION(bool, chams_arms_ignorez, false);
	OPTION(bool, chams_arms_glass, false);
	OPTION(int, chams_alpha_p, 255);
	OPTION(int, chams_alpha_a, 255);

	//
	// MISC
	//
	OPTION(bool, misc_bhop, false);
	OPTION(bool, misc_no_hands, false);
	OPTION(int, viewmodel_fov, 68);
	OPTION(float, mat_ambient_light_r, 0.0f);
	OPTION(float, mat_ambient_light_g, 0.0f);
	OPTION(float, mat_ambient_light_b, 0.0f);

	// 
	// COLORS
	// 
	OPTION(Color, color_esp_ally_visible, Color(0, 128, 255));
	OPTION(Color, color_esp_enemy_visible, Color(255, 0, 0));
	OPTION(Color, color_esp_ally_occluded, Color(0, 128, 255));
	OPTION(Color, color_esp_enemy_occluded, Color(255, 0, 0));
	OPTION(Color, color_esp_crosshair, Color(255, 255, 255));
	OPTION(Color, color_esp_weapons, Color(128, 0, 128));
	OPTION(Color, color_esp_defuse, Color(0, 128, 255));
	OPTION(Color, color_esp_c4, Color(0, 175, 255));
	OPTION(Color, color_esp_grenade, Color(0, 255, 0));
	OPTION(Color, color_esp_notify, Color(255, 0, 0, esp_notify_spotted_alpha));

	OPTION(Color, color_glow_ally, Color(0, 128, 255));
	OPTION(Color, color_glow_enemy, Color(255, 0, 0));
	OPTION(Color, color_glow_chickens, Color(0, 128, 0));
	OPTION(Color, color_glow_c4_carrier, Color(255, 255, 0));
	OPTION(Color, color_glow_planted_c4, Color(128, 0, 128));
	OPTION(Color, color_glow_defuse, Color(255, 255, 255));
	OPTION(Color, color_glow_weapons, Color(255, 128, 0));
	OPTION(Color, color_glow_spotted_enemy, Color(255, 150, 150));
	OPTION(Color, color_glow_spotted_ally, Color(0, 255, 0));

	OPTION(Color, color_chams_player_ally_visible, Color(0, 128, 255));
	OPTION(Color, color_chams_player_ally_occluded, Color(0, 255, 128));
	OPTION(Color, color_chams_player_enemy_visible, Color(255, 0, 0));
	OPTION(Color, color_chams_player_enemy_occluded, Color(255, 128, 0));
	OPTION(Color, color_chams_arms_visible, Color(0, 128, 255));
	OPTION(Color, color_chams_arms_occluded, Color(0, 128, 255));
};

extern Config g_Options;
extern bool   g_Unload;


