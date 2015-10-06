#include "DxLib.h"
#include <vector>

class DxInput{
public:
	DxInput();
	~DxInput();

	void inputUpdate();

private:
	bool checkHitInput();

	char key_buffer[256];
	std::vector<long int> key_status;
	std::vector<long int> input_status;
	int joypad_state;
	int num_joypad_input;
	int num_key_input;
	int C_INPUT_1_1;
	int C_INPUT_1_2;
	int C_INPUT_1_3;
	int C_INPUT_1_4;
	int C_INPUT_1_5;
	int C_INPUT_1_6;
	int C_INPUT_1_7;
	int C_INPUT_1_8;
	int C_INPUT_1_9;
	int C_INPUT_1_SU;
	int C_INPUT_1_SD;
	int C_INPUT_1_START;
	int C_INPUT_1_SEL;
	int C_INPUT_2_1;
	int C_INPUT_2_2;
	int C_INPUT_2_3;
	int C_INPUT_2_4;
	int C_INPUT_2_5;
	int C_INPUT_2_6;
	int C_INPUT_2_7;
	int C_INPUT_2_8;
	int C_INPUT_2_9;
	int C_INPUT_2_SU;
	int C_INPUT_2_SD;
	int C_INPUT_2_START;
	int C_INPUT_2_SEL;
};