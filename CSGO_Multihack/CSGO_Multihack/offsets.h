#pragma once

#ifdef _DEFINE_OFFSETS_

DWORD off_bSpotted;
DWORD off_dwLocalPlayer; 
DWORD off_EntityList; 
DWORD off_iTeam ;
DWORD off_oDormant;
DWORD off_glowObject;
DWORD off_glowIndex;
DWORD off_bSpottedMask;
DWORD off_iHealth;
DWORD off_iCrossHairID;
DWORD off_Attack;
DWORD off_EntityLoopDistance;
DWORD off_FlashMaxAlpha;
DWORD off_FlashDuration;
DWORD off_EnginePointer;
DWORD off_ClientState_Map;
DWORD off_PunchAngle;
DWORD off_dwViewAngles;
DWORD off_aimPunchAngle;
DWORD off_viewPunchAngle;
DWORD off_aimPunchAngleVel;
DWORD off_cmd;
DWORD off_eyeAngles;

#else

extern DWORD off_bSpotted;
extern DWORD off_cmd;
extern DWORD off_dwLocalPlayer;
extern DWORD off_EntityList;
extern DWORD off_iTeam;
extern DWORD off_oDormant;
extern DWORD off_glowObject;
extern DWORD off_glowIndex;
extern DWORD off_bSpottedMask;
extern DWORD off_iHealth;
extern DWORD off_iCrossHairID;
extern DWORD off_Attack;
extern DWORD off_EntityLoopDistance;
extern DWORD off_FlashMaxAlpha;
extern DWORD off_FlashDuration;
extern DWORD off_EnginePointer;
extern DWORD off_ClientState_Map;
extern DWORD off_PunchAngle;
extern DWORD off_dwViewAngles;
extern DWORD off_aimPunchAngle;
extern DWORD off_viewPunchAngle;
extern DWORD off_aimPunchAngleVel;
extern DWORD off_eyeAngles;

#endif