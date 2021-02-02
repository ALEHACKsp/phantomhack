#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <commctrl.h>
#include <richedit.h>
#include <regex>

#include "md5.h"
#include "simple.h"
#include "defs.h"

extern int g_ButtonState;
extern HANDLE g_hEvent;
extern xstringA g_UserID;
extern std::string g_IP; 
extern BOOL g_RegSuccess;
extern BOOL g_Activated;
extern HANDLE g_InjectEvent;
extern BOOL g_Injected;
extern int g_csgoProcess;
extern bool g_bQuit;
extern HANDLE g_hCSGO;
extern std::string gdp;

//Main window event handler
std::string dllpath;
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;
	HINSTANCE hInst = GetModuleHandle(NULL);
	int injcode;
	if(dllpath == "")
		dllpath = GetSysVar("TMP") + DLL_FILENAME;
	switch (message)
	{
	case WM_KEYDOWN: 
		
		break;

	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_USERID))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 0, 0));
			return (LRESULT)((HBRUSH)GetStockObject(BLACK_BRUSH));
		}
		else if ((HWND)lParam == GetDlgItem(hWnd, IDC_INFORMATION))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 255, 0));
			return (LRESULT)((HBRUSH)GetStockObject(BLACK_BRUSH));
		}
		else if ((HWND)lParam == GetDlgItem(hWnd, IDC_INFOSIGN))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			return (LRESULT)((HBRUSH)GetStockObject(COLOR_WINDOW));
		}
		else if ((HWND)lParam == GetDlgItem(hWnd, IDC_USERIDSIGN))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			return (LRESULT)((HBRUSH)GetStockObject(COLOR_WINDOW));
		}
	
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_ACTIVATE)
		{
			switch (g_ButtonState)
			{
				case BTS_RETRY:
					CleanMessages();
					UpdateFunc(hInst, hWnd, g_hEvent, TRUE);
					break;

				case BTS_REGISTER:
					break;

				case BTS_PLAY:
					ActivationResponse(true);
					ShowWindow(hWnd, FALSE);
					break;

				case BTS_RESTART:
					exit(0);
					break;
				case BTS_EXIT:
					if (g_Activated)
					{
						ActivationResponse(true);
						ShowWindow(hWnd, FALSE);
					}
					else
					{
						ActivationResponse(false);
						PostQuitMessage(0);
					}
					break; 
				case BTS_INJECT:
					ChangeButtonState(BTS_WAITING);
					injcode = InjectDLL("csgo.exe", dllpath.c_str(), &g_hCSGO);
					if (injcode > 0)
					{
						g_Injected = true;
						g_csgoProcess = injcode;
						AddInfoLine(_T("****************************************"));
						AddInfoLine(_T("Хак успешно подключен к игре."));
						AddInfoLine(_T("****************************************"));
						PulseEvent(g_InjectEvent);
					}
					else if (injcode == INJDLL_NOTFOUND)
					{
						AddInfoLine(_T("****************************************"));
						AddInfoLine(_T("Не удалось подключить PhantomHack к игре, игра не запущена. Запустите игру и попробуйте еще раз..."));
						ChangeButtonState(BTS_INJECT);
					}
					else
					{
						AddInfoLine(_T("****************************************"));
						AddInfoLine(_T("Неизвестная ошибка, попробуйте еще раз."));
						ChangeButtonState(BTS_INJECT);
					}
					break;
			}
		}
		break;

	case WM_QUIT:
		if (g_bQuit)
		{
			std::string dllpath = gdp.c_str() + DLL_FILENAME;
			remove(dllpath.c_str());
			exit(0);
		}
	case WM_DESTROY:

		if (g_Activated && !g_bQuit)
		{
			ActivationResponse(true);
			ShowWindow(hWnd, FALSE);
		}
		else
		{
			if(g_Injected)
				ActivationResponse(false);
			std::string dllpath = gdp.c_str() + DLL_FILENAME;
			remove(dllpath.c_str());
			PostQuitMessage(0);
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

//New user window event handler
LRESULT CALLBACK RegWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;
	std::regex login_re("^[A-Za-z0-9_.-]+$");
	std::regex email_re("(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$)");
	char login_buf[128], email_buf[128], pass_buf[128], passc_buf[128];
	std::string a, b;
	std::string postargs, result;
	int response;
	switch (message)
	{
	case WM_KEYDOWN:
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_ICHECKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_ICHECKED) == BST_CHECKED)
				EnableWindow(GetDlgItem(hWnd, IDC_REGISTER), TRUE);
			else
				EnableWindow(GetDlgItem(hWnd, IDC_REGISTER), FALSE);
		}
		else if (LOWORD(wParam) == IDC_REGISTER)
		{
			GetWindowTextA(GetDlgItem(hWnd, IDC_LOGIN), login_buf, 128);
			GetWindowTextA(GetDlgItem(hWnd, IDC_EMAIL), email_buf, 128);
			GetWindowTextA(GetDlgItem(hWnd, IDC_PASSWORD), pass_buf, 128);
			GetWindowTextA(GetDlgItem(hWnd, IDC_PASSCONF), passc_buf, 128);
			a = pass_buf;
			b = passc_buf;
			if (strlen(login_buf) == 0)
			{
				MessageBox(hWnd, _T("Введите логин."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (!std::regex_match(login_buf, login_re))
			{
				MessageBox(hWnd, _T("В вашем логине присутствуют недопустимые символы. Допускаются только символы латиницы, а также \"-\", \"_\" и точка."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(login_buf) < 4)
			{
				MessageBox(hWnd, _T("Ваш логин слишком короткий."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(login_buf) > 64)
			{
				MessageBox(hWnd, _T("Ваш логин слишком длинный."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(email_buf) == 0)
			{
				MessageBox(hWnd, _T("Введите e-mail."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (!std::regex_match(email_buf, email_re) || strlen(email_buf) < 6)
			{
				MessageBox(hWnd, _T("Вы ввели недопустимый e-mail. Проверьте его и попробуйте снова."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(email_buf) > 64)
			{
				MessageBox(hWnd, _T("Ваш e-mail слишком длинный."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(pass_buf) == 0)
			{
				MessageBox(hWnd, _T("Введите пароль."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(pass_buf) < 6)
			{
				MessageBox(hWnd, _T("Ваш пароль должен быть не короче 6 символов."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(pass_buf) > 32)
			{
				MessageBox(hWnd, _T("Ваш пароль слишком длинный."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (strlen(passc_buf) == 0)
			{
				MessageBox(hWnd, _T("Введите подтверждение пароля."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			if (a != b)
			{
				MessageBox(hWnd, _T("Ваши пароли не совпадают."), _T("Предупреждение"), MB_OK | MB_ICONINFORMATION);
				break;
			}

			EnableWindow(GetDlgItem(hWnd, IDC_ICHECKED), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_REGISTER), FALSE);

			postargs = md5("reg") + "=" + GetRegistrationKey(g_IP, g_UserID) 
				+ "&" + md5("uid") + "=" + g_UserID
				+ "&" + md5("username") + "=" + login_buf
				+ "&" + md5("pass") + "=" + pass_buf
				+ "&" + md5("email") + "=" + email_buf;
			result = HTTPPost(ACTIVATION_HOST, postargs);

			response = atoi(result.c_str());
			switch (response)
			{
				case 1:
					g_RegSuccess = REGSTATE_SUCCESS;
					PostQuitMessage(0);
					break;

				case 2:
					MessageBox(hWnd, _T("Такое имя пользователя уже существует. Пожалуйста выберите другое имя пользователя."), _T("Предупреждение"), MB_OK | MB_ICONWARNING);
					EnableWindow(GetDlgItem(hWnd, IDC_ICHECKED), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_REGISTER), TRUE);
					CheckDlgButton(hWnd, IDC_ICHECKED, BST_UNCHECKED);
					break;

				case 3:
					g_RegSuccess = REGSTATE_CONFLICT;
					PostQuitMessage(0);
					break;

				case 4:
					MessageBox(hWnd, _T("Пользователь с таким e-mail адресом уже зарегистрирован. Пожалуйста введите другой e-mail."), _T("Предупреждение"), MB_OK | MB_ICONWARNING);
					EnableWindow(GetDlgItem(hWnd, IDC_ICHECKED), TRUE);
					EnableWindow(GetDlgItem(hWnd, IDC_REGISTER), TRUE);
					CheckDlgButton(hWnd, IDC_ICHECKED, BST_UNCHECKED);
					break;
				case 5:
					MessageBox(hWnd, _T("Регистрация невозможна."), _T("Ошибка"), MB_OK | MB_ICONERROR);
					PostQuitMessage(0);
					break;

				default:
					MessageBox(hWnd, _T("Ошибка регистрации."), _T("Ошибка"), MB_OK | MB_ICONERROR);
					PostQuitMessage(0);
			}
		}
		break;

	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}