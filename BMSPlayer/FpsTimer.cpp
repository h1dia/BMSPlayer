#include "FpsTimer.h"

FpsTimer::FpsTimer()
{
	begin = std::chrono::steady_clock::now();
	before_fps = 0;
	now_fps = 0;
	before_time = 0;
}

FpsTimer::~FpsTimer()
{
}

int FpsTimer::GetFps()
{
	now_fps++;

	if (GetLapTime() >= 1000000){
		before_fps = now_fps;

		ResetTime();
		now_fps = 0;
	}

	return before_fps;
}

int FpsTimer::GetDiff(){
	unsigned long long ret_time;
	unsigned long long now_time = GetLapTime();
	
	ret_time = now_time - before_time;
	before_time = now_time;

	return ret_time;
}