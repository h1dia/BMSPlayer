#include "timer.h"

ChronoTimer::ChronoTimer(){
	begin = std::chrono::steady_clock::now();
}

ChronoTimer::~ChronoTimer(){

}

unsigned long long ChronoTimer::GetLapTime(){
	auto lap = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin);
	return lap.count();
}

void ChronoTimer::ResetTime(){
	begin = std::chrono::steady_clock::now();
}