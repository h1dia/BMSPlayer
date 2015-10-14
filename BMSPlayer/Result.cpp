#include "Result.h"


Result::Result(){
	judge.assign(30, 0);
}


Result::~Result()
{
}

void Result::countUp(int key){
	judge.at(key)++;
}

int Result::getResult(int key){
	return judge.at(key);
}