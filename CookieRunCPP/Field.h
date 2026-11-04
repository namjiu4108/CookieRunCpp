#pragma once
#include "Defiens.h"
#include "Struct.h"
#include "GameText.h"
#include "Player.h"
#include "Monster.h"
#include "Gold.h"
#include <list>

class Field
{
protected:
	static const int TILE_COUNT = 20;
	static const int MONSTER_COUNT = 10;
	static const int GOLD_COUNT = 10;

	RenderWindow& window;
	Texture bgTexture;
	Texture tileTexture[4];
	Sprite tile[TILE_COUNT];
	Sprite bg[2]; //배경 2장
	float bgSpeed = 200.f; //배경의 이동속도
	float bgWidth = 0.f; //배경 1장 너비
	float tileW = 0.f; //실제 타일 너비 scale 적용

	//골드
	Gold gold[GOLD_COUNT]; //골드 총 생성수
	float GspawnTimer = 0.f; //스폰 타이머
	float GspawnInterval = 2.f; //스폰 간격(초)

	//제한시간
	float stageTIme = 60.f; //스테이지 제한 시간
	float timeLeft = 60.f; //남은 시간
	bool stageClear = false; //클리어 여부
	bool gameover = false; //게임 오버 체크

	Clock clock;
	
	void ScrollBG(float dt); //배경 스크롤 처리
	void ScrollTile(float dt); //타일 스크롤 처리

	void UpdateGold(float dt);	// 골드 업데이트
	void SpawnGold();        // 골드 스폰

	void Collision(Player& player, float dt); //충돌 감지

public:
	Field(RenderWindow& win);
	virtual bool Run(Player& player) = 0; //게임 루프 + 배경 // 상속을 사용하기 가상함수
};