#include "input.h"

DxInput::DxInput(){
	//input_status.assign(26, 0);
	key_status.assign(256, 0);
	joypad_state = GetJoypadNum();
}

DxInput::~DxInput(){

}

void DxInput::inputUpdate(){
	GetHitKeyStateAll(key_buffer);
	for (int i = 0; i < 256; i++){
		if (key_buffer[i])
			key_status.at(i)++;
		else
			key_status.at(i) = 0;
	}
}

int DxInput::keyStatus(int key){
	if (key_status.at(key) == 0)
		return 0;
	// ‰Ÿ‰º‚µ‚½uŠÔ
	else if (key_status.at(key) == 1){
		return 1;
	}
	// ‰Ÿ‰º‚µ‚Á‚Ï‚È‚µ
	else{
		return 2;
	}
}