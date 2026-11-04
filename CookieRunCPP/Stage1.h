#pragma once
#include "Field.h"
#include "Monster.h"
#include "Defiens.h"

class Stage1 : public Field
{
private:
	Monster monsters[MONSTER_COUNT]; //몬스터 총마리수
	float spawnTimer = 0.f; //스폰 타이머
	float spawnInterval = 2.f; //스폰 간격(초)

	void UpdateMonsters(float dt);   // 몬스터 업데이트
	void SpawnMonster();             // 몬스터 스폰

public:
	Stage1(RenderWindow& win);
	bool Run(Player& player) override; //Field의 Run함수 구현
};

