#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>
#include "../valve_sdk/sdk.hpp"

#define PS_METHOD_NORMAL		Utils::PS_NORMAL
#define PS_METHOD_READ			Utils::PS_READ
#define PS_METHOD_SUBTRACT		Utils::PS_SUBTRACT
#define PS_METHOD_BOTH			(Utils::PS_READ | Utils::PS_SUBTRACT)

namespace Utils
{
	enum
	{
		PS_NORMAL = 0x0,
		PS_READ = 0x1,				   
		PS_SUBTRACT = 0x2,            
	};

    /*
     * @brief Create console
     *
     * Create and attach a console window to the current process
     */
    void AttachConsole();

    /*
     * @brief Detach console
     *
     * Detach and destroy the attached console
     */
    void DetachConsole();

    /*
     * @brief Print to console
     *
     * Replacement to printf that works with the newly created console
     */
    bool ConsolePrint(const char* fmt, ...);
    
    /*
     * @brief Blocks execution until a key is pressed on the console window
     *
     */
    char ConsoleReadKey();

    /*
     * @brief Wait for all the given modules to be loaded
     *
     * @param timeout How long to wait
     * @param modules List of modules to wait for
     *
     * @returns See WaitForSingleObject return values.
     */
    int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);

    /*
     * @brief Scan for a given byte pattern on a module
     *
     * @param module    Base of the module to search
     * @param signature IDA-style byte array pattern
     *
     * @returns Address of the first occurence
     */
    std::uint8_t* PatternScan(void* module, const char* signature);
	DWORD PatternScanEx(void* module, const char* signature, short method, uintptr_t patternOffset, uintptr_t addressOffset);
	DWORD PatternScanEx(LPCWSTR moduleName, const char* signature, short method, uintptr_t patternOffset, uintptr_t addressOffset);

    /*
     * @brief Set player clantag
     *
     * @param tag New clantag
     */
    void SetClantag(const char* tag);

    /*
     * @brief Set player name
     *
     * @param name New name
     */
    void SetName(const char* name);

    /*
     * @brief Reveal the ranks of all players on the server
     *
     */
    void RankRevealAll();
	
	float GetRandomFloat(float min, float max);
	int GetRandomInt(int min, int max);

	template<typename T>
	static T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
	{
		return p1 * (1 - t) * (1 - t) * (1 - t) +
			p2 * 3 * t * (1 - t) * (1 - t) +
			p3 * 3 * t * t * (1 - t) +
			p4 * t * t * t;
	}

}
