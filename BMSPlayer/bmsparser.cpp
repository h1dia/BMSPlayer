// BMS 関連の用語は以下にて参照ください。
// BMS command memo (JP) - http://hitkey.nekokan.dyndns.info/cmdsJP.htm

#pragma once
#include "bmsparser.h"
#include <chrono>
#include <unordered_map>

BmsParser::BmsParser(std::string filepath){
	// 前後のクォートがあれば削除
	if (filepath.substr(0, 1) == "\"")
		filepath.erase(0, 1);
	if (filepath.substr(filepath.length(), 1) == "\"")
		filepath.erase(filepath.length(), 1);

	// ファイルパス読み込み
	filename = filepath.substr(filepath.find_last_of("\\") + 1);
	filefolder = filepath.substr(0, filepath.find_last_of("\\") + 1);

	// vector 初期化
	channel_data_array.resize(MAX_CHANNEL);

	// パースエラーチェック 初期化
	fatal_error = false;

	// ランダムエンジン初期化
	std::random_device r;
	random_engine = std::mt19937(r());
}

BmsParser::~BmsParser(){
}


void BmsParser::parseBms(){

	// ファイル入力
	std::ifstream ifs(filefolder + filename);
	std::string tempstring;
	std::vector<std::string> temp_array, header_array, channel_array;
	
	// 失敗時処理
	if (ifs.fail()){
		load_fail = true;
		return;
	}
	else
		load_fail = false;

	// temp_array にコマンド行のみを格納
	while (getline(ifs, tempstring))
	{
		if (tempstring.find_first_of("#") == 0){
			// #の削除
			tempstring.erase(0, 1);
			temp_array.push_back(tempstring);

		}
	}
	
	// CONTROL FLOWの処理
	parse_random(temp_array, 0, temp_array.size());

	// 配列への振り分け
	for (unsigned i = 0; i < temp_array.size(); i++){
		
		if (control_flow_exsist){
			if (judge_disused_command(temp_array.at(i)))
				continue;
		}

		if (std::all_of(temp_array.at(i).cbegin(), temp_array.at(i).cbegin() + 5, isdigit))
			channel_array.push_back(temp_array.at(i));
		else
			header_array.push_back(temp_array.at(i));
	}

	// HEADER 文解析
	header_analysis(header_array);

	// CHANNEL 文解析
	for (unsigned i = 0; i < channel_array.size(); i++){
		// コマンドの大文字変換
		std::transform(channel_array.at(i).cbegin(), channel_array.at(i).cend(), channel_array.at(i).begin(), toupper);

		CHANNEL temp = {};

		temp.measure = stoi(channel_array.at(i).substr(0, 3));

		int channel = stoi(channel_array.at(i).substr(3, 2));
		try{
			// #xxx02 では小数のみが与えられる
			if (channel == 2){
				temp.resolution = 1; // div 0対策
				temp.step = 0;

				temp.num = stod(channel_array.at(i).substr(6));
				channel_data_array.at(channel).push_back(temp);
			}
			else {
				//解像度の計算
				temp.resolution = channel_array.at(i).substr(6).length() / 2;
	
				//ID 及びその位置の計算
				for (unsigned j = 0; j < temp.resolution; j++){
					temp.id = channel_array.at(i).substr(6 + j * 2, 2);
					//ID 00 を無視する
					if (temp.id != "00"){
						temp.step = j;
						channel_data_array.at(channel).push_back(temp);
					}
				}
			}
		}
		// ch2に数字以外が突っ込まれているとき(コマンドの無効化)
		catch (std::invalid_argument&){
			temp.resolution = 1;
			temp.step = 0;
			temp.num = 1;
		}

	}

	// ソート
	for (int i = 0; i < MAX_CHANNEL; i++){
		if (channel_data_array.at(i).size())
			std::sort(channel_data_array.at(i).begin(), channel_data_array.at(i).end());
	}

	return;
}

