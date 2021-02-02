#include <chrono>
#include <thread>
#include <random>
#include "globals.h"

template <typename T>
T __fastcall RandomBetween(T min, T max)
{
	static bool rinit = false;
	static std::default_random_engine generator;
	if (!rinit)
	{
		generator.seed((UINT)time(NULL));
		rinit = true;
	}
	std::uniform_int_distribution<T> distribution(min, max);
	int dice_roll = distribution(generator);
	return dice_roll;
}

void __stdcall FakeLagThread(LPVOID lpThreadParameter)
{
	int dur, per;
	while (true)
	{
		
		WaitForSingleObject(hFakeLagMutex, INFINITE);
		if (g_FakeLagRandomize)
		{
			dur = RandomBetween<int>(125, g_FakeLagDuration);
			per = RandomBetween<int>(125, g_FakeLagPeriod);
		}
		else
		{
			dur = g_FakeLagDuration;
			per = g_FakeLagPeriod;
		}
		ReleaseMutex(hFakeLagMutex);

		if (g_FakeLag)
		{
			g_FakeLagNow = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(dur));
			g_FakeLagNow = false;
		}
		else
			g_FakeLagNow = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(per));
	}
}