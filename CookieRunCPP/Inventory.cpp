#include "Inventory.h"
#include <algorithm>
Inventory::Inventory(RenderWindow& win)
	:window(win)
{
    //인벤토리 배경 설정
    ibgTexture.loadFromFile("UI/inventory_bg.png");
    invenbg.setTexture(ibgTexture);
    float scaleX = 1500.f / ibgTexture.getSize().x;
    float scaleY = 600.f / ibgTexture.getSize().y;
    invenbg.setScale(scaleX, scaleY);

    // 셀 이미지
    invenTexture.loadFromFile("UI/InventoryCell.png");

    // 아이템 이미지
    for (int i = 0; i < 4; i++)
    {
        rareTexture[i].loadFromFile("Items/hp_potion_" + to_string(i) + ".png");
        uniTexture[i].loadFromFile("Items/Elixir_" + to_string(i) + ".png");
    }
}

Inventory::~Inventory()
{
    ClearAll();
}

void Inventory::ClearAll()
{
    ItemNode* curr = invenListHead;
    while (curr != nullptr)
    {
        ItemNode* next = curr->next;
        delete curr;
        curr = next;
    }
    invenListHead = nullptr;
}

void Inventory::AddItem(ItemINFO info, Item data)
{
	// 같은 이름 아이템이 있으면 count++
	ItemNode* curr = invenListHead;
	while (curr != nullptr)
	{
		if (curr->info.name == info.name)
		{
			curr->count++;
			return;
		}
		curr = curr->next;
	}

	// 없으면 새 노드 추가
	ItemNode* newNode = new ItemNode(info, data);
	if (invenListHead == nullptr)
	{
		invenListHead = newNode;
	}
	else
	{
		ItemNode* temp = invenListHead;
		while (temp->next != nullptr) temp = temp->next;
		temp->next = newNode;
	}
}

bool Inventory::ConsumeItem(const string& name)
{
	ItemNode* curr = invenListHead;
	ItemNode* prev = nullptr;
	while (curr != nullptr)
	{
		if (curr->info.name == name)
		{
			curr->count--;
			if (curr->count <= 0)
			{
				// 노드 제거
				if (prev == nullptr)
				{
					invenListHead = curr->next;
				}
				else
				{
					prev->next = curr->next;
				}
				delete curr;
			}
			return true;
		}
		prev = curr;
		curr = curr->next;
	}
	return false;
}

bool Inventory::HasItem(const string& name)
{
	ItemNode* curr = invenListHead;
	while (curr != nullptr)
	{
		if (curr->info.name == name && curr->count > 0)
		{
			return true;
		}
		curr = curr->next;
	}
	return false;
}

void Inventory::DrawCell(int index, ItemNode* node, int currentAniFrame)
{
	int col = index % COLS;
	int row = index / COLS;

	float x = startX + col * (cellSize + padding);
	float y = startY + row * (cellSize + padding);

	// 셀 배경
	cellSprite.setTexture(invenTexture);
	cellSprite.setScale(cellSize / (float)invenTexture.getSize().x, cellSize / (float)invenTexture.getSize().y);
	cellSprite.setPosition(x, y);
	window.draw(cellSprite);

	if (node == nullptr) return;

	// 아이템 이미지
	if (node->info.name == "Rare Potion")
	{
		itemSprite.setTexture(rareTexture[currentAniFrame]);
	}
	else if (node->info.name == "Unique Potion (Elixir)")
	{
		itemSprite.setTexture(uniTexture[currentAniFrame]);
	}
	else
	{
		return;
	}

	// 아이템 이미지를 셀 안에 맞게 크기 조정 (셀의 70%)
	float imgSize = cellSize * 0.70f;
	Vector2u texSize = itemSprite.getTexture()->getSize();
	itemSprite.setScale(imgSize / (float)texSize.x, imgSize / (float)texSize.y);

	// 셀 중앙에 배치
	float offset = (cellSize - imgSize) / 2.f;
	itemSprite.setPosition(x + offset, y + offset);
	window.draw(itemSprite);

	// 개수 표시 (오른쪽 아래 구석)
	if (node->count > 1)
	{
		gametext.SetCountText(window, to_string(node->count), x + cellSize, y + cellSize);
	}
}

void Inventory::invenSelect(Player& player)
{
	// 타이틀 텍스트
	gametext.SetInvenTitle("INVENTORY");
	// esc 텍스트
	gametext.SetGuide("ESC - Back");

	while (window.isOpen())
	{
		float dt = aniClock.restart().asSeconds();
		aniTimer += dt;
		if (aniTimer >= 0.15f)
		{
			aniTimer = 0.f;
			aniFrame = (aniFrame + 1) % 4;
		}

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape) return;
			}
		}

		window.clear();
		window.draw(invenbg);
		gametext.RenderInventory(window);

		// 인벤토리 칸 그리기 (전체 ROWS*COLS개)
		ItemNode* curr = invenListHead;
		int totalCells = ROWS * COLS;
		for (int i = 0; i < totalCells; i++)
		{
			DrawCell(i, curr, aniFrame);
			if (curr != nullptr) curr = curr->next;
		}
		window.display();
	}
}

int Inventory::PotionSelect()
{
	gametext.SetMsgText(""); // 진입 시 이전 메시지 삭제

	// 보유 포션 목록 수집
	PotionEntry potions[10];
	int potionCount = 0;

	ItemNode* curr = invenListHead;
	while (curr != nullptr)
	{
		if (curr->info.type == POTION && curr->count > 0)
		{
			potions[potionCount++] = { curr->info.name, curr->data.addHp, curr->count };
		}
		curr = curr->next;
	}

	// 텍스트 준비
	gametext.SetInvenTitle("Use a Potion?");
	gametext.SetGuide("0 - Enter stage start");
	gametext.SetNopotion("No potions in inventory.");

	string message = "";
	bool potionUsed = false; // 이미 이번 진입에서 포션 사용했는지

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				return -1;
			}
			if (event.type == Event::KeyPressed)
			{
				// 0키: 포션 없이 입장
				if (event.key.code == Keyboard::Num0)
					return 0;

				// 숫자 1~potionCount: 포션 선택
				int choice = -1;
				if (event.key.code == Keyboard::Num1) choice = 0;
				else if (event.key.code == Keyboard::Num2) choice = 1;
				else if (event.key.code == Keyboard::Num3) choice = 2;
				else if (event.key.code == Keyboard::Num4) choice = 3;
				else if (event.key.code == Keyboard::Num5) choice = 4;

				if (choice >= 0)
				{
					if (choice < potionCount)
					{
						ConsumeItem(potions[choice].name);
						return potions[choice].addHp;
					}
					else if (choice >= 0)
					{
						message = "No such potion!";
						gametext.SetMsgText(message);
					}
				}
			}
		}

		window.clear(Color(30, 30, 50));

		gametext.RenderPotionSelect(window, (potionCount == 0));

		for (int i = 0; i < potionCount; i++)
		{
			string str = to_string(i + 1) + " - " + potions[i].name +
				" (+" + to_string(potions[i].addHp) + " HP) x" + to_string(potions[i].count);
			gametext.Setpt(window, str, 220.f + i * 60.f);
		}

		window.display();
	}
	return -1;
}