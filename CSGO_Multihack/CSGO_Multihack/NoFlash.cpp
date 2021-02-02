#include "sdk\include\csgosdk.h"
#include "globals.h"

void NoFlash()
{
	if (!g_LocalPlayer)
		return;

	WaitForSingleObject(hFlashMutex, INFINITE);
	if (g_NF)
	{
		float alpha = g_FlashAlpha;
		ReleaseMutex(hFlashMutex);
		g_LocalPlayer->m_flFlashMaxAlpha() = alpha;
		//g_LocalPlayer->m_flFlashDuration() = 0;
	}
	else
	{
		ReleaseMutex(hFlashMutex);
		g_LocalPlayer->m_flFlashMaxAlpha() = 255.0f;
	}
}
