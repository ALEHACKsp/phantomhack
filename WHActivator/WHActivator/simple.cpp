#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <commctrl.h>
#include <richedit.h>
#include <string>
#include "simple.h"
#include <curl\curl.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <random>

#pragma comment(lib, "Shlwapi.lib")

std::default_random_engine generator;
bool rinit = false;

HWND __fastcall CreateWindowEasy(const TCHAR* ClassName, WNDPROC WndProc, HINSTANCE hInst, const TCHAR* Title, HWND Parent, INT pos_x, INT pos_y,
	INT Width, INT Height, HBRUSH Color, DWORD WindowStyle, DWORD WindowStyleEx, UINT ClassStyle)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = ClassStyle;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = DLGWINDOWEXTRA;
	wndClass.hInstance = hInst;
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = Color;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = ClassName;
	wndClass.hIconSm = NULL;

	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, _T("Ошибка инициализации класса окна."), _T("Ошибка"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	return CreateWindowEx(WindowStyleEx, ClassName, Title, WindowStyle, pos_x, pos_y, Width, Height, Parent, NULL, hInst, NULL);
}

HWND __fastcall CreateWindowElement(HWND Parent, UINT Type, const TCHAR* Title, HINSTANCE hInst, DWORD Style, DWORD StyleEx, UINT ElementID, INT pos_x, INT pos_y, INT Width, INT Height, BOOL NewRadioGroup)
{
	HWND hWnd = NULL;
	TCHAR* ClassName = nullptr;

	Style |= WS_CHILD;

	switch (Type)
	{
	case ET_STATIC:
		ClassName = _T("STATIC");
		break;

	case ET_BUTTON:
		ClassName = _T("BUTTON");
		break;

	case ET_EDIT:
		ClassName = _T("EDIT");
		break;

	case ET_COMBOBOX:
		ClassName = _T("COMBOBOX");
		break;

	case ET_LISTBOX:
		ClassName = _T("LISTBOX");
		break;

	case ET_MDICLIENT:
		ClassName = _T("MDICLIENT");
		break;

	case ET_SCROLLBAR:
		ClassName = _T("SCROLLBAR");
		break;

	case ET_CHECKBOX:
		ClassName = _T("BUTTON");
		Style |= BS_AUTOCHECKBOX;
		break;

	case ET_RADIOBUTTON:
		ClassName = _T("BUTTON");
		Style |= BS_AUTORADIOBUTTON;
		if (NewRadioGroup)
			Style |= WS_GROUP;
		break;

	case ET_GROUPBOX:
		ClassName = _T("BUTTON");
		Style |= BS_GROUPBOX;
		break;

	case ET_PROGRESS:
		ClassName = PROGRESS_CLASS;
		break;

	case ET_SLIDER_H:
		ClassName = TRACKBAR_CLASS;
		Style |= TBS_HORZ;
		break;
	case ET_SLIDER_V:
		ClassName = TRACKBAR_CLASS;
		Style |= TBS_VERT;
		break;

	default: ClassName = _T("STATIC");
	}

	hWnd = CreateWindowEx(StyleEx, ClassName, Title, Style, pos_x, pos_y, Width, Height, Parent, (HMENU)ElementID, hInst, NULL);

	if (!hWnd)
		return hWnd;


	if (NewRadioGroup)
		CheckDlgButton(Parent, ElementID, BST_CHECKED);

	HFONT hFont = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	return hWnd;
}

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

static size_t WriteBinaryCallback(BYTE* buf, size_t size, size_t nmemb, CURL_BIN_BUFFER* cfb)
{
	size_t _i = 0;
	for (size_t c = 0; c < size*nmemb; c++)
	{
		if (!cfb->init)
		{
			if (_i >= 3)
				cfb->push(buf[c]);
			_i++;
		}
		else
			cfb->push(buf[c]);

		if (cfb->max > 0)
		{
			int per = (int)floor(((float)cfb->size / (float)cfb->max) * 100.0f);
			UpdaterProgress(per, cfb->size, cfb->max);
		}
	}
	return size*nmemb;
}

std::string HTTPGet(std::string request)
{
	CURL *curl;
	CURLcode res;
	std::string result;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	return result;
}

std::string HTTPPost(std::string url, std::string fields)
{
	CURL *curl;
	CURLcode res;
	std::string result;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	return result;
}

DWORD DownloadFile(std::string url, std::string save_path)
{
	CURL *curl;
	CURLcode res;
	CURL_BIN_BUFFER* result = new CURL_BIN_BUFFER;
	DWORD written = 0;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBinaryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		HANDLE hFile = CreateFileA(save_path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#ifndef _CBB_USE_STACK
		WriteFile(hFile, result->Buffer, result->size, &written, NULL);
#else
		WriteFile(hFile, result->Buf._Get_data()._Myfirst, result->size, &written, NULL);
#endif
		CloseHandle(hFile);
	}

	delete result;
	return written;
}

