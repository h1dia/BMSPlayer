#include "define.h"
#include "parser.h"
#include "array"
#include "ECSystem.h"

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

	std::pair <int, unsigned long long int> data_out_range;

	std::vector < std::vector<std::pair<int, unsigned long long>> > channel_array;
	std::vector<int> sound_handle;
	std::vector<int> graph_handle;
	std::vector<int> index_count;
	std::vector<int> visnote_begin;
	std::vector<int> visnote_size;
	std::vector<int> visnote_next;

	int before_graph_index;

	int casc_PlaySoundMem(int, int);
	int casc_DrawGraph(int, int);
	int play_channel_sound(int, unsigned long long);
	int play_channel_graph(int, int, unsigned long long);
	int visible_time; // —Î”š‚à‚Ç‚« ’PˆÊ‚Íusec, —Î”š•ÏŠ·‚Í visible_time * 60 / 100000

	const static int visible_y = 300; // •\¦•(px)

	void drawInterface(unsigned long long time);

	std::pair<int, unsigned long long int> ret_channel_array(int channel, int index);
	std::vector<std::pair<int, unsigned long long int>> visible_notes(int channel);

};