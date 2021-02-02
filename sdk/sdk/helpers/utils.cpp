#include "utils.hpp"

#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <random>
#include <time.h>

#include "../valve_sdk/csgostructs.hpp"
#include "math.hpp"

HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

#define stime (UINT)time(NULL)

namespace Utils
{
	LPVOID __fastcall _getPointer(DWORD baseAddress, DWORD offset)
	{
		LPVOID address;
		address = LPVOID(baseAddress);
		address = LPVOID((DWORD)address + offset);
		return address;
	}

	DWORD_PTR __fastcall _Read(DWORD dwOff)
	{
		DWORD dwBase = 0;
		DWORD dwProtection = 0;
		LPVOID addr = _getPointer(dwBase, dwOff);
		size_t dataSize = sizeof(DWORD_PTR);
		VirtualProtect(addr, dataSize, PAGE_READWRITE, &dwProtection);
		DWORD_PTR* pData = reinterpret_cast<DWORD_PTR*>(addr);
		DWORD_PTR data = *pData;
		VirtualProtect(addr, dataSize, dwProtection, &dwProtection);
		return data;
	}

	bool CompareBytes(const unsigned char* bytes, const char* pattern)
	{
		for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern)
		{
			if (*pattern == ' ' || *pattern == '?')
				continue;
			if (*bytes != getByte(pattern))
				return false;
			++pattern;
		}
		return true;
	}

	/*
	* @brief Create console
	*
	* Create and attach a console window to the current process
	*/
	void AttachConsole()
	{
		_old_out = GetStdHandle(STD_OUTPUT_HANDLE);
		_old_err = GetStdHandle(STD_ERROR_HANDLE);
		_old_in = GetStdHandle(STD_INPUT_HANDLE);

		::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

		_out = GetStdHandle(STD_OUTPUT_HANDLE);
		_err = GetStdHandle(STD_ERROR_HANDLE);
		_in = GetStdHandle(STD_INPUT_HANDLE);

		SetConsoleMode(_out,
			ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

		SetConsoleMode(_in,
			ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
			ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
	}

	/*
	* @brief Detach console
	*
	* Detach and destroy the attached console
	*/
	void DetachConsole()
	{
		if (_out && _err && _in) {
			FreeConsole();

			if (_old_out)
				SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
			if (_old_err)
				SetStdHandle(STD_ERROR_HANDLE, _old_err);
			if (_old_in)
				SetStdHandle(STD_INPUT_HANDLE, _old_in);
		}
	}

	/*
	* @brief Print to console
	*
	* Replacement to printf that works with the newly created console
	*/
	bool ConsolePrint(const char* fmt, ...)
	{
		if (!_out)
			return false;

		char buf[1024];
		va_list va;

		va_start(va, fmt);
		_vsnprintf_s(buf, 1024, fmt, va);
		va_end(va);

		return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
	}

	/*
	* @brief Blocks execution until a key is pressed on the console window
	*
	*/
	char ConsoleReadKey()
	{
		if (!_in)
			return false;

		auto key = char{ 0 };
		auto keysread = DWORD{ 0 };

		ReadConsoleA(_in, &key, 1, &keysread, nullptr);

		return key;
	}


	/*
	* @brief Wait for all the given modules to be loaded
	*
	* @param timeout How long to wait
	* @param modules List of modules to wait for
	*
	* @returns See WaitForSingleObject return values.
	*/
	int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules)
	{
		bool signaled[32] = { 0 };
		bool success = false;

		std::uint32_t totalSlept = 0;

		if (timeout == 0) {
			for (auto& mod : modules) {
				if (GetModuleHandleW(std::data(mod)) == NULL)
					return WAIT_TIMEOUT;
			}
			return WAIT_OBJECT_0;
		}

		if (timeout < 0)
			timeout = INT32_MAX;

		while (true) {
			for (auto i = 0u; i < modules.size(); ++i) {
				auto& module = *(modules.begin() + i);
				if (!signaled[i] && GetModuleHandleW(std::data(module)) != NULL) {
					signaled[i] = true;

					//
					// Checks if all modules are signaled
					//
					bool done = true;
					for (auto j = 0u; j < modules.size(); ++j) {
						if (!signaled[j]) {
							done = false;
							break;
						}
					}
					if (done) {
						success = true;
						goto exit;
					}
				}
			}
			if (totalSlept > std::uint32_t(timeout)) {
				break;
			}
			Sleep(10);
			totalSlept += 10;
		}

	exit:
		return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
	}

	/*
	* @brief Scan for a given byte pattern on a module
	*
	* @param module    Base of the module to search
	* @param signature IDA-style byte array pattern
	*
	* @returns Address of the first occurence

	*/

	std::uint8_t* PatternScan(void* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return &scanBytes[i];
			}
		}
		return nullptr;

	}

	DWORD PatternScanEx(LPCWSTR moduleName, const char* signature, short method, uintptr_t patternOffset, uintptr_t addressOffset)
	{
		HMODULE hModule = GetModuleHandleW(moduleName);
		return PatternScanEx(hModule, signature, method, patternOffset, addressOffset);
	}

	DWORD PatternScanEx(void* module, const char* signature, short method, uintptr_t patternOffset, uintptr_t addressOffset)
	{
		DWORD result = 0;
		uintptr_t base = uintptr_t(module);
		PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)module;
		PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		DWORD sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		std::uint8_t* scanBytes = reinterpret_cast<std::uint8_t*>(module);


		for (auto off = 0UL; off < sizeOfImage; ++off)
		{
			if (CompareBytes(scanBytes + off, signature))
			{

				result = base + off + patternOffset;

				if (method & PS_READ)
					result = _Read(result);

				if (method & PS_SUBTRACT)
					result -= base;

				return result + addressOffset;
			}
		}

		return 0;
	}

	/*
	* @brief Set player clantag
	*
	* @param tag New clantag
	*/
	void SetClantag(const char* tag)
	{
		static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))PatternScan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

		fnClantagChanged(tag, tag);
	}

	/*
	* @brief Set player name
	*
	* @param name New name
	*/
	void SetName(const char* name)
	{
		static auto nameConvar = g_CVar->FindVar("name");
		nameConvar->m_fnChangeCallbacks.m_Size = 0;

		// Fix so we can change names how many times we want
		// This code will only run once because of `static`
		static auto do_once = (nameConvar->SetValue("\nญญญ"), true);

		nameConvar->SetValue(name);
	}

	/*
	* @brief Reveal the ranks of all players on the server
	*
	*/
	void RankRevealAll()
	{
		using ServerRankRevealAll = char(__cdecl*)(int*);

		static auto fnServerRankRevealAll = PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 8B 0D ? ? ? ? 68");

		int v[3] = { 0,0,0 };

		reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
	}

	float GetRandomFloat(float min, float max)
	{
		std::random_device Random;
		std::mt19937 RandomGenerator(Random());
		std::uniform_real<float> RandomDistribute(min, max);
		return RandomDistribute(RandomGenerator);
	}

	int GetRandomInt(int min, int max)
	{
		static bool rinit = false;
		static std::default_random_engine generator;
		if (!rinit)
		{
			generator.seed(stime);
			rinit = true;
		}
		std::uniform_int_distribution<int> distribution(min, max);
		int dice_roll = distribution(generator);
		return dice_roll;
	}
}