#pragma once
#include "Defiens.h"
#include "Struct.h"
#include "GameText.h"
#include "Player.h"
#include "Inventory.h"

class Shop {
private:
    RenderWindow& window;
    Inventory& inventory;   // 인벤토리 참조 (구매 시 아이템 추가용)
    GameText gametext;

    //텍스쳐
    Texture bgTexture;
    Texture shoptitleTexture;
    Texture rareTexture[4];
    Texture uniTexture[4];

    //스프라이트
    Sprite bg;
    Sprite rareSprite;
    Sprite uniSprite;
    Sprite titleSprite[3];

    //애니메이션
    float aniTimer = 0.f;
    int aniFrame = 0;
    Clock aniClock;
    // 연결 리스트 헤드 (판매 목록)
    ItemNode* shopListHead = nullptr;

    // 아이템 추가 함수
    void AddShopItem(ItemINFO info, Item data);
    // 판매 목록 초기화
    void InitShopItems();

public:
    // 생성자에서 보스 스테이지 클리어 여부를 받음
    Shop(RenderWindow& win, Inventory& inven);
    // 소멸자에서 동적 할당된 연결 리스트 메모리 해제
    ~Shop();

    // 상점 메인 루프 (플레이어의 골드와 상호작용)
    void ItemSelect(Player& player);
};