#include <iostream>
#include <string>
#include <vector>

class GameScene{
public:
	GameScene(LPSTR lpCmdLine);
	~GameScene();
	
private:
	void scene_loop();
	int boot();
	int start_player();
	int start_menu();

	bool dxlib_error;
	bool vsync_flag;
	bool network_flag;
	std::string cmd_line;
};