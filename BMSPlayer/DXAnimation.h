#pragma once
#include "DXGraphic.h"
#include <array>

class DXAnimation :
	public DXGraphic
{
public:
	DXAnimation();
	~DXAnimation();

	void setHandle(std::string);
	void setSplit(int Xnum, int Ynum, int AllNum, int Xsize, int Ysize);

private:
	int handle[100];
	int size;
};

