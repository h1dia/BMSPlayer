#include "input.h"

DxInput::DxInput(){
	input_status.assign(256, std::pair<bool, unsigned long long>(false, 0));

	key_status.assign(256, 0);
	joypad_state = GetJoypadNum();
}

DxInput::~DxInput(){

}

void DxInput::inputUpdate(){
	GetHitKeyStateAll(key_buffer);
	for (int i = 0; i < 256; i++){
		if (key_buffer[i]){
			key_status.at(i)++;

			if (key_status.at(i) == 1){
				input_status.at(i).first = true;
				input_status.at(i).second = GetLapTime();
			}
		}
		else
			key_status.at(i) = 0;
	}
}

unsigned long long DxInput::getInputTime(int key){
	return input_status.at(key).second;
}

bool DxInput::isUpdateStatus(int key){
	if (input_status.at(key).first){
		input_status.at(key).first = false;
		return true;
	}
	else{
		return false;
	}
}