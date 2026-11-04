#pragma once
#include "Defiens.h"

enum PlayerState
{
	IDLE,
	RUN,
	JUMP,
	DOUBLE_JUMP,
	SLIDE,
	PSKILL,
	DIE
};

enum BossState
{
	BOSSIDLE,
	TRANSFORM,
	SKILL
};

enum ItemType
{
	POTION,
	SKIN
};

typedef struct PlayerINFO
{
	string name; //이름(플레이어, 몬스터 공용)
	int hp; //플레이어 체력
	int Gold; //골드
}pinfo;

typedef struct ItemINFO
{
	ItemType type; //아이템 종류 구분
	string name; //아이템 이름
	int price; //아이템 가격
}iteminfo;

typedef struct Item
{
	int addHp; //아이템 체력 회복(일반 포션 현재 체력에서 +2)(유니크 포션 현재체력 +4) 일시적
}item;

// 아이템 노드 (상점 및 인벤토리 공용 연결 리스트)
struct ItemNode {
	iteminfo info;      // 이름, 가격, 타입
	item data;          // 회복량, 스킨 경로
	int count = 0;      // 보유 수량
	ItemNode* next = nullptr;
	//ItemNode* next;

	ItemNode(iteminfo i, item d) : info(i), data(d), next(nullptr), count(1) {}
};

struct PotionEntry
{
	string name;
	int addHp;
	int count;
};