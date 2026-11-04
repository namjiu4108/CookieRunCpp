#pragma once
#include "Defiens.h"
#include "Struct.h"
#include "GameText.h"
#include "Player.h"

class Inventory
{
private:
	RenderWindow& window;
	GameText gametext;

	//텍스쳐
	Texture ibgTexture; //인벤토리 배경
	Texture invenTexture; //인벤토리cell -> 아이템 이미지가 들어갈 이미지

	//스프라이트
	Sprite invenbg;
	Sprite invenSprite; //인벤토리cell -> 아이템 이미지들이 들어갈 이미지들
	Sprite cellSprite;
	Sprite itemSprite;

	// 아이템 이미지
	Texture rareTexture[4];
	Texture uniTexture[4];
	float aniTimer = 0.f;
	int aniFrame = 0;

	Clock aniClock;

	// 셀 레이아웃 설정
	static const int COLS = 5;
	static const int ROWS = 4;
	float cellSize = 90.f;
	float startX = 150.f;
	float startY = 120.f;
	float padding = 10.f;

	// 연결 리스트 헤드
	ItemNode* invenListHead = nullptr;

	void DrawCell(int index, ItemNode* node, int currentAniFrame); // 개별 칸 그리기
public:
	//생성자
	Inventory(RenderWindow& win);
	//소멸자
	~Inventory();

	void ClearAll(); // 로드 전 초기화용

	// 인벤토리 메인 루프 (플레이어와 상호작용)
	void invenSelect(Player& player);

	// 아이템 추가 (같은 이름이면 count++)
	void AddItem(ItemINFO info, Item data);

	// 아이템 차감 (스테이지 진입 시 포션 사용)
	bool ConsumeItem(const string& name);

	// 포션 사용 화면 (스테이지 진입 전)
	// 반환값: 선택한 포션의 addHp (0이면 사용 안 함 / -1이면 창 닫힘)
	int PotionSelect();

	// 보유 여부 확인
	bool HasItem(const string& name);

	// 연결 리스트의 시작 노드(헤드)를 반환하는 Getter
	ItemNode* GetListHead() const { return invenListHead; }
};

