#include "FileSave.h"

void FileSave::SaveGame(Player& player, bool stage1Cleared, Inventory& inventory)
{
	ofstream saveFile("save_CookieRun.txt");
	if (saveFile.is_open())
	{
		// 1 - 기본 정보 저장
		saveFile << player.GetGold() << endl;
		saveFile << stage1Cleared << endl;

		// 2 - 인벤토리 저장(연결리스트 순회)
		ItemNode* curr = inventory.GetListHead();
		while (curr != nullptr)
		{
			saveFile << "ITEM" << endl;
			saveFile << curr->info.name << endl;
			saveFile << curr->count << endl;
			curr = curr->next;
		}
		saveFile << "END_INVEN" << endl;
		saveFile.close();
	}
}

bool FileSave::LoadGame(Player& player, bool& stage1Cleared, Inventory& inventory)
{
	ifstream loadFile("save_CookieRun.txt");
	if (!loadFile.is_open())
	{
		return false;
	}

	int gold;
	bool cleared;

	// 1 - 기본 정보 읽기
	loadFile >> gold;
	player.SetGold(gold);

	loadFile >> cleared;
	stage1Cleared = cleared;

	// 2 - 인벤토리 읽기
	inventory.ClearAll(); //기존 인벤토리 비우기
	string marker;
	while (loadFile >> marker && marker == "ITEM")
	{
		string itemName;
		int count;
		loadFile.ignore(); //버퍼 지우기
		getline(loadFile, itemName);
		loadFile >> count;

		// 아이템 정보(ItemINFO, Item)를 이름에 따라 다시 매칭하여 AddItem 호출
		// 상점의 InitShopItems 로직을 참고하여 아이템 객체를 생성해 넣어주어야 합니다.

		if (itemName == "Rare Potion")
		{
			ItemINFO info = { POTION, "Rare Potion", 50 };
			Item data = { 2 };
			for (int i = 0; i < count; i++)
			{
				inventory.AddItem(info, data);
			}
		}
		else if (itemName == "Unique Potion (Elixir)")
		{
			ItemINFO info = { POTION, "Unique Potion (Elixir)", 125 };
			Item data = { 4 };
			for (int i = 0; i < count; i++)
			{
				inventory.AddItem(info, data);
			}
		}
	}
	loadFile.close();
	return true;
}