// std::string で記述された任意の進数の値を、10進数で返します。
// 引数:
//		str: 任意の進数で記述された値。
//		base: str の進数。
inline int BmsParser::base_stoi(int base, std::string str){
	int ans = 0;

	for (unsigned int i = 0; i < str.length(); i++){
		for (int j = 0; j < base; j++){
			if (str.at(i) == num_check.at(j))
				ans += (int)(j * pow(36, str.length() - 1 - i));
		}
	}

	return ans;
}

// 1 から max までの範囲で乱数を返します。
// 引数:
//     max: 乱数の最大値。
int BmsParser::random(int max){
	return random_engine() % max + 1;
}

// RANDOM 命令をパースします。
// 引数:
//     temp_array: BMS 命令の vector。
//     from: パースする範囲の先頭位置。
//     length: パースする範囲の長さ。
// 詳細:
//     RANDOM 命令を評価し、IF 命令で条件に一致しないものを利用不可としてマークします。
//     ネストされた RANDOM、IF 命令をパースできます。temp_array への変更は破壊的変更です。
void BmsParser::parse_random(std::vector<std::string>& temp_array, unsigned int from, unsigned int length){
	control_flow_exsist = false;
	int random_value = 0;

	for (unsigned int i = from; i < from + length; i++){
		if (starts_with(temp_array.at(i), "RANDOM")){
			control_flow_exsist = true;
			// RANDOM 値の生成
			int random_max = stoi(temp_array.at(i).substr(7));
			random_value = random(random_max);
		}
		else if (starts_with(temp_array.at(i), "IF")){
			// IF 値の取得
			int value = stoi(temp_array.at(i).substr(3));

			unsigned int endif = find_endif(temp_array, i);
			if (random_value == value){
				// 条件にヒットしたとき
				// 再帰呼び出しでネストされた分をパース
				parse_random(temp_array, i + 1, endif - i - 1);
				// 次を評価するためにカウンタを操作
				i = endif;
			}
			else {
				// 条件にヒットしなかったとき
				// IF から ENDIF の間に利用不可としてマーク
				for (unsigned int j = i; j <= endif; j++){
					temp_array[j] = kNotAvailable;
				}
				// 次を評価するためにカウンタを操作
				i = endif;
			}
		}
	}
}

// 後方の ENDIF 命令の位置を検索します。
// 引数:
//     temp_array: BMS 命令の vector。
//     index: 検索する ENDIF 命令に対応する IF 命令の位置。
// 戻り値:
//     指定した IF 命令に対応する ENDIF 命令の位置。
unsigned int BmsParser::find_endif(std::vector<std::string>& temp_array, unsigned int index){
	int level = 0;
	for (unsigned int i = index + 1; i < temp_array.size(); i++){
		if (starts_with(temp_array.at(i), "IF")){
			level++;
		}
		//#ENDIF #END IF #IFEND は全て同義です。
		else if (starts_with(temp_array.at(i), "END") || starts_with(temp_array.at(i), "IFEND")){
			if (level == 0)
				return i;
			else
				level--;
		}
	}
	return temp_array.size() - 1;
}

// str が substr で始まる文字列かどうかを返します。
// 引数:
//     str: 評価される文字列。
//     substr: 評価する文字列。
// 戻り値:
//     str が substr で始まるなら true、そうでないなら false。
bool BmsParser::starts_with(std::string& str, std::string substr){
	return str.substr(0, substr.length()) == substr;
}

// ID と PATH が記述された命令を正規化します。
// 引数:
//		data_array: ID と PATH が記述されている命令の vector
//		normalized_array: 正規化された命令を格納する vector
void BmsParser::normalize_data(std::vector<std::string> &data_array, std::vector<std::string> &normalized_array){
	for (unsigned i = 0; i < data_array.size(); i++){
		std::string id = data_array.at(i).substr(3, 2);
		std::transform(id.cbegin(), id.cend(), id.begin(), toupper);
		int tempid = base_stoi(36, id);
		std::string temppath = data_array.at(i).substr(6);

		normalized_array.at(tempid) = temppath;
	}
}

