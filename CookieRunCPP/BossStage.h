#pragma once
#include "Field.h"
#include "Defiens.h"
#include "BossMonster.h"
class BossStage : public Field
{
private:
	BossMonster boss;
	bool gameStarted = false; //스폰 완료 후 게임 시작 여부
	void ShowBossRoom();

public:
	BossStage(RenderWindow& win);
	bool Run(Player& player) override; //Field의 Run함수 구현

};

