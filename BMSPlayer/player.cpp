#pragma once
#include "DxLib.h"
#include "player.h"
#include "bmsparser.h"
#include "timer.h"
#include "FpsTimer.h"
#include "DXGraphic.h"

BmsPlayer::BmsPlayer(std::string bms_path){
	sound_handle.resize(1296);
	graph_handle.resize(1296);
	visnote_begin.resize(8, 0);
	visnote_next.resize(8, 0);
	visnote_size.resize(8, 0);
	before_graph_index = -1;
	data_out_range = std::make_pair(0, 0);
	index_count.resize(1296);
	visible_time = 500000;

	// parse
	parser = new BmsParser(bms_path);

	parser->parseBms();
	channel_array = parser->getInfoAllChannel();

	// note infomation
	for (int i = 0; i <= 7; i++){
		visnote_size.at(i) = visible_notes(i).size();
	}

	setSoundToMem();
	setGraphToMem();

	ScreenFlip();
	ClearDrawScreen();
}

BmsPlayer::~BmsPlayer(){
	delete parser;
}

// getoptions

void BmsPlayer::bmsPlay(){	
	// �Q�[���p�^�C�}�[
	ChronoTimer timer, calcflame;
	while (ProcessMessage() == 0)
	{
		// �v���[����
		play_channel_sound(1, timer.GetLapTime());
		for (int j = 11; j < 20; j++){
			play_channel_sound(j, timer.GetLapTime());
		}
		
		// ����
		
		// �`��u���b�N
		if (calcflame.GetLapTime() >= (1.0 / GetRefreshRate()) * 100000){
			system_graph.drawsystem();
			drawInterface(timer.GetLapTime());
			calcflame.ResetTime();
		}
	}

	// ���U���g�N���X�Ăяo���Ƃ�
	InitSoundMem();
}

void BmsPlayer::drawInterface(unsigned long long time){
	DrawFormatString(450, 60, GetColor(255, 255, 255), "GENRE : %s", parser->getHeader("GENRE").c_str());
	DrawFormatString(450, 75, GetColor(255, 255, 255), "TITLE : %s", parser->getHeader("TITLE").c_str());
	DrawFormatString(450, 90, GetColor(255, 255, 255), "ARTIST: %s", parser->getHeader("ARTIST").c_str());
	DrawFormatString(450, 105, GetColor(255, 255, 255), "BPM   : %s", parser->getHeader("BPM").c_str());
	DrawFormatString(450, 120, GetColor(255, 255, 255), "TIME  : %d", time);

	double visible_area;
	for (int j = 0; j <= 7; j++){
		for (int note_num = visnote_begin.at(j); note_num < visnote_size.at(j); note_num++){
			visible_area = ((double)visible_notes(j).at(note_num).second - time) / visible_time;
			// �͈͊O
			if (visible_area > 1){
				break;
			}
			if (visible_area < 0)
				visnote_begin.at(j)++;
			// �͈͓�
			else{
				// �`��
				double y = ((double)visible_notes(j).at(note_num).second - time) / visible_time;
				system_graph.drawkey(j, 480 - (y * 480));
			}
		}
	}

	ScreenFlip();
	ClearDrawScreen();
}

