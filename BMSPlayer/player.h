#include "define.h"
#include "parser.h"
#include "array"
#include "ECSystem.h"
#include "DxLib.h"
#include "input.h"

class BmsParser;

class BmsPlayer{
public:
	BmsPlayer(std::string bms_path);
	~BmsPlayer();

	void bmsSoundTest();
	void setSoundToMem();
	void setGraphToMem();
	void deleteSoundToMem();

	void bmsPlay();
private:
	BmsParser *parser;
	ECSystem system_graph;
	DxInput input;


	std::pair <int, unsigned long long int> data_out_range;

	std::vector < std::vector<std::pair<int, unsigned long long>> > channel_array;
	std::vector<int> sound_handle;
	std::vector<int> graph_handle;
	std::vector<int> index_count;
	std::vector<int> visnote_begin;
	std::vector<int> visnote_size;
	std::vector<int> visnote_next;

	std::vector<int> notes; //現在の判定するべきノーツ
	int key1;

	int before_graph_index;

	int combo_debug;//削除予定.
	int casc_PlaySoundMem(int, int);
	int casc_DrawGraph(int, int);
	int play_channel_sound(int, unsigned long long);
	int play_channel_graph(int, int, unsigned long long);
	int visible_time; // 緑数字もどき 単位はusec, 緑数字変換は visible_time * 60 / 100000

	const static int visible_y = 300; // 表示幅(px)　使わない?

	void checkJudge(unsigned long long);
	void drawInterface(unsigned long long time);
	std::pair<int, unsigned long long int> ret_channel_array(int channel, int index);
	std::vector<std::pair<int, unsigned long long int>> visible_notes(int channel);
	int keymap(int);
};