// HEADER, CHANNEL 命令をパースする上で、不要な命令かどうかを判定します。
// parse_random 関数よりも先に呼び出した場合は未定義の動作です。
// 引数:
//     command: 評価される文字列。
// 戻り値:
//     command が不要なコマンドの条件に一致するなら true、そうでないなら false。
bool BmsParser::judge_disused_command(std::string command){
	if (kNotAvailable == command
		|| starts_with(command, "RANDOM")
		|| starts_with(command, "IF")
		|| starts_with(command, "ELSEIF")
		|| starts_with(command, "ENDIF")
		|| starts_with(command, "SETRANDOM")
		|| starts_with(command, "ENDRANDOM")
		)
		return true;
	else
		return false;
}


// HEADER 命令を、すべて header_map に格納します。
// 重複する命令は EOF に近い命令が優先されます。
// 引数:
//		headder_array: <HEADER> に属した BMS 命令の vector
void BmsParser::header_analysis(std::vector<std::string>& header_array){
	HEADER temp;
	for (unsigned int i = 0; i < header_array.size(); i++){
		std::string header = header_array.at(i);

		temp.command = header.substr(0, header.find_first_of(" "));
		temp.str = header.substr(header.find_first_of(" ") + 1);

		header_map.emplace(temp.command, temp.str);
	}
}

// HEADER 命令の内容を string 型で取得します。
// 引数:
//     command: 呼び出すべき命令の内容。
// 戻り値:
//     命令の内容が存在していれば内容の string、そうでないなら kNotAvailable。
std::string BmsParser::getHeader(std::string command){
	if (header_map[command].empty())
		return kNotAvailable;
	else
		return header_map[command];
}

// ファイルパスを string 型で取得します。
// 戻り値:	フォルダの絶対パス
std::string BmsParser::getBmsFolder(){
	return filefolder;
}

