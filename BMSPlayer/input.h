#include "DxLib.h"
#include "timer.h"
#include <vector>

class DxInput : public ChronoTimer{
public:
	DxInput();
	~DxInput();

	void inputUpdate();
	unsigned long long getInputTime(int key);
	bool isUpdateStatus(int key);

private:

	char key_buffer[256];
	std::vector<unsigned long long> key_status;
	std::vector<std::pair<bool, unsigned long long>> input_status; //ƒL[î•ñŠi”[—p

	int joypad_state;
	int num_joypad_input;
	int num_key_input;
};