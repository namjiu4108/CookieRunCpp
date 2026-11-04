#pragma once
#include "Defiens.h"
#include "GameText.h"

enum MenuResult
{
	Stage,
	itemShop,
	Inven,
	Save,
	Load,
	Exit
};

class Menu
{
private:
	RenderWindow& window;
	GameText gametext;

	Texture bgTexture;
	Sprite bg; //배경

	//버튼
	Texture btnTexture;
	Sprite btnSprite[6]; //버튼 6개

public:
	Menu(RenderWindow& win);
	MenuResult Select(); //선택 결과 반환 함수
};

