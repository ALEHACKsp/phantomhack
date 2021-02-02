#include "MemMgr.h"
#include "offsets.h"
#include "imgui\imgui.h"
#include "imgui\imgui_internal.h"
#include "imgui\dx9\imgui_impl_dx9.h"
#include "globals.h"

#include "d3d.h"


void __fastcall D3DHookedDraw(D3D9DevPtr lpD3D9Dev);
//void __fastcall _whdip(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
//void __fastcall _whEndScene(LPDIRECT3DDEVICE9 pDev);
//void __fastcall _whReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
void __fastcall D3DHookedDrawReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);


pReset oReset = 0;		
pDrawIndexedPrimitive oDrawIndexedPrimitive = 0;	
pEndScene oEndScene = 0;		


HRESULT WINAPI hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	//_whdip(pDev, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	return oDrawIndexedPrimitive(pDev, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDev)	
{
	D3DHookedDraw(pDev);
	//_whEndScene(pDev);	
	return oEndScene(pDev);
}

HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//_whReset(pDevice, pPresentationParameters);
	D3DHookedDrawReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto ret = oReset(pDevice, pPresentationParameters);
	if (ret >= 0)
	{
		ImGui_ImplDX9_CreateDeviceObjects();
		ESP::CreateFonts(g_Options.esp_font_size);
	}
	return ret;
}

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
		return E_FAIL;

	WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
		| (WORD)(((colour32 >> 20) & 0xF) << 8)
		| (WORD)(((colour32 >> 12) & 0xF) << 4)
		| (WORD)(((colour32 >> 4) & 0xF) << 0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for (int xy = 0; xy < 8 * 8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}