DWORD _downloadFile(std::string url, std::string save_path, size_t maxsize)	//Curl implementation. (Very low speed, deprecated)
{
	CURL *curl;
	CURLcode res;
	CURL_BIN_BUFFER* result = new CURL_BIN_BUFFER;
	result->max = maxsize;
	DWORD written = 0;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBinaryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl); 

		HANDLE hFile = CreateFileA(save_path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#ifndef _CBB_USE_STACK
		WriteFile(hFile, result->Buffer, result->size, &written, NULL);
#else
		WriteFile(hFile, result->Buf._Get_data()._Myfirst, result->size, &written, NULL);
#endif
		CloseHandle(hFile);
	}

	delete result;
	return written;
}

std::string wtoa_std(std::wstring wstr)
{
	std::string result;
	size_t size = wstr.length() + 1;
	char* tmp = new char[size];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), tmp, size, NULL, NULL);
	tmp[size - 1] = '\0';
	result = tmp;
	delete[] tmp;
	return result;
}

std::wstring atow_std(std::string str)
{
	std::wstring result;
	size_t size = str.length() + 1;
	wchar_t* tmp = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), tmp, size);
	tmp[size - 1] = '\0';
	result = tmp;
	delete[] tmp;
	return result;
}

int InjectDLL(LPCSTR processName, LPCSTR dllName, HANDLE* pHandle)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
			
		{
			if (_stricmp(wtoa_std(entry.szExeFile).c_str(), processName) == 0)
			{
				
				HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
				if (h)
				{
					LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
					LPVOID dereercomp = VirtualAllocEx(h, NULL, strlen(dllName), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
					WriteProcessMemory(h, dereercomp, dllName, strlen(dllName), NULL);
					HANDLE asdc = CreateRemoteThread(h, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
					WaitForSingleObject(asdc, INFINITE);
					VirtualFreeEx(h, dereercomp, strlen(dllName), MEM_RELEASE);
					CloseHandle(asdc);
					if(pHandle)
						*pHandle = h;
					else
						CloseHandle(h);
					CloseHandle(snapshot);
					return entry.th32ProcessID;
				}
				else
				{
					CloseHandle(snapshot);
					return INJDLL_FAILED;
				}
			}
		}
	}
	
	CloseHandle(snapshot);
	return INJDLL_NOTFOUND;

}

std::string GetCurrentAppPath()
{
	std::string str;
	char FullPath[MAX_PATH];
	GetModuleFileNameA(NULL, FullPath, MAX_PATH);
	str = FullPath;
	char* FileName = PathFindFileNameA(FullPath);
	size_t i = str.find(FileName);
	str.erase(i, str.length() - 1);
	return str;
}

int GetRandomBetween(int a, int b)
{
	if (!rinit)
	{
		generator.seed((UINT)time(NULL));
		rinit = true;
	}
	std::uniform_int_distribution<int> distribution(a, b);
	int dice_roll = distribution(generator);
	return dice_roll;
}

HANDLE GetProcessByName(LPCSTR processName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)

		{
			if (_stricmp(wtoa_std(entry.szExeFile).c_str(), processName) == 0)
			{

				HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
				if (h)
					return h;
				else
				{
					return NULL;
				}
			}
		}
	}
	return NULL;

}

std::string GetSysVar(LPCSTR var)
{
	char* ptr;
	size_t psize;
	_dupenv_s(&ptr, &psize, var);
	std::string res = ptr;
	delete[] ptr;

	res += "\\";

	return res;
}


std::wstring To_wcs(std::string str)
{
	std::wstring wtmp;
	size_t len = str.length();
	if (len > 0)
	{
		size_t size = len + 1;
		wchar_t* buf = new wchar_t[size];
		std::mbstowcs(buf, str.c_str(), size);
		wtmp = buf;
		return wtmp.c_str();
	}
	else
	{
		wtmp = L"";
		return wtmp.c_str();
	}
}

void RemoveBOMFromFile(LPCTSTR path)
{
	HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dw = 0;
	size_t file_size = GetFileSize(hFile, NULL);
	BYTE* file_buffer = new BYTE[file_size];
	ReadFile(hFile, file_buffer, file_size, &dw, NULL);
	CloseHandle(hFile);

	if (file_buffer[0] == 0xEF && file_buffer[1] == 0xBB && file_buffer[2] == 0xBF)
	{
		size_t file_size_new = file_size - 3;
		BYTE* file_buffer_new = new BYTE[file_size_new];

		for (size_t i = 0; i < file_size_new; i++)
		{
			file_buffer_new[i] = file_buffer[i + 3];
		}
		hFile = CreateFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hFile, file_buffer_new, file_size_new, &dw, NULL);
		CloseHandle(hFile);
		delete[] file_buffer_new;
	}

	delete[] file_buffer;
}