#ifndef CMemMgr_H 
#define CMemMgr_H

/*

Type: Class
Class name: ÑMemMgr
Full name: Memory Manager
Intended for: Reading and writing current process memory by offsets. Only internal usage.

*/

#include <windows.h> 
#include <TlHelp32.h> 

#define BASE_ADDR 0x00400000		//For non-randomized start address of process

// offsets upper
typedef NTSTATUS(NTAPI* NtRVM)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesReaded);
typedef NTSTATUS(NTAPI* NtWVM) (HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesReaded);

class CMemMgr
{
protected:
	DWORD dwProtection;
	BOOL bProt;

	LPVOID __fastcall _getPointer(DWORD baseAddress, DWORD offset);
public:
	CMemMgr();
	~CMemMgr();

	inline void Protection(LPVOID dwAddress, size_t bytesOfData);

	template <class cData>
	cData __fastcall Read(DWORD dwBase, DWORD dwOff)
	{
		if (dwBase == 0)
			return 0;

		LPVOID addr = _getPointer(dwBase, dwOff);
		size_t dataSize = sizeof(cData);
		Protection(addr, dataSize);
		cData* pData = reinterpret_cast<cData*>(addr);
		cData data = *pData;
		Protection(addr, dataSize);
		return data;
	}

	template <class cData>
	cData __fastcall Read(LPCTSTR baseModule, DWORD dwOff)
	{
		DWORD dwBase = (DWORD)GetModuleHandle(baseModule);
		if (dwBase == 0)
			return 0;

		LPVOID addr = _getPointer(dwBase, dwOff);
		size_t dataSize = sizeof(cData);
		Protection(addr, dataSize);
		cData* pData = reinterpret_cast<cData*>(addr);
		cData data = *pData;
		Protection(addr, dataSize);
		return data;
	}


	void __fastcall ReadData(DWORD dwBase, DWORD dwOff, LPVOID dataBuffer, size_t bytesToRead, size_t sizeOfBuffer);
	void __fastcall ReadData(LPCTSTR baseModule, DWORD dwOff, LPVOID dataBuffer, size_t bytesToRead, size_t sizeOfBuffer);

	size_t __fastcall ReadDataAsStringA(DWORD dwBase, DWORD dwOff, LPSTR dataBuffer, size_t sizeOfBuffer);
	size_t __fastcall ReadDataAsStringW(DWORD dwBase, DWORD dwOff, LPWSTR dataBuffer, size_t sizeOfBuffer);
	size_t __fastcall ReadDataAsStringA(LPCTSTR baseModule, DWORD dwOff, LPSTR dataBuffer, size_t sizeOfBuffer);
	size_t __fastcall ReadDataAsStringW(LPCTSTR baseModule, DWORD dwOff, LPWSTR dataBuffer, size_t sizeOfBuffer);

	template <class cData>
	void __fastcall Write(DWORD dwBase, DWORD dwOff, cData value)
	{
		if (dwBase == 0)
			return;

		LPVOID addr = _getPointer(dwBase, dwOff);
		size_t dataSize = sizeof(cData);
		Protection(addr, dataSize);
		cData* pData = reinterpret_cast<cData*>(addr);
		*pData = value;
		Protection(addr, dataSize);
	}

	template <class cData>
	void __fastcall Write(LPCTSTR baseModule, DWORD dwOff, cData value)
	{
		DWORD dwBase = (DWORD)GetModuleHandle(baseModule);
		if (dwBase == 0)
			return;

		LPVOID addr = _getPointer(dwBase, dwOff);
		size_t dataSize = sizeof(cData);
		Protection(addr, dataSize);
		cData* pData = reinterpret_cast<cData*>(addr);
		*pData = value;
		Protection(addr, dataSize);
	}

	void __fastcall WriteData(DWORD dwBase, DWORD dwOff, LPVOID dataBuffer, size_t bytesToWrite);
	void __fastcall WriteData(LPCTSTR baseModule, DWORD dwOff, LPVOID dataBuffer, size_t bytesToWrite);
};

#endif