void BmsPlayer::bmsSoundTest(){
	ChronoTimer timer, calcflame;
	FpsTimer fps;
	std::vector<int> i(1296, 0);

	while (ProcessMessage() == 0){
		play_channel_sound(1, timer.GetLapTime());
		for (int j = 11; j < 20; j++){
			play_channel_sound(j, timer.GetLapTime());
		}

		// TODO : play_channel_graph�֐����߂�����d��
		//i.at(4) += play_channel_graph(4, i.at(4), timer.GetLapTime());

		if (calcflame.GetLapTime() >= (1.0 / GetRefreshRate()) * 1000000){
			DrawFormatString(0, 0, GetColor(255, 255, 255), "SoundTest");
			DrawFormatString(0, 15, GetColor(255, 255, 255), "GENRE : %s", parser->getHeader("GENRE").c_str());
			DrawFormatString(0, 30, GetColor(255, 255, 255), "TITLE : %s", parser->getHeader("TITLE").c_str());
			DrawFormatString(0, 45, GetColor(255, 255, 255), "ARTIST: %s", parser->getHeader("ARTIST").c_str());
			DrawFormatString(0, 60, GetColor(255, 255, 255), "BPM   : %s", parser->getHeader("BPM").c_str());
			DrawFormatString(0, 75, GetColor(255, 255, 255), "TIME  : %d", timer.GetLapTime());
			DrawFormatString(0, 90, GetColor(255, 255, 255), "����FPS : %d", fps.GetLapTime());
			DrawFormatString(0, 120, GetColor(255, 255, 255), "�𑜓x : %d(us)", fps.GetDiff());
			DrawFormatString(0, 135, GetColor(255, 255, 255), "RefLate : %d", GetRefreshRate());

			ScreenFlip();
			ClearDrawScreen();
			calcflame.ResetTime();
			fps.ResetTime();
		}
	}

	InitSoundMem();
}

void BmsPlayer::setSoundToMem(){
	std::vector<std::string> load_path = parser->getHeaderDataPath("WAV");
	for (unsigned int i = 0; i < load_path.size(); i++){
		if (LoadSoundMem(load_path.at(i).c_str()) != -1)
			sound_handle.at(i) = LoadSoundMem(load_path.at(i).c_str());
		else
			sound_handle.at(i) = LoadSoundMem((load_path.at(i).substr(0, load_path.at(i).find_last_of(".")) + ".ogg").c_str());
		if (ProcessMessage())
			break;
	}
}

void BmsPlayer::setGraphToMem(){
	std::vector<std::string> load_path = parser->getHeaderDataPath("BMP");
	for (unsigned int i = 0; i < load_path.size(); i++){
		graph_handle.at(i) = LoadGraph(load_path.at(i).c_str());

		if (ProcessMessage())
			break;
	}

}

int BmsPlayer::casc_DrawGraph(int channel, int index){
	try{
		if (!(before_graph_index == index))
			PlayMovieToGraph(graph_handle.at(channel_array.at(channel).at(index).first));

		DrawGraph(100, 150, graph_handle.at(channel_array.at(channel).at(index).first), FALSE);
	}
	catch (std::out_of_range&){
		return -1;
	}
	before_graph_index = index;
	return 0;
}

int BmsPlayer::play_channel_sound(int channel, unsigned long long time){
	std::pair<int, int> channel_data = ret_channel_array(channel, index_count.at(channel));

	if (channel_data.second <= time && channel_data.first != 0){
		PlaySoundMem(sound_handle.at(channel_data.first), DX_PLAYTYPE_BACK);
		index_count.at(channel)++;

		return 1;
	}

	return 0;
}

std::vector<std::pair<int, unsigned long long int>> BmsPlayer::visible_notes(int num){
	switch (num)
	{
	case 0:
		return channel_array.at(16);
	case 1:
		return channel_array.at(11);
	case 2:
		return channel_array.at(12);
	case 3:
		return channel_array.at(13);
	case 4:
		return channel_array.at(14);
	case 5:
		return channel_array.at(15);
	case 6:
		return channel_array.at(18);
	case 7:
		return channel_array.at(19);
	default:
		return channel_array.at(555);
	}
}

int BmsPlayer::play_channel_graph(int channel, int index, unsigned long long time){
	try{
		if (channel_array.at(channel).at(index).second <= time){
			casc_DrawGraph(channel, index);
		}
		else{
			casc_DrawGraph(channel, index - 1);
			return 0;
		}
	}
	catch (std::out_of_range&){
		return 0;
	}
	return 1;
}


// channel_array�̃��b�p�ł��B��������Ăяo���Ă�������
std::pair<int, unsigned long long int> BmsPlayer::ret_channel_array(int channel, int index){
	if (channel >= channel_array.size()){
		return data_out_range;
	} else if (index >= channel_array.at(channel).size()){
		return data_out_range;
	}

	return channel_array.at(channel).at(index);
}
