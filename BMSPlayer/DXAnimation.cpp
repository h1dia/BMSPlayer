#include "DXAnimation.h"

DXAnimation::DXAnimation()
{
}


DXAnimation::~DXAnimation()
{
}

void DXAnimation::setSplit(int AllNum, int Xnum, int Ynum, int Xsize, int Ysize){
	LoadDivGraph(path.c_str(), AllNum, Xnum, Ynum, Xsize, Ysize, handle);
}