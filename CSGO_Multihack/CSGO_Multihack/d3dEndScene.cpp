#include <Process.h>
#include <Windows.h>
#include "d3d.h"
#include "MemMgr.h"
#include "offsets.h"
#include "sdk/include/csgosdk.h"
#include "Draw.h"
#include "globals.h"

void __fastcall DrawMenu(D3D9DevPtr lpD3D9Dev);

static D3DDEVICE_CREATION_PARAMETERS s_cparams;
static bool s_dcpi = false;

void __fastcall DrawCrosshair(D3D9DevPtr lpD3D9Dev)
{
	WaitForSingleObject(hCrossMutex, INFINITE);
	D3DCOLOR color = D3DCOLOR_RGBA(g_cCrosshair[0], g_cCrosshair[1], g_cCrosshair[2], g_cCrosshair[3]);
	int ctype = g_iCrosshairType;
	int thickness = g_iCrosshairThickness;
	int csize = g_iCrosshairSize;
	int dotscale = g_iDotScale;
	bool extradot = g_ExtraDot;
	bool cspread = g_bCrosshairSpread;
	if (!g_bCrosshair)
	{
		ReleaseMutex(hCrossMutex);
		return;
	}
	ReleaseMutex(hCrossMutex);

	if (!g_LocalPlayer)
		return;

	auto hp = g_LocalPlayer->m_iHealth();
	if (hp < 1)
		return;

	if (!s_dcpi)
	{
		lpD3D9Dev->GetCreationParameters(&s_cparams);
		s_dcpi = true;
	}

	register INT16 x, y;
	static RECT s_rect;
	int nLocalPlayerID = g_EngineClient->GetLocalPlayer();
	C_BasePlayer* player = (C_BasePlayer*)g_EntityList->GetClientEntity(nLocalPlayerID);


	GetClientRect(s_cparams.hFocusWindow, &s_rect);

	x = (s_rect.right / 2);
	y = (s_rect.bottom / 2);

	if (cspread)
	{
		QAngle punchAngle = player->m_viewPunchAngle();
		const float scale = 1.55f;
		int dy = (s_rect.right / 90);
		int dx = (s_rect.bottom / 90);
		x -= (dx*(punchAngle.yaw * scale));
		y += (dy*(punchAngle.pitch * scale));
	}


	switch (ctype)
	{
		case 0:	//Dot
			Draw::FilledSquareWithCenter(lpD3D9Dev, x, y, csize, color);
			break;
		case 1: //Square
			Draw::OutlinedSquareWithCenter(lpD3D9Dev, x, y, csize, thickness, color);
			if(extradot)
				Draw::FilledSquareWithCenter(lpD3D9Dev, x, y, dotscale, color);
			break;
		case 2: //Circle
			Draw::FilledCircle(lpD3D9Dev, x, y, csize, color);
			break;
		case 3: //Outlined circle
			Draw::OutlinedCircle(lpD3D9Dev, x, y, csize, thickness, color);
			if (extradot)
				Draw::FilledSquareWithCenter(lpD3D9Dev, x, y, dotscale, color);
			break;
		case 4: //Cross
			Draw::Cross(lpD3D9Dev, x, y, csize * 2, thickness, color);
			break;
		default:
			__assume(0);
	}
	
}

inline int Sign(double x)
{
	if (x < 0)
		return -1;
	else
		return 1;
}

void __fastcall D3DHookedDraw(D3D9DevPtr lpD3D9Dev)
{
	if (!g_EngineClient->IsTakingScreenshot())
	{
		if (!g_DisableAll)
			DrawCrosshair(lpD3D9Dev);
		DrawMenu(lpD3D9Dev);

		if (InputSys::Get().IsKeyDown(VK_TAB) && g_RevealRanks && !g_DisableAll)
			Utils::RankRevealAll();
	}
}

void __fastcall D3DHookedDrawReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ESP::DestroyFonts();
}