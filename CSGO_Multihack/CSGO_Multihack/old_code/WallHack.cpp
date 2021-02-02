#include <process.h>
#include "CSGO_Process.h"
#include "offsets.h"
#include "sdk\include\csgosdk.h"

extern HANDLE hGlowMutex;
extern HANDLE g_hEventWH;
extern volatile float g_cGlowVis[3];
extern volatile float g_cGlowSpot[3];
extern volatile bool g_bSpotted;
extern volatile float g_cGlowAlpha;
volatile bool g_bGlowEnabled = false;


CSGO_Process* l_csgo_mem;




//player struct
struct player
{
	float r, g, b, a;
	bool backlight;
};

void LightPlayer(DWORD bClient, int human, player man, float alpha = 0) // show player
{
	man.a = alpha;
	int GlowIndex = l_csgo_mem->Read<int>(human + off_glowIndex);
	int GlowObject = l_csgo_mem->Read<int>(bClient + off_glowObject);
	int calculation = GlowIndex * 0x38 + 0x4;
	l_csgo_mem->Write<float>(GlowObject + calculation, man.r);
	calculation = GlowIndex * 0x38 + 0x8;
	l_csgo_mem->Write<float>(GlowObject + calculation, man.g);
	calculation = GlowIndex * 0x38 + 0xC;
	l_csgo_mem->Write<float>(GlowObject + calculation, man.b);
	calculation = GlowIndex * 0x38 + 0x10;
	l_csgo_mem->Write<float>(GlowObject + calculation, man.a);
	calculation = GlowIndex * 0x38 + 0x24;
	l_csgo_mem->Write<bool>(GlowObject + calculation, man.backlight);
}


void __stdcall WallHackThread(LPVOID lpThreadParameter)
{
	l_csgo_mem = new CSGO_Process;

	HMODULE lib = LoadLibrary(TEXT("ntdll.dll"));

	
	NtRVM NtReadVirtualMemory = (NtRVM)GetProcAddress(lib, "NtReadVirtualMemory");
	NtWVM NtWriteVirtualMemory = (NtWVM)GetProcAddress(lib, "NtWriteVirtualMemory");
	
	player glCol;
	glCol.r = 0;
	glCol.g = 0;
	glCol.b = 0;
	glCol.a = 0;
	glCol.backlight = true;

	player spotCol;
	spotCol.r = 0;
	spotCol.g = 0;
	spotCol.b = 0;
	spotCol.a = 0;
	spotCol.backlight = true;



	player none;
	none.r = 0;
	none.g = 0;
	none.b = 0;
	none.a = 0;
	none.backlight = false;

	DWORD bClient, bEngine, LocalBase;
	int human, team, myteam, visible;
	bool dormant;
	float al;

	bClient = l_csgo_mem->Module(TEXT("client.dll"));
	bEngine = l_csgo_mem->Module(TEXT("engine.dll"));
	
	PulseEvent(g_hEventWH);

	while (true) 
	{
		WaitForSingleObject(hGlowMutex, INFINITE);
		if (g_bGlowEnabled)
		{
			glCol.r = g_cGlowVis[0];
			glCol.g = g_cGlowVis[1];
			glCol.b = g_cGlowVis[2];
			if (g_bSpotted)
			{
				spotCol.r = g_cGlowSpot[0];
				spotCol.g = g_cGlowSpot[1];
				spotCol.b = g_cGlowSpot[2];
			}
			else
			{
				spotCol.r = g_cGlowVis[0];
				spotCol.g = g_cGlowVis[1];
				spotCol.b = g_cGlowVis[2];
			}
			al = g_cGlowAlpha;
			ReleaseMutex(hGlowMutex);
			for (int i = 0; i <= 64; ++i) // player's loop
			{
				LocalBase = l_csgo_mem->Read<int>(bClient + off_dwLocalPlayer);
				human = l_csgo_mem->Read<int>(bClient + off_EntityList + (i - 1) * 0x10); // get player
				team = l_csgo_mem->Read<int>(human + off_iTeam); // get player's team
				myteam = l_csgo_mem->Read<int>(LocalBase + off_iTeam); // get my team
				dormant = l_csgo_mem->Read<bool>(human + off_oDormant); // check if object is player
				visible = l_csgo_mem->Read<int>(human + off_bSpottedMask); // if player is visible


				if (!dormant && myteam != team)
				{
					if (visible)	
							LightPlayer(bClient, human, spotCol, al);
					else
						LightPlayer(bClient, human, glCol, al);

				}
				else
					LightPlayer(bClient, human, none);
			}
		}
		else
			ReleaseMutex(hGlowMutex);
		Sleep(10); 
	}
}

