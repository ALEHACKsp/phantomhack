#define ISOLATION_AWARE_ENABLED 1

#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <psapi.h> 
#include <tchar.h>
#include <commctrl.h>
#include <Rpcdce.h>
#include <string>
#include <xstring.h>
#include <atlbase.h>
#include <curl\curl.h>
#include <ctime>

#include "simple.h"
#include "md5.h"
#include "defs.h"
#include "xorkeys.h"


#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Rpcrt4.lib")  

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")

#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Wininet.lib")

#ifdef CURL_USE_SSL

#pragma comment(lib, "libcurlssl.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

#else

#ifndef _DEBUG

#pragma comment(lib, "libcurl.lib") 

#else

#pragma comment(lib, "libcurld.lib") 

#endif

#endif


HWND Info;
HWND hUserID;
HWND hButton;
HANDLE hRegisterEvent;
HANDLE g_hEvent;
HANDLE g_hCSGO = NULL;
xstringA g_UserID;
std::string g_IP;
int g_ButtonState = BTS_WAITING;
INT g_RegSuccess = REGSTATE_FAILED;
int g_RetryCount = 0;
std::string gdp;
BOOL g_Activated = FALSE;
BOOL g_Injected = FALSE;
HANDLE g_InjectEvent = NULL;
int g_csgoProcess = 0;
bool g_bQuit = false;

#define DOWNLOAD_PATH gdp.c_str()

unsigned int __stdcall InjectorRoutine(LPVOID lpParam);
void CheckForUpdates();
void UnpackTextures();
void UnpackSounds();

