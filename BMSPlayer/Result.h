#pragma once
#include "define.h"

class Result
{
public:
	Result();
	~Result();

	void countUp(int key);
	int getResult(int key);

private:
	std::vector <unsigned long> judge;
};

