#pragma once
#include "Defiens.h"
#include "GameText.h"
#include "Inventory.h"

enum StageResult
{
    Stage1Clear,
    GoBossStage,
    BackToMenu
};

class StageSelect
{
private:
    RenderWindow& window;
    Inventory& inventory; // 포션 선택용

    //텍스쳐
    Texture SbgTexture;
    //스프라이트
    Sprite stagebg;

    bool stage1Cleared = false;  // 스테이지 1 클리어 여부
    GameText gametext;

public:
    StageSelect(RenderWindow& win, bool cleared, Inventory& inven);
    StageResult Select(Player& player);// 포션 선택 포함하므로 Player도 받음
};