static size_t WriteCallback(char* buf, size_t size, size_t nmemb, std::string* curl_data)
{
	size_t _i = 0;
	for (size_t c = 0; c < size*nmemb; c++)
	{
		if (_i >= 3)
			curl_data->push_back(buf[c]);
		_i++;
	}
	return size*nmemb; 
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	InitCommonControls();
	bool bUpd = false;
	gdp = GetSysVar("TMP");
	std::string cmd = lpCmdLine;
	std::string dllpath = DOWNLOAD_PATH + DLL_FILENAME;

#ifdef _DEBUG
	MessageBox(NULL, TEXT("Димон, приаттач дебаггер к этому процессу, после чего тыкай ОК. Вот только не жми ОК раньше времени, перезапускать придется."), TEXT("Debug mode"), MB_OK | MB_ICONWARNING);
#endif

	if (cmd.find("-upd") == std::string::npos)
	{
		UnpackDLL();

		CHAR launcher_path[MAX_PATH];
		GetModuleFileNameA(NULL, launcher_path, MAX_PATH);

		g_csgoProcess = InjectDLL("csgo.exe", dllpath.c_str(), &g_hCSGO);
		if (g_csgoProcess > 0)
			g_Injected = TRUE;


		else if (g_csgoProcess == INJDLL_NOTFOUND)
		{
		}
		else
		{
			remove(dllpath.c_str());
			MessageBox(NULL, _T("Ошибка инициализации PhantomHack. Попробуйте еще раз."), _T("Ошибка"), MB_OK | MB_ICONERROR);
			return 2;
		}
	}
	else
		bUpd = true;

	
	MSG msg = { 0 };
	int iGetOk = 0;
	RECT rscreen;
	GetWindowRect(GetDesktopWindow(), &rscreen);
	int x = rscreen.right / 2 - 150;
	int y = rscreen.bottom / 2 - 75;
	g_UserID = GetUniqueID(GetMAC());
	hRegisterEvent = CreateEvent(NULL, FALSE, FALSE, _T("RegEvent"));
	g_hEvent = hRegisterEvent; 

	HWND hWnd = CreateWindowEasy(_T("PrimaryWnd"), &MainWndProc, hInst, _T("PhantomHack"), NULL, x, y, 302, 335 + MAINWND_HEIGHT_MOD, DEFAULT_COLOR, WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, WS_EX_TOPMOST);
	
	ShowWindow(hWnd, TRUE);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Ошибка создания окна."), _T("Ошибка"), MB_OK | MB_ICONERROR);
		return 2;
	}

	CreateWindowElement(hWnd, ET_STATIC, _T("Информация:"), hInst, WS_VISIBLE, NULL, IDC_INFOSIGN, 10, 5, 100, 15);
	Info = CreateWindowElement(hWnd, ET_EDIT, _T(""), hInst, WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, NULL, IDC_INFORMATION, 10, 20, 266, 205 + MAINWND_HEIGHT_MOD);
	CreateWindowElement(hWnd, ET_STATIC, _T("ID:"), hInst, WS_VISIBLE, NULL, IDC_USERIDSIGN, 10, 231 + MAINWND_HEIGHT_MOD, 30, 15);
	hUserID = CreateWindowElement(hWnd, ET_EDIT, _T("..."), hInst, WS_VISIBLE | WS_BORDER| ES_CENTER | ES_READONLY, NULL, IDC_USERID, 28, 230 + MAINWND_HEIGHT_MOD, 248, 18);
	HFONT hFont = CreateFont(13, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
	SendMessage(hUserID, WM_SETFONT, (WPARAM)hFont, TRUE);
	hButton = CreateWindowElement(hWnd, ET_BUTTON, _T("Подождите..."), hInst, WS_VISIBLE | WS_DISABLED | WS_TABSTOP | BS_FLAT, NULL, IDC_ACTIVATE, 9, 252 + MAINWND_HEIGHT_MOD, 268, 35);
	
	SetForegroundWindow(hWnd);
	SetFocus(Info);
	     
	WNDThreadData thrd;
	thrd.cmd = cmd;
	thrd.hMainWindow = hWnd;
	thrd.hInst = hInst;
	thrd.extra = dllpath;
	thrd.upd = bUpd;
	g_InjectEvent = CreateEvent(NULL, FALSE, FALSE, _T("InjectEvent"));

	_beginthreadex(NULL, 0, &InjectorRoutine, reinterpret_cast<LPVOID>(&thrd), 0, NULL);



	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (iGetOk == -1)
			return 3;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

xstringA oshiri(/*std::string pair*/)
{
	DWORD procID = GetCurrentProcessId();
	xstringA str;
	str.Format("%X", procID);
	return str;
}

std::string doge()
{
	std::string hokage;
	char buf[128];
	time_t seconds = time(NULL);
	tm timeinfo, *ti_ptr;
	localtime_s(&timeinfo, &seconds); ti_ptr = &timeinfo;
	strftime(buf, 128, "w53%A%B%d%Y%H%Ms", ti_ptr);
	hokage = md5(md5(buf)) + md5(oshiri());
	return md5(hokage);
}



unsigned int __stdcall InjectorRoutine(LPVOID lpParam)
{
	WNDThreadData* thrd = reinterpret_cast<WNDThreadData*>(lpParam);

	if (thrd->upd)
	{
		AddInfoLine(_T("PhantomHack был успешно обновлён и готов к работе!"), FALSE);
		std::wstring cver_wstr = L"Текущая версия: v";
		cver_wstr.append(_T(WHVERSION));
		AddInfoLine(cver_wstr.c_str());
		AddInfoLine(_T("Можете перезапустить PhantomHack!"));
		ChangeButtonState(BTS_RESTART);
		return 0;
	}

	std::string updpath = DOWNLOAD_PATH + UPDATER_FILENAME;
	remove(updpath.c_str());

	AddInfoLine(_T("PhantomHack запущен!"), FALSE);

	CheckForUpdates();

	if (!g_Injected && !thrd->upd)
	{
		ChangeButtonState(BTS_INJECT);
		AddInfoLine(_T("Игра Counter-Strike: Global Offensive не запущена."));
		AddInfoLine(_T("Запустите игру, дождитесь пока она загрузится и откроется главное меню. Затем нажмите кнопку \"Подключиться к игре\"..."));
	}
	else if (g_Injected && !thrd->upd)
		AddInfoLine(_T("Обнаружен csgo.exe, подготовка хака к работе..."));


	if (!g_Injected)
		WaitForSingleObject(g_InjectEvent, 0xFFFFFFFF);

	UnpackTextures();
	UnpackSounds();

	Sleep(450);

	UpdateFunc(thrd->hInst, thrd->hMainWindow, hRegisterEvent, FALSE);

	WaitForSingleObject(g_hCSGO, INFINITE);
	g_bQuit = true;
	remove(thrd->extra.c_str());
	ExitProcess(0);
	return 0;
}

void ActivationResponse(bool Success)
{
	HANDLE hPipe;
	DWORD dwWritten;

	xstringA buf;

	xstringA left, right, a_begin, a_end, a_delim;
	a_begin = "*#@dsfd@heaven$ad#gsd$";
	a_end = "($@$)chaoticF3";
	a_delim = "ef37fjd10g25fd01x45[chaotic+2]null";
	char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_!##$";
	size_t charslast = strlen(chars) - 1;
	//size_t llen;
	if (Success)
	{
		/*llen = GetRandomBetween(0, 32);
		left = "";

		for (size_t i = 0; i < llen; i++)
			left.push_back(chars[GetRandomBetween(0, charslast)]);*/
		left = doge();
		right = oshiri(/*left*/);
	}
	else
	{
		left = md5("0");
		right = md5("0");
	}

	buf.Format("%s%s%s%s%s", a_begin.GetStr().c_str(), left.GetStr().c_str(), a_delim.GetStr().c_str(), right.GetStr().c_str(), a_end.GetStr().c_str());
	hPipe = CreateFile(TEXT("\\\\.\\pipe\\FESPH"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, _T("Ошибка инициализации PhantomHack. Попробуйте еще раз."), _T("Ошибка"), MB_OK | MB_ICONERROR);
		ExitProcess(0);
		return;
	}

	WriteFile(hPipe, buf.GetStr().c_str(), buf.GetLen() + 1, &dwWritten, NULL);
	CloseHandle(hPipe);
}


void AddInfoLine(LPCTSTR msg_, BOOL nl)
{
	size_t size = lstrlen(msg_) + 1;
	if (nl)
		size += 2;
	LPTSTR msg = new TCHAR[size];

	TCHAR c = msg_[0];
	size_t i = 0;
	size_t d = 0;

	if (nl)
		d = 2;

	if (nl)
	{
		LPCTSTR end = _T("\r\n");
		msg[i] = end[0];
		i++;
		msg[i] = end[1];
		i++;
	}

	while (c != '\0')
	{
		msg[i] = c;
		i++;
		c = msg_[i - d];
	}

	msg[i] = '\0';

	int TextLen = SendMessage(Info, WM_GETTEXTLENGTH, 0, 0);
	SendMessage(Info, EM_SETSEL, (WPARAM)TextLen, (LPARAM)TextLen);
	SendMessage(Info, EM_REPLACESEL, FALSE, (LPARAM)msg);
	delete[] msg;
	UpdateWindow(Info);
}


void __fastcall UpdaterProgress(int per, size_t bytesDownloaded, size_t fullSize)
{
	static bool init = false;
	if (!init)
	{
		EnableWindow(hButton, TRUE);
		init = true;
	}
	
	xstring dstatus;
	size_t kb1, kb2;
	kb1 = bytesDownloaded;
	kb2 = fullSize / 1000;
	if (bytesDownloaded < 1000)
	{
		dstatus.Format(_T("Загружено %i%% (%i Байт из %i КБ)"), per, kb1, kb2);
	}
	else
	{
		kb1 = bytesDownloaded / 1000;
		dstatus.Format( _T("Загружено %i%% (%i КБ из %i КБ)"), per, kb1, kb2);
	}

	SetWindowText(hButton, dstatus.GetStr().c_str());
	UpdateWindow(hButton);
}

void CheckForUpdates()
{
	std::wstring ui;
	std::string current_version = WHVERSION;
	std::wstring cver_wstr = _T(WHVERSION);
	ui = L"Текущая версия: v" + cver_wstr;
	AddInfoLine(ui.c_str());
	AddInfoLine(_T("Проверка на наличие обновлений..."));

	std::string latest_version = HTTPPost(ACTIVATION_HOST, md5("ucheck") + "=" + WHVERSION);
	if (latest_version.c_str() == "0" || latest_version.c_str() == "")
		AddInfoLine(_T("Ошибка проверки обновлений."));
	else if (latest_version == current_version)
	{
		AddInfoLine(_T("Новых обновлений нет.\r\n"));
		AddInfoLine(_T("****************************************"));
	}
	else
	{
		std::wstring wstr2(latest_version.begin(), latest_version.end());
		ui = _T("Ваш PhantomHack устарел! Новая версия: v") + wstr2;
		AddInfoLine(ui.c_str());
		AddInfoLine(_T("Загрузка обновления..."));
		DWORD s_size = atoi(HTTPPost(ACTIVATION_HOST, md5("getsize") + "=" + md5("activator")).c_str());
		//DWORD size = _downloadFile(ACTIVATION_HOST + "?" + md5("getupd") + "=" + g_UserID, DOWNLOAD_PATH + (std::string)"g8h7ther987k8re.tmp", s_size);
		std::string durl = ACTIVATION_HOST + "?" + md5("getupd") + "=" + g_UserID;
		std::string dpath = DOWNLOAD_PATH + (std::string)"g8h7ther987k8re.tmp";

		std::wstring wdurl;
		std::wstring wdpath;

		wdurl = To_wcs(durl);
		wdpath = To_wcs(dpath);

		DeleteUrlCacheEntry(wdurl.c_str());
		CallbackHandler callbackHandler;
		callbackHandler.SetBytesMax(s_size);
		IBindStatusCallback* pBindStatusCallback = NULL;
		callbackHandler.QueryInterface(IID_IBindStatusCallback,
			reinterpret_cast<void**>(&pBindStatusCallback));

		HRESULT hr = URLDownloadToFile(NULL, wdurl.c_str(), wdpath.c_str(), 0, pBindStatusCallback);

		if (s_size == 0 || !SUCCEEDED(hr))
			AddInfoLine(_T("Ошибка загрузки обновления."));
		else
		{
			RemoveBOMFromFile(wdpath.c_str());
			AddInfoLine(_T("Обновление загружено!"));
			Sleep(1500);
			LaunchUpdater(FALSE);
		}
	}
}

xstringA GetMAC()
{
	BYTE MACData[6];
	UUID uuid;
	xstringA r;
	UuidCreateSequential(&uuid);    
	for (int i = 2; i<8; i++)				
		MACData[i - 2] = uuid.Data4[i];
	r.Format("%[6]nrx", MACData);
	return r;
}

xstringA GetUniqueID(xstringA mac)
{
	char d[13];
	char tmp;

	for (size_t i = 0; i < 12; i++)
		d[i] = mac[11 - i];

	tmp  = d[3];
	d[3] = d[7];
	d[7] = tmp;
	
	d[12] = '\0';
	xstringA _uid(md5(d), true);
	return _uid;
}

xstringA GetActivationKey(xstringA uid)
{
	char part1[17], part2[17];
	char mix[33];
	xstringA str(true);

	for (size_t i = 0; i < 16; i++)
		part1[i] = uid[i];

	for (size_t i = 16; i < 32; i++)
		part2[i-16] = uid[i];

	size_t j = 0, k = 0;
	for (size_t i = 0; i < 32; i++)
	{
		if (i % 2 == 0)
		{
			mix[i] = part2[j];
			j++;
		}
		else
		{
			mix[i] = part1[k];
			k++;
		}
	}

	part1[16] = '\0'; part2[16] = '\0'; mix[32] = '\0';

	str  = md5(md5(md5(part1))); //x3 md5
	str += md5(mix);			 //x1 md5
	str += md5(md5(part2));		 //x2 md5

	return str;
}

unsigned int __stdcall _BackgroundFunc(LPVOID lpData)
{	
	BGTData* bgd = reinterpret_cast<BGTData*>(lpData); 
	BOOL retry = bgd->retry;
	HANDLE hEvent = bgd->hEvent;
	HINSTANCE hInst = bgd->hInst;
	HWND hMainWindow = bgd->hMainWindow;
	delete bgd;
	std::string result;
	std::wstring ui;
	if (!retry)
	{
		SetWindowTextA(hUserID, g_UserID.GetStr().c_str());
		UpdateWindow(hUserID);
	}


	ChangeButtonState(BTS_WAITING);
	if (retry)
		g_RetryCount++;

	if (retry == false)
	{
		AddInfoLine(_T("Проверка активации..."));
	}
	else
		AddInfoLine(_T("Попытка проверки активации..."));

	std::string postargs = md5("getip") + "=1";
	g_IP = HTTPPost(ACTIVATION_HOST, postargs); 
	//result = HTTPGet(ACTIVATION_HOST + "?" + md5("uid") + "=" + g_UserID);
	postargs = md5("uid") + "=" + g_UserID;
	result = HTTPPost(ACTIVATION_HOST, postargs); 
	if (result == "1")
	{
		AddInfoLine(_T("Вы не зарегистрированы, зарегистрируйтесь...")); 
		OpenRegisterWindow(hInst, hMainWindow, hEvent); 
		WaitForSingleObject(hRegisterEvent, INFINITE);
		if(g_RegSuccess == REGSTATE_SUCCESS)
			AddInfoLine(_T("Успешная регистрация!"));
		else if (g_RegSuccess == REGSTATE_CONFLICT)
		{
			AddInfoLine(_T("Невозможно зарегистрироваться. Этот ID оборудования уже привязан к существующей учетной записи."));
			ChangeButtonState(BTS_EXIT);
			return 0;
		}
		else
		AddInfoLine(_T("Ошибка регистрации! Повторная попытка..."));
			UpdateFunc(hInst, hMainWindow, hEvent, TRUE);
		return 0;
	}
	else if (result == "2")
	{
		std::string username = HTTPPost(ACTIVATION_HOST, md5("getusr") + "=" + g_UserID);
		std::wstring wstr(username.begin(), username.end());
		ui = _T("Ваше имя пользователя: ") + wstr;
		AddInfoLine(ui.c_str());
		AddInfoLine(_T("Ваш PhantomHack не активирован, обратитесь к продавцу и повторите проверку активации."));
		ChangeButtonState(BTS_RETRY);
		return 0;
	}
	else if (result == "3")
	{
		std::string username = HTTPPost(ACTIVATION_HOST, md5("getusr") + "=" + g_UserID);
		std::wstring wstr(username.begin(), username.end());
		ui = _T("Ваше имя пользователя: ") + wstr;
		AddInfoLine(ui.c_str());
		AddInfoLine(_T("Ваш PhantomHack не активирован и подлежит удалению с вашего ПК за неуплату. Вас предупреждали. "));
		LaunchUpdater(TRUE);
		ChangeButtonState(BTS_EXIT);
		return 0;
	}
	else if (result.length() == 96)
	{
		if (result == GetActivationKey(g_UserID))
		{
			std::string username = HTTPPost(ACTIVATION_HOST, md5("getusr") + "=" + g_UserID);
			std::wstring wstr(username.begin(), username.end());
			ui = _T("Ваше имя пользователя: ") + wstr;
			AddInfoLine(ui.c_str());
			AddInfoLine(_T("Успешная активация!"));
		}
		else
		{
			AddInfoLine(_T("Ошибка проверки активации."));
			ChangeButtonState(BTS_EXIT);
			return 0;
		}
	}
	else
	{
		AddInfoLine(_T("Ошибка сервера активации."));
		ChangeButtonState(BTS_RETRY);
		return 0;
	}

	
	g_Activated = TRUE;
	AddInfoLine(_T("****************************************"));
	AddInfoLine(_T("Готово к игре!"));
	AddInfoLine(_T("****************************************"));
	AddInfoLine(_T("Как пользоваться:"));
	AddInfoLine(_T("-- Клавиша INSERT --"));
	AddInfoLine(_T("После того как вы нажмете здесь Играть, нажмите INSERT в игре для подробного меню."));
	AddInfoLine(_T(""));
	AddInfoLine(_T(""));
	AddInfoLine(_T(""));
	AddInfoLine(_T("Помните, что если вы будете злоупотреблять - вы можете быть забанены патрулем!"));
	AddInfoLine(_T("****************************************"));
	AddInfoLine(_T("Нажмите Играть и наслаждайтесь!"));

	ChangeButtonState(BTS_PLAY); 

	return 0;
}

void CleanMessages(const TCHAR* str)
{
	SetWindowText(Info, str);
}

void ChangeButtonState(int state)
{
	if (g_ButtonState == state)
		return;
	switch (state)
	{
		case BTS_WAITING:
			EnableWindow(hButton, FALSE);
			SetWindowText(hButton, _T("Подождите..."));
			g_ButtonState = state;
			break;

		case BTS_RETRY:
			EnableWindow(hButton, TRUE);
			SetWindowText(hButton, _T("Подключиться еще раз"));
			g_ButtonState = state;
			break;

		case BTS_REGISTER:
			EnableWindow(hButton, TRUE);
			SetWindowText(hButton, _T("Регистрация"));
			g_ButtonState = state;
			break;

		case BTS_PLAY:
			EnableWindow(hButton, TRUE);
			SetWindowText(hButton, _T("Играть"));
			g_ButtonState = state;
			break;

		case BTS_EXIT:
			EnableWindow(hButton, TRUE);
			SetWindowText(hButton, _T("Выход"));
			g_ButtonState = state;
			break;
		case BTS_INJECT:
			EnableWindow(hButton, TRUE);
			SetWindowText(hButton, _T("Подключиться к игре"));
			g_ButtonState = state;
			break;
		case BTS_RESTART:
			EnableWindow(hButton, TRUE);
			SetWindowText(hButton, _T("Перезапустить"));
			g_ButtonState = state;
			break;

		default:
			EnableWindow(hButton, FALSE);
			SetWindowText(hButton, _T("Подождите..."));
			g_ButtonState = BTS_WAITING;
	}
}


void OpenRegisterWindow(HINSTANCE hInst, HWND hWnd, HANDLE ev)
{
	WNDThreadData* wtd = new WNDThreadData;
	wtd->hInst = hInst;
	wtd->hMainWindow = hWnd;
	wtd->hEvent = ev;
	_beginthreadex(NULL, 0, &_RegWndThread, reinterpret_cast<LPVOID>(wtd), 0, NULL);
}

unsigned int __stdcall _RegWndThread(LPVOID lpData)
{
	WNDThreadData* wtd = reinterpret_cast<WNDThreadData*>(lpData);
	HWND hMainWindow = wtd->hMainWindow;
	HINSTANCE hInst = wtd->hInst;
	HANDLE hEvent = wtd->hEvent;
	delete wtd;
	EnableWindow(hMainWindow, FALSE);
	MSG msg = { 0 };
	int iGetOk = 0;
	RECT rscreen;
	GetWindowRect(GetDesktopWindow(), &rscreen);
	int x = rscreen.right / 2 - 150 +50;
	int y = rscreen.bottom / 2 - 75 + 50; 

	std::wstring classname = _T("NewUserWnd");
	TCHAR cnum[2];
	cnum[0] = static_cast<TCHAR>(g_RetryCount);
	cnum[1] = '\0';
	classname += cnum;
	
	HWND hWnd = CreateWindowEasy(classname.c_str(), &RegWndProc, hInst, _T("Регистрация пользователя"), NULL, x, y, 350, 248, DEFAULT_COLOR, WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_POPUPWINDOW, WS_EX_PALETTEWINDOW);
	ShowWindow(hWnd, TRUE);
	EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	if (!hWnd)
	{
		MessageBox(NULL, _T("Ошибка создания окна."), _T("Ошибка"), MB_OK | MB_ICONERROR);
		return 2;
	}
	HWND hSign = CreateWindowElement(hWnd, ET_STATIC, _T("Это первая попытка запуска PhantomHack с этого ПК.\r\nНеобходимо для привязки PhantomHack к вашему ПК.\r\nПожалуйста зарегистрируйтесь..."), hInst, WS_VISIBLE, NULL, IDC_REGSIGN, 10, 5, 300, 45);
	LONG style = GetWindowLong(hSign, GWL_STYLE);
	style &= ~SS_SIMPLE;
	SetWindowLong(hSign, GWL_STYLE, style);
	UpdateWindow(hSign);
	CreateWindowElement(hWnd, ET_STATIC, _T("Введите логин:"), hInst, WS_VISIBLE, NULL, IDC_LOGINSIGN, 10, 50, 80, 15);
	CreateWindowElement(hWnd, ET_EDIT, _T(""), hInst, WS_VISIBLE | WS_BORDER, NULL, IDC_LOGIN, 10, 65, 150, 20);
	CreateWindowElement(hWnd, ET_STATIC, _T("Введите e-mail:"), hInst, WS_VISIBLE, NULL, IDC_EMAILSIGN, 174, 50, 80, 15);
	CreateWindowElement(hWnd, ET_EDIT, _T(""), hInst, WS_VISIBLE | WS_BORDER, NULL, IDC_EMAIL, 174, 65, 150, 20);
	CreateWindowElement(hWnd, ET_STATIC, _T("Введите пароль:"), hInst, WS_VISIBLE, NULL, IDC_PASS_SIGN, 10, 90, 120, 15);
	CreateWindowElement(hWnd, ET_EDIT, _T(""), hInst, WS_VISIBLE | WS_BORDER | ES_PASSWORD, NULL, IDC_PASSWORD, 10, 105, 150, 20);
	CreateWindowElement(hWnd, ET_STATIC, _T("Повторите пароль:"), hInst, WS_VISIBLE, NULL, IDC_PASSCONFSIGN, 174, 90, 120, 15);
	CreateWindowElement(hWnd, ET_EDIT, _T(""), hInst, WS_VISIBLE | WS_BORDER | ES_PASSWORD, NULL, IDC_PASSCONF, 174, 105, 150, 20);
	CreateWindowElement(hWnd, ET_CHECKBOX, _T("Я заполнил все поля верно"), hInst, WS_VISIBLE, NULL, IDC_ICHECKED, 10, 135, 175, 20);
	CreateWindowElement(hWnd, ET_BUTTON, _T("Зарегистрироваться"), hInst, WS_VISIBLE | WS_DISABLED | WS_TABSTOP | BS_FLAT, NULL, IDC_REGISTER, 9, 160, 316, 40);
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (iGetOk == -1)
			return 3;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	PulseEvent(hRegisterEvent);
	EnableWindow(hMainWindow, TRUE);
	SetForegroundWindow(hMainWindow);
	return msg.wParam;
}


xstringA GetRegistrationKey(xstringA ip, xstringA uid)
{
	xstringA str = ip + uid;
	str = md5(str) + md5(md5(uid));
	str = md5(str);
	return str;
}

void UpdateFunc(HINSTANCE hInst, HWND hWnd, HANDLE hEvent, BOOL retry)
{
	BGTData* bgd = new BGTData; 
	bgd->retry = retry;
	bgd->hInst = hInst;
	bgd->hMainWindow = hWnd;
	bgd->hEvent = hEvent;
	_beginthreadex(NULL, 0, &_BackgroundFunc, reinterpret_cast<LPVOID>(bgd), 0, NULL);
}

void UnpackDLL()
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_DATA1), _T("DATA")); 
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	DWORD dwSize = SizeofResource(hModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);

	BYTE* bytes = new BYTE[dwSize];
	memcpy(bytes, lpAddress, dwSize);


	size_t rbs = GetRandomBetween(1, _DllXorKeySize);
	size_t file_size = dwSize + rbs;
	BYTE* decrypted_buffer = new BYTE[file_size];

	size_t i = 0, k = 0, kk = 1;
	while(i < dwSize)
	{
		decrypted_buffer[i] = bytes[i] ^ _DllXorKey[k];
		k++;
		if (k == _DllXorKeySize)
			k = 0;

		decrypted_buffer[i] = decrypted_buffer[i] ^ _DllXorKey[kk];
		kk++;
		if (kk == _DllXorKeySize)
			kk = 0;

		i++;
	}

	BYTE rb;
	while (i < file_size)
	{
		rb = GetRandomBetween(0, 255);
		decrypted_buffer[i] = rb;
		i++;
	}

	delete[] bytes;

	DWORD written = 0;
	std::string dllpath = DOWNLOAD_PATH + DLL_FILENAME;
	HANDLE hFile = CreateFileA(dllpath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	WriteFile(hFile, decrypted_buffer, file_size, &written, NULL);
	CloseHandle(hFile);

	delete[] decrypted_buffer;
}

