#include "DxLib.h"
#include <vector>

class DxInput{
public:
	DxInput();
	~DxInput();

	void inputUpdate();
	int keyStatus(int key);

private:

	char key_buffer[256];
	std::vector<long int> key_status;
	std::vector<int> input_status; //ƒL[î•ñŠi”[—p

	int joypad_state;
	int num_joypad_input;
	int num_key_input;
};