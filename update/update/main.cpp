#include <windows.h>
#include <process.h>
#include <string>
#include <Shlwapi.h>
#include <tlhelp32.h>

#include "defs.h"


#pragma comment(lib, "Shlwapi.lib")



HANDLE GetProcessByName(LPCSTR processName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)

		{
			if (_stricmp(entry.szExeFile, processName) == 0)
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


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	std::string cmd = lpCmdLine;
	std::string fpath = GetSysVar("TMP") + "g8h7ther987k8re.tmp";
	HANDLE hActivator = GetProcessByName(PathFindFileNameA(cmd.c_str()));

	if (cmd.find("#") == std::string::npos)
		return 1;

	size_t i = cmd.find("#");
	cmd.erase(i, 1);
	i = cmd.find("#");
	cmd.erase(i, cmd.length() - 1);


	if (hActivator)
		TerminateProcess(hActivator, 0);
	
	hActivator = GetProcessByName(PathFindFileNameA(cmd.c_str()));
	if (hActivator)
		TerminateProcess(hActivator, 0);

	int rc = 1;
	while(rc != 0)
		rc = remove(cmd.c_str());

	if (cmd.find("#rd") != std::string::npos)
		return 2;

	rc = 1;
	while (rc != 0)
		rc = rename(fpath.c_str(), cmd.c_str());

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	std::string cmdstrupd = cmd + " -upd";
	std::string cmdstr = cmd;
	CreateProcessA(NULL, const_cast<LPSTR>(cmdstrupd.c_str()), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CreateProcessA(NULL, const_cast<LPSTR>(cmdstr.c_str()), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	return 0;
}