#pragma once

#include <string>
#include "resource.h"

#define IDC_INFOSIGN					1001
#define IDC_INFORMATION					1002
#define IDC_USERIDSIGN					1003
#define IDC_USERID						1004
#define IDC_ACTIVATE					1005

#define IDC_REGSIGN						2001
#define IDC_LOGINSIGN					2002
#define IDC_LOGIN						2003
#define IDC_PASS_SIGN					2004
#define IDC_PASSWORD					2005
#define IDC_EMAILSIGN					2006
#define IDC_EMAIL						2007
#define IDC_ICHECKED					2008
#define IDC_REGISTER					2010
#define IDC_PASSCONFSIGN				2011
#define IDC_PASSCONF					2012

#define BTS_WAITING						0
#define BTS_RETRY						1
#define BTS_REGISTER					2
#define BTS_PLAY						3
#define BTS_EXIT						4
#define BTS_INJECT						5
#define BTS_RESTART						6

#define MAINWND_HEIGHT_MOD				-20

#define REGSTATE_FAILED					0
#define REGSTATE_SUCCESS				1
#define REGSTATE_CONFLICT				2

#define WHVERSION						"1.20"
#define ACTIVATION_HOST					(std::string)"http://tech.zlopochta.ru/shampoo.php"
#define DLL_FILENAME					(std::string)"Comctl86.dll"
#define UPDATER_FILENAME				(std::string)"upd.exe"
#define EYEVMT_FILENAME					(std::string)"eye.vmt"
#define EYEVTF_FILENAME					(std::string)"eye.vtf"
#define SPOTTEDWAV_FILENAME				(std::string)"spotted.wav"

#define SUCCESS_EXIT_CODE				2261794

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RegWndProc(HWND, UINT, WPARAM, LPARAM);
void AddInfoLine(LPCTSTR, BOOL nl = TRUE);
unsigned int __stdcall _BackgroundFunc(LPVOID);
unsigned int __stdcall _RegWndThread(LPVOID);
xstringA GetMAC();
xstringA GetUniqueID(xstringA);
xstringA GetActivationKey(xstringA);
void OpenRegisterWindow(HINSTANCE, HWND, HANDLE);
void ChangeButtonState(int state);
xstringA GetRegistrationKey(xstringA ip, xstringA uid);
void UpdateFunc(HINSTANCE hInst, HWND hWnd, HANDLE hEvent, BOOL retry);
void CleanMessages(const TCHAR* str = _T(""));
void UnpackDLL();
void UnpackUpdater();
BOOL LaunchUpdater(BOOL rd);
void ActivationResponse(bool Success);
std::wstring To_wcs(std::string str);
void RemoveBOMFromFile(LPCTSTR path);

struct WNDThreadData
{
	HINSTANCE hInst;
	HWND hMainWindow;
	HANDLE hEvent;
	BOOL b_b1;
	HANDLE h1;
	bool upd;
	std::string cmd;
	std::string extra;
};

struct BGTData
{
	BOOL retry;
	HINSTANCE hInst;
	HWND hMainWindow;
	HANDLE hEvent;
};