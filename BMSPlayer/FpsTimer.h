#pragma once
#include "timer.h"
class FpsTimer :
	public ChronoTimer
{
public:
	FpsTimer();
	~FpsTimer();
	int GetFps();
	int GetDiff();
	
private:
	int now_fps;
	int before_fps;
	unsigned long long before_time;
};