void UnpackUpdater()
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_DATA2), _T("DATA"));
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	DWORD dwSize = SizeofResource(hModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);

	BYTE* bytes = new BYTE[dwSize];
	memcpy(bytes, lpAddress, dwSize);


	BYTE* decrypted_buffer = new BYTE[dwSize];

	size_t i = 0, k = 0, kk = 1;
	while (i < dwSize)
	{
		decrypted_buffer[i] = bytes[i] ^ _UpdXorKey[k];
		k++;
		if (k == _UpdXorKeySize)
			k = 0;

		decrypted_buffer[i] = decrypted_buffer[i] ^ _UpdXorKey[kk];
		kk++;
		if (kk == _UpdXorKeySize)
			kk = 0;

		i++;
	}

	delete[] bytes;

	DWORD written = 0;
	std::string updpath = DOWNLOAD_PATH + UPDATER_FILENAME;
	HANDLE hFile = CreateFileA(updpath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	WriteFile(hFile, decrypted_buffer, dwSize, &written, NULL);
	CloseHandle(hFile);

	delete[] decrypted_buffer;
}

xstring GetTexturesPath()
{
	TCHAR filename[MAX_PATH];
	if (GetModuleFileNameEx(g_hCSGO, NULL, filename, MAX_PATH) == 0)
	{
		MessageBox(NULL, TEXT("Не удается подгрузить необходимые ресурсы."), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		exit(0);
		return _T("");
	}
	else
	{
		size_t _off = 0, cutindex = 0;
		std::wstring fname = filename;
		while (true)
		{
			if (fname.find(L"\\", _off) != std::wstring::npos)
			{
				cutindex = fname.find(L"\\", _off);
				_off = cutindex + 1;
			}
			else
				break;
		}
		fname = fname.substr(0, _off);
		xstring xpath = fname + L"csgo\\materials\\";
		CreateDirectory(xpath.GetStr().c_str(), NULL);
		xpath +=L"vgui\\";
		CreateDirectory(xpath.GetStr().c_str(), NULL);
		xpath += L"hud\\";
		CreateDirectory(xpath.GetStr().c_str(), NULL);
		return xpath;
	}
}

xstring GetSoundsPath()
{
	TCHAR filename[MAX_PATH];
	if (GetModuleFileNameEx(g_hCSGO, NULL, filename, MAX_PATH) == 0)
	{
		MessageBox(NULL, TEXT("Не удается подгрузить необходимые ресурсы."), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		exit(0);
		return _T("");
	}
	else
	{
		size_t _off = 0, cutindex = 0;
		std::wstring fname = filename;
		while (true)
		{
			if (fname.find(L"\\", _off) != std::wstring::npos)
			{
				cutindex = fname.find(L"\\", _off);
				_off = cutindex + 1;
			}
			else
				break;
		}
		fname = fname.substr(0, _off);
		xstring xpath = fname + L"csgo\\sound\\"; 
		CreateDirectory(xpath.GetStr().c_str(), NULL);
		xpath += L"UI\\"; 
		CreateDirectory(xpath.GetStr().c_str(), NULL);
		return xpath;
	}
}

void UnpackTextures()
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_DATA3), _T("DATA"));
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	DWORD dwSize = SizeofResource(hModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);
	xstring path = GetTexturesPath();

	BYTE* bytes = new BYTE[dwSize];
	memcpy(bytes, lpAddress, dwSize);

	BYTE* decrypted_buffer = new BYTE[dwSize];

	size_t i = 0, k = 0, kk = 1;
	while (i < dwSize)
	{
		decrypted_buffer[i] = bytes[i] ^ _eyeVtfKey[k];
		k++;
		if (k == _eyeVtfKeySize)
			k = 0;

		decrypted_buffer[i] = decrypted_buffer[i] ^ _eyeVtfKey[kk];
		kk++;
		if (kk == _eyeVtfKeySize)
			kk = 0;

		i++;
	}

	delete[] bytes;

	DWORD written = 0;
	path = GetTexturesPath();
	std::string filepath = path.GetStr_a() + EYEVTF_FILENAME;
	remove(filepath.c_str());
	HANDLE hFile = CreateFileA(filepath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, decrypted_buffer, dwSize, &written, NULL);
	CloseHandle(hFile);

	delete[] decrypted_buffer;

	hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_DATA4), _T("DATA"));
	hMemory = LoadResource(hModule, hResource);
	dwSize = SizeofResource(hModule, hResource);
	lpAddress = LockResource(hMemory);

	bytes = new BYTE[dwSize];
	memcpy(bytes, lpAddress, dwSize);

	decrypted_buffer = new BYTE[dwSize];

	i = 0; k = 0; kk = 1;
	while (i < dwSize)
	{
		decrypted_buffer[i] = bytes[i] ^ _eyeVmtKey[k];
		k++;
		if (k == _eyeVmtKeySize)
			k = 0;

		decrypted_buffer[i] = decrypted_buffer[i] ^ _eyeVmtKey[kk];
		kk++;
		if (kk == _eyeVmtKeySize)
			kk = 0;

		i++;
	}

	delete[] bytes;

	written = 0;
	filepath = path.GetStr_a() + EYEVMT_FILENAME;
	remove(filepath.c_str());
	hFile = CreateFileA(filepath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, decrypted_buffer, dwSize, &written, NULL);
	CloseHandle(hFile);

	delete[] decrypted_buffer;
}

