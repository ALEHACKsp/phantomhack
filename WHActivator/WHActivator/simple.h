/*

Simple functions for WinAPI, CURL and other utils.

*/

#pragma once
#include <vector>
#include <wininet.h>
#include <urlmon.h>



#define CURL_STATICLIB
//#define CURL_USE_SSL

#define ET_STATIC						0
#define ET_EDIT							1
#define ET_COMBOBOX						2
#define ET_LISTBOX						3
#define ET_MDICLIENT					4
#define ET_SCROLLBAR					5
#define ET_BUTTON						6
#define ET_CHECKBOX						7
#define ET_RADIOBUTTON					8
#define ET_GROUPBOX						9
#define ET_RICHEDIT						10
#define ET_PROGRESS						11
#define ET_SLIDER_V						12
#define ET_SLIDER_H						13
#define DEFAULT_COLOR					(HBRUSH)COLOR_WINDOW

#define DMB(MSG)						MessageBox(NULL, MSG, _T("DEBUG"), MB_OK)

#define INJDLL_FAILED					0
#define INJDLL_NOTFOUND					-1

HWND __fastcall CreateWindowEasy(const TCHAR* ClassName, WNDPROC WndProc, HINSTANCE hInst,
const TCHAR* Title, HWND Parent = NULL, INT pos_x = CW_USEDEFAULT, INT pos_y = CW_USEDEFAULT,
INT Width = CW_USEDEFAULT, INT Height = CW_USEDEFAULT, HBRUSH Color = DEFAULT_COLOR,
DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE, DWORD WindowStyleEx = NULL,
UINT ClassStyle = CS_HREDRAW | CS_VREDRAW);
HWND __fastcall CreateWindowElement(HWND, UINT, const TCHAR*, HINSTANCE, DWORD, DWORD, UINT, INT, INT, INT, INT, BOOL = FALSE);
std::string HTTPGet(std::string);
std::string HTTPPost(std::string url, std::string fields);
DWORD DownloadFile(std::string url, std::string save_path);
DWORD _downloadFile(std::string url, std::string save_path, size_t maxsize);
std::string wtoa_std(std::wstring);
std::wstring atow_std(std::string);
BOOL InjectDLL(LPCSTR processName, LPCSTR dllName, HANDLE* pHandle);
std::string GetCurrentAppPath();
int GetRandomBetween(int a, int b);
HANDLE GetProcessByName(LPCSTR processName);
std::string GetSysVar(LPCSTR var);


void __fastcall UpdaterProgress(int per, size_t bytesDownloaded, size_t fullSize);

//#define _CBB_USE_STACK

struct CURL_BIN_BUFFER
{
#ifndef _CBB_USE_STACK
	BYTE* Buffer;
#else
	std::vector<BYTE> Buf;
#endif
	bool init;
	size_t size;
	size_t max;

	CURL_BIN_BUFFER()
	{
		init = false;
#ifndef _CBB_USE_STACK
		Buffer = nullptr;
#endif
		size = 0;
		max = 0;
	}

	~CURL_BIN_BUFFER()
	{
#ifndef _CBB_USE_STACK
		if (init)
			delete Buffer;
#endif
	}

	void __fastcall push(BYTE b)
	{
		if (!init)
		{
			size = 1;
			init = true;
#ifndef _CBB_USE_STACK
			Buffer = new BYTE[size];
			Buffer[0] = b;
#else
			Buf.push_back(b);
#endif
		}
		else
		{
#ifndef _CBB_USE_STACK
			BYTE* tmp;
			tmp = Buffer;
			Buffer = new BYTE[size+1];
			//memcpy(Buffer, tmp, size+1);
			for (size_t i = 0; i < size; i++)
				Buffer[i] = tmp[i];
			delete[] tmp;
			Buffer[size] = b;
			size++;
#else
			Buf.push_back(b);
			size++;
#endif
		}
	}
};


void SetMainButtonText(LPCTSTR text);
class CallbackHandler : public IBindStatusCallback
{
private:
	int m_percentLast;
	size_t m_bytesMax;
	size_t m_bytesDownloaded;

public:
	CallbackHandler() : m_percentLast(0)
	{
		m_bytesMax = 0;
		m_bytesDownloaded = 0;
	}

	// IUnknown

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject)
	{

		if (IsEqualIID(IID_IBindStatusCallback, riid)
			|| IsEqualIID(IID_IUnknown, riid))
		{
			*ppvObject = reinterpret_cast<void*>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	int STDMETHODCALLTYPE GetPercentage() const
	{
		return m_percentLast;
	}

	size_t STDMETHODCALLTYPE GetBytesCount() const
	{
		return m_bytesDownloaded;
	}

	size_t STDMETHODCALLTYPE GetBytesMax() const
	{
		return m_bytesMax;
	}

	void STDMETHODCALLTYPE SetBytesMax(size_t size)
	{
		m_bytesMax = size;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 2UL;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		return 1UL;
	}

	// IBindStatusCallback

	HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, IBinding* pib)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetPriority(LONG* pnPriority)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
	{
		switch (ulStatusCode)
		{
		case BINDSTATUS_FINDINGRESOURCE:
			
			break;
		case BINDSTATUS_CONNECTING:
			
			break;
		case BINDSTATUS_SENDINGREQUEST:
			
			break;
		case BINDSTATUS_MIMETYPEAVAILABLE:
			
			break;
		case BINDSTATUS_CACHEFILENAMEAVAILABLE:
			
			break;
		case BINDSTATUS_BEGINDOWNLOADDATA:
		
			break;
		case BINDSTATUS_DOWNLOADINGDATA:
		case BINDSTATUS_ENDDOWNLOADDATA:
		{
			m_bytesDownloaded = ulProgress;
			int per = (int)floor(((float)m_bytesDownloaded / (float)m_bytesMax) * 100.0f);
			UpdaterProgress(per, m_bytesDownloaded, m_bytesMax);

			if (m_percentLast < per)
			{
				m_percentLast = per;
			}

			if (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
			{
				//tcout << endl << _T("End download") << endl;
			}
		}
		break;

		default:
		{
			
		}
		}
		// The download can be cancelled by returning E_ABORT here
		// of from any other of the methods.
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID   riid, IUnknown* punk)
	{
		return E_NOTIMPL;
	}
};
