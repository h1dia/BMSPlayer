#pragma once
#include "DxLib.h"
#include "scene.h"
#include "player.h"
#include "define.h"

GameScene::GameScene(LPSTR lpCmdLine){
	cmd_line = lpCmdLine;
	// csv(xml)�p�[�T�Ăяo��

	vsync_flag = false;
	network_flag = false;

	SetGraphMode(1280, 720, 16);
	SetWindowSizeChangeEnableFlag(false);
	ChangeWindowMode(true);
	SetMainWindowText("�����́`�с`�� ������ǂ�������I");
	SetWaitVSyncFlag(vsync_flag);
	SetAlwaysRunFlag(true);

	if (DxLib_Init() == -1)
		dxlib_error = true;
	
	SetDrawScreen(DX_SCREEN_BACK);
	SetFontThickness(1);

	// �R�}���h���C�������`�F�b�N
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

	// �����f�[�^�\��
	vsync_flag ? printfDx("���� : �����������L���ł��B\n") : 0 ;
	network_flag ? printfDx("�l�b�g���[�N�͗L���ł�\n") : printfDx("�l�b�g���[�N�͖����ł�\n");
	GetJoypadNum() ? printfDx("�Q�[���p�b�h�� %d ��ڑ�����Ă��܂�\n", GetJoypadNum()) : printfDx("�Q�[���p�b�h�͐ڑ�����Ă��܂���\n");
	ScreenFlip();

	// �l�b�g���[�N�f�[�^������
	if (network_flag){
		;
	}

	// BMS �f�[�^�x�[�X�X�V��

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