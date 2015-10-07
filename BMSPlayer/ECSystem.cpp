#include "ECSystem.h"

ECSystem::ECSystem(){
	frame.setHandle("Theme/EndlessCirculation/Play/frame/SelfEvolution/SelfEvolution.png");
	lane.setHandle("Theme/EndlessCirculation/Play/object/AC/DEFAULT.png");
	notes.setHandle("Theme/EndlessCirculation/Play/parts/note/Default.png");
}

ECSystem::~ECSystem()
{
}

void ECSystem::drawsystembg(){
	//BG
	frame.setRect(0, 0, 1280, 720);
	frame.drawRect();

	//LANEBG
	frame.setRect(0, 724, 346, 585);
	frame.drawRect();

	//LANE
	lane.setRect(0, 431, 288, 481);
	lane.drawRect(45, 0);

}
void ECSystem::drawsystem(){
	//GAUGE BG
	frame.setRect(348, 724, 347, 169);
	frame.drawRect(0, 551);

	//TURNTABLE, KEY
	frame.setRect(348, 896, 333, 112);
	frame.drawRect(5, 468);
}

void ECSystem::drawkey(int key, int y){
	if (key == 0){
		notes.setRect(0, 8, 60, 8);
		notes.drawRect(45, y);
	}
	else if (key % 2 == 1){
		notes.setRect(61, 8, 34, 8);
		notes.drawRect(107 + 64 * (key / 2), y);
	}
	else{
		notes.setRect(96, 8, 26, 8);
		notes.drawRect(143 + 64 * ((key / 2) - 1), y);
	}
} 