void UnpackSounds()
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_DATA5), _T("DATA"));
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	DWORD dwSize = SizeofResource(hModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);
	xstring path = GetSoundsPath();

	BYTE* bytes = new BYTE[dwSize];
	memcpy(bytes, lpAddress, dwSize);



	BYTE* decrypted_buffer = new BYTE[dwSize];

	size_t i = 0, k = 0, kk = 1;
	while (i < dwSize)
	{
		decrypted_buffer[i] = bytes[i] ^ _sndKey[k];
		k++;
		if (k == _sndKeySize)
			k = 0;

		decrypted_buffer[i] = decrypted_buffer[i] ^ _sndKey[kk];
		kk++;
		if (kk == _sndKeySize)
			kk = 0;

		i++;
	}

	delete[] bytes;

	DWORD written = 0;
	std::string filepath = path.GetStr_a() + SPOTTEDWAV_FILENAME;
	remove(filepath.c_str());
	HANDLE hFile = CreateFileA(filepath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, decrypted_buffer, dwSize, &written, NULL);
	CloseHandle(hFile);

	delete[] decrypted_buffer;
}

BOOL LaunchUpdater(BOOL rd)
{
	UnpackUpdater();
	std::string dllpath = DOWNLOAD_PATH + DLL_FILENAME;
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	char FullPath[MAX_PATH];
	GetModuleFileNameA(NULL, FullPath, MAX_PATH);
	std::string upd_arg = DOWNLOAD_PATH + UPDATER_FILENAME;
	upd_arg += " ";
	upd_arg += "#";
	upd_arg += FullPath;
	upd_arg += "#";
	if(rd)
		upd_arg += " #rd";

	Sleep(1000);
	/*HANDLE hCSGO = GetProcessByName("csgo.exe");
	if (hCSGO)
		TerminateProcess(hCSGO, 0);*/


	BOOL bSuccess = CreateProcessA(NULL, const_cast<LPSTR>(upd_arg.c_str()), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	exit(SUCCESS_EXIT_CODE);
	return bSuccess;
}

void SetMainButtonText(LPCTSTR text)
{
	SetWindowText(hButton, text);
	UpdateWindow(hButton);
}
