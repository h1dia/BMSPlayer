#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "DxLib.h"
#include "scene.h"
#include "define.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	// ���낻��f�U�C���p�^�[����׋�����ׂ����Ǝv���Ă�
	GameScene *scene = new GameScene(lpCmdLine);

	delete scene;
	return 0;
}