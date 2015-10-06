#include "input.h"

DxInput::DxInput(){
	input_status.assign(26, 0);
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
	}

	for (int i = 0; i < 26; i++){
		if (checkHitInput())
			input_status.at(i)++;
	}
}

bool DxInput::checkHitInput(){
	return false;
}