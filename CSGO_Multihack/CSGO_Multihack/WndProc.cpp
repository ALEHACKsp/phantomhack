#include <windows.h>
#include "imgui\imgui.h"
#include "imgui\imgui_internal.h"
#include "imgui\dx9\imgui_impl_dx9.h"
#include <xstring.h>
#include "sdk\include\csgosdk.h"
#include "globals.h"

extern WNDPROC oWndProc;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline LONG VKToLParam(DWORD vkCode)
{
	UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
	LONG lParamValue = (scanCode << 16);
	return lParamValue;
}

LRESULT __stdcall hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	TCHAR lpBuf[32];
	switch (uMsg)	
	{
	case WM_KEYDOWN:
		if (wParam == VK_INSERT && !g_bKeyEditMode && !g_Options.esp_zoom_key_edit_mode)
		{ 
			WaitForSingleObject(hMenuMutex, INFINITE);
			if (g_bMenuVisible)
				g_bMenuVisible = false;
			else
				g_bMenuVisible = true;
			ReleaseMutex(hMenuMutex);
		}
		else if (g_bKeyEditMode)
		{
			g_bKeyEditMode = false;
			GetKeyNameText(VKToLParam(wParam), lpBuf, 32);
			WaitForSingleObject(hTriggerHotkeyMutex, INFINITE);
			g_TrigKeyText = lpBuf;
			g_TriggerHotkey = wParam;
			ReleaseMutex(hTriggerHotkeyMutex);	
		}
		else if (g_Options.esp_zoom_key_edit_mode)
		{
			g_Options.esp_zoom_key_edit_mode = false;
			GetKeyNameText(VKToLParam(wParam), lpBuf, 32);
			WaitForSingleObject(g_Options.esp_zoom_key_mutex, INFINITE);
			g_Options.esp_zoom_key_text.Format("%zs.w", lpBuf);
			g_Options.esp_zoom_key = wParam;
			ReleaseMutex(g_Options.esp_zoom_key_mutex);
		}
		else if (g_Options.ab_key_edit_mode)
		{
			g_Options.ab_key_edit_mode = false;
			GetKeyNameText(VKToLParam(wParam), lpBuf, 32);
			WaitForSingleObject(g_Options.ab_key_mutex, INFINITE);
			g_Options.ab_key_text.Format("%zs.w", lpBuf);
			g_Options.ab_hotkey = wParam;
			ReleaseMutex(g_Options.ab_key_mutex);
		}
		else if (g_bAlertKeyEditMode)
		{
			g_bAlertKeyEditMode = false;
			GetKeyNameText(VKToLParam(wParam), lpBuf, 32);
			WaitForSingleObject(g_hAlertKeyMutex, INFINITE);
			g_sAlertKeyText.Format("%zs.w", lpBuf);
			g_dwAlertKey = wParam;
			ReleaseMutex(g_hAlertKeyMutex);
		}
		break;
	case WM_SYSKEYDOWN:
		if (g_bKeyEditMode)
		{
			g_bKeyEditMode = false;
			GetKeyNameText(VKToLParam(VK_ALT), lpBuf, 32);
			WaitForSingleObject(hTriggerHotkeyMutex, INFINITE);
			g_TrigKeyText = lpBuf;
			g_TriggerHotkey = VK_ALT;
			ReleaseMutex(hTriggerHotkeyMutex);
		}
		else if (g_Options.esp_zoom_key_edit_mode)
		{
			g_Options.esp_zoom_key_edit_mode = false;
			GetKeyNameText(VKToLParam(VK_ALT), lpBuf, 32);
			WaitForSingleObject(g_Options.esp_zoom_key_mutex, INFINITE);
			g_Options.esp_zoom_key_text.Format("%zs.w", lpBuf);
			g_Options.esp_zoom_key = VK_ALT;
			ReleaseMutex(g_Options.esp_zoom_key_mutex);
		}
		else if (g_Options.ab_key_edit_mode)
		{
			g_Options.ab_key_edit_mode = false;
			GetKeyNameText(VKToLParam(VK_ALT), lpBuf, 32);
			WaitForSingleObject(g_Options.ab_key_mutex, INFINITE);
			g_Options.ab_key_text.Format("%zs.w", lpBuf);
			g_Options.ab_hotkey = VK_ALT;
			ReleaseMutex(g_Options.ab_key_mutex);
		}
		else if (g_bAlertKeyEditMode)
		{
			g_bAlertKeyEditMode = false;
			GetKeyNameText(VKToLParam(VK_ALT), lpBuf, 32);
			WaitForSingleObject(g_hAlertKeyMutex, INFINITE);
			g_sAlertKeyText.Format("%zs.w", lpBuf);
			g_dwAlertKey = VK_ALT;
			ReleaseMutex(g_hAlertKeyMutex);
		}

		break;
	}

	if (g_Options.esp_zoom_key_edit_mode)
	{
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			g_Options.esp_zoom_key_edit_mode = false;
			WaitForSingleObject(g_Options.esp_zoom_key_mutex, INFINITE);
			g_Options.esp_zoom_key_text = "RMB";
			g_Options.esp_zoom_key = VK_RBUTTON;
			ReleaseMutex(g_Options.esp_zoom_key_mutex);
		}
		else if (GetAsyncKeyState(VK_MBUTTON))
		{
			g_Options.esp_zoom_key_edit_mode = false;
			WaitForSingleObject(g_Options.esp_zoom_key_mutex, INFINITE);
			g_Options.esp_zoom_key_text = "MMB";
			g_Options.esp_zoom_key = VK_MBUTTON;
			ReleaseMutex(g_Options.esp_zoom_key_mutex);
		}


	}
	else if (g_Options.ab_key_edit_mode)
	{
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			g_Options.ab_key_edit_mode = false;
			WaitForSingleObject(g_Options.ab_key_mutex, INFINITE);
			g_Options.ab_key_text = "RMB";
			g_Options.ab_hotkey = VK_RBUTTON;
			ReleaseMutex(g_Options.ab_key_mutex);
		}
		else if (GetAsyncKeyState(VK_MBUTTON))
		{
			g_Options.ab_key_edit_mode = false;
			WaitForSingleObject(g_Options.ab_key_mutex, INFINITE);
			g_Options.ab_key_text = "MMB";
			g_Options.ab_hotkey = VK_MBUTTON;
			ReleaseMutex(g_Options.ab_key_mutex);
		}
	}
	else if (g_bKeyEditMode)
	{
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			g_bKeyEditMode = false;
			WaitForSingleObject(hTriggerHotkeyMutex, INFINITE);
			g_TrigKeyText = L"RMB";
			g_TriggerHotkey = VK_RBUTTON;
			ReleaseMutex(hTriggerHotkeyMutex);
		}
		else if (GetAsyncKeyState(VK_MBUTTON))
		{
			g_bKeyEditMode = false;
			WaitForSingleObject(hTriggerHotkeyMutex, INFINITE);
			g_TrigKeyText = L"MMB";
			g_TriggerHotkey = VK_MBUTTON;
			ReleaseMutex(hTriggerHotkeyMutex);
		}
	}
	else if (g_bAlertKeyEditMode)
	{
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			g_bAlertKeyEditMode = false;
			WaitForSingleObject(g_hAlertKeyMutex, INFINITE);
			g_sAlertKeyText = "RMB";
			g_dwAlertKey = VK_RBUTTON;
			ReleaseMutex(g_hAlertKeyMutex);
		}
		else if (GetAsyncKeyState(VK_MBUTTON))
		{
			g_bAlertKeyEditMode = false;
			WaitForSingleObject(g_hAlertKeyMutex, INFINITE);
			g_sAlertKeyText = "MMB";
			g_dwAlertKey = VK_MBUTTON;
			ReleaseMutex(g_hAlertKeyMutex);
		}
	}
	
	WaitForSingleObject(hMenuMutex, INFINITE);
	if (g_bMenuVisible)
	{
		if (ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam) && g_bMenuVisible)
			ReleaseMutex(hMenuMutex);		
	}
	ReleaseMutex(hMenuMutex);

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
