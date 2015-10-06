#pragma once
#include <chrono>

class ChronoTimer
{
public:
	ChronoTimer();
	~ChronoTimer();
	unsigned long long GetLapTime();
	void StopTimer();
	void ResetTime();
	

protected:
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
};