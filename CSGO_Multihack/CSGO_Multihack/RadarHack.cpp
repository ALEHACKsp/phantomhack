#include "sdk\include\csgosdk.h"
#include "globals.h"

void RadarHack()
{
	if (!g_LocalPlayer)
		return;

	WaitForSingleObject(hRadarMutex, INFINITE);
	if (g_RH) 
	{
		ReleaseMutex(hRadarMutex);

		for (int i = 1; i < 65; i++)
		{
			C_BasePlayer* pl = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
			if (!pl || pl == g_LocalPlayer || pl->IsDormant() || g_LocalPlayer->m_iTeamNum() == pl->m_iTeamNum())
				continue;
				
			pl->m_bSpotted() = true;
		}
	}
	else
		ReleaseMutex(hRadarMutex);
}
