#pragma once
#include "DxLib.h"
#include "scene.h"
#include "player.h"
#include "define.h"

GameScene::GameScene(LPSTR lpCmdLine){
	cmd_line = lpCmdLine;
	// csv(xml)パーサ呼び出し

	vsync_flag = false;
	network_flag = false;

	SetGraphMode(1280, 720, 16);
	SetWindowSizeChangeEnableFlag(false);
	ChangeWindowMode(true);
	SetMainWindowText("えくは～び～と せかんどすたいる！");
	SetWaitVSyncFlag(vsync_flag);
	SetAlwaysRunFlag(true);

	if (DxLib_Init() == -1)
		dxlib_error = true;
	
	SetDrawScreen(DX_SCREEN_BACK);
	SetFontThickness(1);

	// コマンドライン引数チェック
	if (*lpCmdLine == 0){
		cmd_line = "C:\\Users\\h1dia.LB-S231X\\Documents\\_bms\\_spackage\\[est]angelic_snow\\as_air_extreme.bme";
		boot();
	}
	else
		start_player();
}

GameScene::~GameScene(){
	DxLib_End();
}

int GameScene::boot(){
	printfDx("EXHbeat 2nd style version %d\n\n", THIS_VERSION);

	// 内部データ表示
	vsync_flag ? printfDx("注意 : 垂直同期が有効です。\n") : 0 ;
	network_flag ? printfDx("ネットワークは有効です\n") : printfDx("ネットワークは無効です\n");
	GetJoypadNum() ? printfDx("ゲームパッドは %d 台接続されています\n", GetJoypadNum()) : printfDx("ゲームパッドは接続されていません\n");
	ScreenFlip();

	// ネットワークデータ処理部
	if (network_flag){
		;
	}

	// BMS データベース更新部

	printfDx("\nPRESS ANY KEY\n");
	ScreenFlip();

	WaitKey();

	printfDx("READY\n");
	ScreenFlip();

	WaitTimer(100);

	clsDx();
	return 0;
}

int GameScene::start_menu(){
	return 0;
}

void GameScene::scene_loop(){
}

int GameScene::start_player(){
	BmsPlayer player(cmd_line);
	player.bmsPlay();

	return 0;
}