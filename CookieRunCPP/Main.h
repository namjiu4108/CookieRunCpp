#pragma once
#include "Defiens.h"
#include "Field.h"
#include "GameText.h"
#include "Menu.h"
#include "Stage1.h"
#include "BossStage.h"
#include "StageSelect.h"
#include "Shop.h"
#include "Inventory.h"
#include "FileSave.h"

class Main
{
private:
	RenderWindow window;
	Texture introTexture;
	Sprite introbg; //인트로 배경

	// 게임 전체에서 공유되는 인벤토리
	Inventory inventory;

public:
	Main();
	void Start();
	void Intro();
};