// パースされた CHANNEL 命令の内容を、pair<string, unsigned long long int> 型の2次元 vector に変換して返します。
// BPM 命令が読み込まれていない場合、130 が適用されます。
// 恐らく重いので、1つのファイルに対して1度だけ呼び出すようにしてください。
// 戻り値:
//		ret_channel.at(channel).at(sorted_num).first 該当する命令の ID
//		ret_channel.at(channel).at(sorted_num).second 該当する命令の呼び出すべき時刻(us)
std::vector < std::vector<std::pair<int, unsigned long long>> > BmsParser::getInfoAllChannel(){
	// 以下のチャンネルを、時間順にソートした状態で読み込みます。
	// 03 : BPM 変更
	// 08 : exBPM
	std::vector<CHANNEL> sorted_bpm_change(channel_data_array.at(3).size() + channel_data_array.at(8).size());
	CHANNEL temp_bpm;

	for (unsigned int i = 0; i < channel_data_array.at(3).size(); i++){
		temp_bpm.measure = channel_data_array.at(3).at(i).measure;
		temp_bpm.step = channel_data_array.at(3).at(i).step;
		temp_bpm.resolution = channel_data_array.at(3).at(i).resolution;
		temp_bpm.num = base_stoi(36, channel_data_array.at(3).at(i).id);

		sorted_bpm_change.push_back(temp_bpm);
	}
	for (unsigned int i = 0; i < channel_data_array.at(8).size(); i++){
		temp_bpm.measure = channel_data_array.at(8).at(i).measure;
		temp_bpm.step = channel_data_array.at(8).at(i).step;
		temp_bpm.resolution = channel_data_array.at(8).at(i).resolution;
		temp_bpm.num = base_stoi(36, channel_data_array.at(8).at(i).id);

		sorted_bpm_change.push_back(temp_bpm);
	}
	try{
		temp_bpm.num = stod(getHeader("BPM"));
	}
	catch (std::invalid_argument&){
		temp_bpm.num = 130;
	}

	temp_bpm.measure = 0;
	temp_bpm.step = 0;
	temp_bpm.resolution = 1;
	sorted_bpm_change.push_back(temp_bpm);

	if (sorted_bpm_change.size())
		std::sort(sorted_bpm_change.begin(), sorted_bpm_change.end());	
	
	// sorted_bpm_change を時間軸(us)に変換します。
	// .first が BPM, .second が 時刻(us)を表します。
	std::vector<std::pair<double, unsigned long long>> bpm_change(100);
	std::pair<double, unsigned long long> now_bpm;
	for (unsigned int i = 0; i < sorted_bpm_change.size(); i++){
		unsigned long long calc_bpm_usec_sum = 0;
		int measure_sum = 0;

		// 単純な時間の計算式から小節の変更があったときに、引くべきだけの小節の長さの合計
		// ex) 小節長5のうち、#00102:0.5があると 0.5 が代入されます。
		double change_measure_diff = 0;

		// 02 : #xxxの長さ変更 についてある範囲で検索し、引くべき時間を代入
		for (unsigned int mi = measure_sum; mi < sorted_bpm_change.at(mi).measure; mi++){
			for (unsigned int j = 0; j < channel_data_array.at(2).size(); j++){
				if (channel_data_array.at(2).at(j).measure == mi)
					change_measure_diff += channel_data_array.at(2).at(j).num;
			}
		}

		calc_bpm_usec_sum += calc_data_pos_usec(now_bpm.first, sorted_bpm_change.at(i).measure - measure_sum - change_measure_diff, sorted_bpm_change.at(i).step, sorted_bpm_change.at(i).resolution);
		
		measure_sum += sorted_bpm_change.at(i).measure;
		now_bpm.first = sorted_bpm_change.at(i).num;
		now_bpm.second = calc_bpm_usec_sum;
		bpm_change.push_back(now_bpm);
	}

	std::vector < std::vector<std::pair<int, unsigned long long>> > ret_channel(MAX_CHANNEL);
	// データ位置の計算
	for (int chi = 0; chi < MAX_CHANNEL; chi++){
		std::pair<int, unsigned long long> temp_data;
		int bi = 0; // BPMカウンター

		for (unsigned int i = 0; i < channel_data_array.at(chi).size(); i++){

			if (chi != 2)
				temp_data.first = base_stoi(36, channel_data_array.at(chi).at(i).id);
			else
				temp_data.first = channel_data_array.at(chi).at(i).num;

			// 時間計算
			// BPM,小節変化については考慮していません
			temp_data.second = calc_data_pos_usec(stod(getHeader("BPM")), channel_data_array.at(chi).at(i).measure, channel_data_array.at(chi).at(i).step, channel_data_array.at(chi).at(i).resolution);
			ret_channel.at(chi).push_back(temp_data);
		}
	}

	return ret_channel;
}

// データの位置を µsec で返します。
unsigned long long BmsParser::calc_data_pos_usec(double bpm, double measure, int step, int resolution){
	unsigned long long ret_data_pos_msec = (double)(60 * 4 * (measure + ((double)step / resolution)) / bpm) * 1000000;
	return ret_data_pos_msec;
}

// header_command で始まる名前のヘッダ命令の内容を、ファイルパスと結合してstringのvectorで返します。
// ex)	#WAVAB AAA.wav　があれば、
//		ret_path_data_array.at(x) == "c:\hoge\AAA.wav" です。
// ソートはされていないので注意してください。 
std::vector<std::string> BmsParser::getHeaderDataPath(std::string header_command){
	std::vector<std::string> ret_path_data_array(1296);

	for (auto itr = header_map.begin(); itr != header_map.end(); itr++){
		if (itr->first.substr(0, 3) == header_command)
			ret_path_data_array.at(base_stoi(36, itr->first.substr(3))) = filefolder + itr->second;
	}

	return ret_path_data_array;
};