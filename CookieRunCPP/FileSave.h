#pragma once
#include "Defiens.h"
#include "Struct.h"
#include "Player.h"
#include "Inventory.h"
#include <fstream> //파일 입출력을 위한 헤더

class FileSave
{
public:
	//저장 -> 플레이어 정보, 스테이지 클리어 여부, 인벤토리 데이터
	static void SaveGame(Player& player, bool stage1Cleared, Inventory& inventory);

    // 불러오기 -> 파일에서 데이터를 읽어와 객체들에 적용
    static bool LoadGame(Player& player, bool& stage1Cleared, Inventory& inventory);
};

