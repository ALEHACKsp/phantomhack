#include <process.h>
#include "MemMgr.h"
#include "offsets.h"
#include "xstring.h"
#include "sdk\include\csgosdk.h"
#include <tchar.h>
#include "globals.h"


void __stdcall _updGlobals(LPVOID lpThreadParameter)
{
	CMemMgr csgo_mem;	//csgo.exe memory operating unit
	DWORD EnginePtr, LocalBase;
	const size_t mapBufSize = 256;
	CHAR MapName[mapBufSize] = {'\0'};
	SIZE_T br = 0;
	TEXT("client.dll");	
	TEXT("engine.dll");
	g_MapName = "";
	bool init = false;
	while (true)
	{
		EnginePtr = csgo_mem.Read<int>(TEXT("engine.dll"), off_EnginePointer);
		br = csgo_mem.ReadDataAsStringA(EnginePtr, off_ClientState_Map, MapName, mapBufSize);
		if (br == 0)
			strcpy_s(MapName, 256, "");

		g_MapName = MapName; //Thread safe string

		WaitForSingleObject(hGlobalsMutex, INFINITE);

		if (g_LocalPlayer)
		{
			LocalBase = csgo_mem.Read<int>(TEXT("client.dll"), off_dwLocalPlayer);
			g_myTeam = csgo_mem.Read<int>(LocalBase, off_iTeam);
		}
		else
		{
			g_myTeam = 0;
		}

		ReleaseMutex(hGlobalsMutex);

		if (!init)
		{
			init = true;
			PulseEvent(g_hEventGlobals);
		}
		Sleep(5);
	}
}