#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <list>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <unordered_map>

class BmsParser{
public:
	BmsParser(std::string filepath);
	~BmsParser();

	void parseBms();
	void loadheader();
	bool getParseError() { return fatal_error; };

	std::string getBmsFolder();
	std::vector<std::string> getHeaderDataPath(std::string);
	std::string getHeader(std::string command);
	double search_header_d(std::string command);
	int getsize(int channel);

	std::vector<std::vector<std::pair<int, unsigned long long>>> getInfoAllChannel();
private:
	std::string filename;
	std::string filefolder;
	static const int MAX_CHANNEL = 575;
	const char* kNotAvailable = "<N/A>";
	const std::string num_check = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	struct HEADER{
		std::string command;
		std::string str;
	};

	struct CHANNEL{
		unsigned short int measure;
		unsigned int step;			// 位置を示します。 ex.{1}/4
		unsigned int resolution;	// n/rhythm ex.4/{4}
		std::string id;
		double num;

		bool operator<(const CHANNEL& next) const	//ソート用のオペレータです。
		{
			return measure == next.measure ? (double)step / resolution < (double)next.step / next.resolution : measure < next.measure;
		}
	};

	int random(int max);
	void parse_random(std::vector<std::string>& temp_array, unsigned int from, unsigned int length);
	unsigned int find_endif(std::vector<std::string>& temp_array, unsigned int index);
	void header_analysis(std::vector<std::string>& header_array);
	int base_stoi(int base, std::string num);
	void normalize_data(std::vector<std::string>& data_array, std::vector<std::string> & normalized_array);
	unsigned long long calc_data_pos_usec(double bpm, double measure, int step, int resolution);
	static bool starts_with(std::string& str, std::string substr);
	bool judge_disused_command(std::string command);

	std::vector<std::vector<CHANNEL>> channel_data_array;
	std::unordered_map<std::string, std::string> header_map;
	bool load_fail;
	bool control_flow_exsist;
	std::string bmsfolder;
	std::string extension;
	std::string banner;
	std::string title;
	std::mt19937 random_engine;
	bool fatal_error;
};
