#include "MemMgr.h"
#include "offsets.h"
#include "globals.h"

#ifdef UNICODE

#define tstrcmp wcscmp

#else

#define tstrcmp strcmp

#endif


CMemMgr::CMemMgr()
{
	bProt = false;
	dwProtection = NULL;
}

CMemMgr::~CMemMgr()
{
}

LPVOID __fastcall CMemMgr::_getPointer(DWORD baseAddress, DWORD offset)
{
	LPVOID address;
	address = LPVOID(baseAddress);
	address = LPVOID((DWORD)address + offset);
	return address;
}

inline void CMemMgr::Protection(LPVOID dwAddress, size_t bytesOfData)
{
	if (!bProt)
		VirtualProtect(dwAddress, bytesOfData, PAGE_READWRITE, &dwProtection);
	else
		VirtualProtect(dwAddress, bytesOfData, dwProtection, &dwProtection);

	bProt = !bProt;
}

void __fastcall CMemMgr::ReadData(DWORD dwBase, DWORD dwOff, LPVOID dataBuffer, size_t bytesToRead, size_t sizeOfBuffer)
{
	if (dwBase == 0)
		return;

	LPVOID addr = _getPointer(dwBase, dwOff);
	Protection(addr, bytesToRead);
	memcpy_s(dataBuffer, sizeOfBuffer, addr, bytesToRead);
	Protection(addr, bytesToRead);
}

void __fastcall CMemMgr::ReadData(LPCTSTR baseModule, DWORD dwOff, LPVOID dataBuffer, size_t bytesToRead, size_t sizeOfBuffer)
{
	DWORD dwBase = (DWORD)GetModuleHandle(baseModule);
	if (dwBase == 0)
		return;

	LPVOID addr = _getPointer(dwBase, dwOff);
	Protection(addr, bytesToRead);
	memcpy_s(dataBuffer, sizeOfBuffer, addr, bytesToRead);
	Protection(addr, bytesToRead);
}

size_t __fastcall CMemMgr::ReadDataAsStringA(DWORD dwBase, DWORD dwOff, LPSTR dataBuffer, size_t sizeOfBuffer)
{
	if (dwBase == 0)
		return 0;

	LPSTR remoteStr = reinterpret_cast<LPSTR>(_getPointer(dwBase, dwOff));
	Protection(remoteStr, sizeOfBuffer);
	size_t len = 0;
	for (size_t i = 0; i < sizeOfBuffer; i++)
	{
		dataBuffer[i] = remoteStr[i];
		if (remoteStr[i] == '\0')
			break;

		if (i == sizeOfBuffer - 1)
			remoteStr[i] = '\0';

		len = i;
	}
	Protection(remoteStr, sizeOfBuffer);

	return len;
}

size_t __fastcall CMemMgr::ReadDataAsStringW(DWORD dwBase, DWORD dwOff, LPWSTR dataBuffer, size_t sizeOfBuffer)
{
	if (dwBase == 0)
		return 0;

	LPWSTR remoteStr = reinterpret_cast<LPWSTR>(_getPointer(dwBase, dwOff));
	Protection(remoteStr, sizeOfBuffer);
	size_t len = 0;
	for (size_t i = 0; i < sizeOfBuffer; i++)
	{
		dataBuffer[i] = remoteStr[i];
		if (remoteStr[i] == '\0')
			break;

		if (i == sizeOfBuffer - 1)
			remoteStr[i] = '\0';

		len = i;
	}
	Protection(remoteStr, sizeOfBuffer);

	return len;
}

size_t __fastcall CMemMgr::ReadDataAsStringA(LPCTSTR baseModule, DWORD dwOff, LPSTR dataBuffer, size_t sizeOfBuffer)
{
	DWORD dwBase = (DWORD)GetModuleHandle(baseModule);
	if (dwBase == 0)
		return 0;

	LPSTR remoteStr = reinterpret_cast<LPSTR>(_getPointer(dwBase, dwOff));
	Protection(remoteStr, sizeOfBuffer);
	size_t len = 0;
	for (size_t i = 0; i < sizeOfBuffer; i++)
	{
		dataBuffer[i] = remoteStr[i];
		if (remoteStr[i] == '\0')
			break;

		if (i == sizeOfBuffer - 1)
			remoteStr[i] = '\0';

		len = i;
	}
	Protection(remoteStr, sizeOfBuffer);

	return len;
}

size_t __fastcall CMemMgr::ReadDataAsStringW(LPCTSTR baseModule, DWORD dwOff, LPWSTR dataBuffer, size_t sizeOfBuffer)
{
	DWORD dwBase = (DWORD)GetModuleHandle(baseModule);
	if (dwBase == 0)
		return 0;

	LPWSTR remoteStr = reinterpret_cast<LPWSTR>(_getPointer(dwBase, dwOff));
	Protection(remoteStr, sizeOfBuffer);
	size_t len = 0;
	for (size_t i = 0; i < sizeOfBuffer; i++)
	{
		dataBuffer[i] = remoteStr[i];
		if (remoteStr[i] == '\0')
			break;

		if (i == sizeOfBuffer - 1)
			remoteStr[i] = '\0';

		len = i;
	}
	Protection(remoteStr, sizeOfBuffer);

	return len;
}

void __fastcall CMemMgr::WriteData(DWORD dwBase, DWORD dwOff, LPVOID dataBuffer, size_t bytesToWrite)
{
	if (dwBase == 0)
		return;

	LPVOID addr = _getPointer(dwBase, dwOff);
	Protection(addr, bytesToWrite);
	memcpy_s(addr, bytesToWrite, dataBuffer, bytesToWrite);
	Protection(addr, bytesToWrite);
}

void __fastcall CMemMgr::WriteData(LPCTSTR baseModule, DWORD dwOff, LPVOID dataBuffer, size_t bytesToWrite)
{
	DWORD dwBase = (DWORD)GetModuleHandle(baseModule);
	if (dwBase == 0)
		return;

	LPVOID addr = _getPointer(dwBase, dwOff);
	Protection(addr, bytesToWrite);
	memcpy_s(addr, bytesToWrite, dataBuffer, bytesToWrite);
	Protection(addr, bytesToWrite);
}