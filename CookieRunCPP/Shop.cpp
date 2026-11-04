#include "Shop.h"

Shop::Shop(RenderWindow& win, Inventory& inven)
    : window(win), inventory(inven)
{
    // 배경 설정
    bgTexture.loadFromFile("UI/Menu.png");
    bg.setTexture(bgTexture);
    float scaleX = 1500.f / bgTexture.getSize().x;
    float scaleY = 600.f / bgTexture.getSize().y;
    bg.setScale(scaleX, scaleY);

    shoptitleTexture.loadFromFile("UI/titles1.png");

    //아이템 애니메이션
    for (int i = 0; i < 4; i++)
    {
        rareTexture[i].loadFromFile("Items/hp_potion_" + to_string(i) + ".png");
        uniTexture[i].loadFromFile("Items/Elixir_" + to_string(i) + ".png");
    }

    rareSprite.setTexture(rareTexture[0]);
    uniSprite.setTexture(uniTexture[0]);

    // 연결 리스트 초기화
    InitShopItems();
}

Shop::~Shop() 
{
    // 메모리 누수 방지를 위한 연결 리스트 해제
    ItemNode* curr = shopListHead;
    while (curr != nullptr)
    {
        ItemNode* next = curr->next;
        delete curr;
        curr = next;
    }
}

void Shop::InitShopItems() 
{
    // 1. 일반 포션 (현재 체력 +2)
    AddShopItem({ POTION, "Rare Potion", 100 }, { 2 });

    // 2. 유니크 포션 (엘릭서 이미지, 현재 체력 +4)
    AddShopItem({ POTION, "Unique Potion (Elixir)", 250 }, { 4 });

}

void Shop::AddShopItem(ItemINFO info, Item data)
{
    ItemNode* newNode = new ItemNode(info, data);
    if (shopListHead == nullptr) 
    {
        shopListHead = newNode;
    }
    else 
    {
        ItemNode* temp = shopListHead;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = newNode;
    }
}

void Shop::ItemSelect(Player& player) 
{
    string message = "";
    float msgTimer = 0.f;

    while (window.isOpen()) 
    {
        float dt = aniClock.restart().asSeconds();
        aniTimer += dt;

        if (msgTimer > 0.f) msgTimer -= dt;
        else message = "";

        if (aniTimer >= 0.15f)
        {
            aniTimer = 0.f;
            aniFrame = (aniFrame + 1) % 4;
            rareSprite.setTexture(rareTexture[aniFrame]);
            uniSprite.setTexture(uniTexture[aniFrame]);
        }

        Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed) 
            {
                if (event.key.code == Keyboard::Escape) return; // 상점 나가기

                // 키 입력(1, 2, 3)에 따른 아이템 선택 로직
                int choice = -1;
                if (event.key.code == Keyboard::Num1) choice = 0;
                else if (event.key.code == Keyboard::Num2) choice = 1;

                if (choice != -1) 
                {
                    ItemNode* curr = shopListHead;
                    for (int i = 0; i < choice && curr != nullptr; i++) 
                    {
                        curr = curr->next;
                    }

                    if (curr != nullptr) 
                    {
                        if (player.GetGold() >= curr->info.price) 
                        {
                            // 골드 차감
                            player.SetGold(player.GetGold() - curr->info.price);
                            // 인벤토리에 추가
                            inventory.AddItem(curr->info, curr->data);
                            message = "Purchase: " + curr->info.name + "!";
                            msgTimer = 2.f;
                        }
                        else 
                        {
                            message = "Not enough Gold!";
                            msgTimer = 2.f;
                        }
                    }
                }
            }
        }
        window.clear();
        window.draw(bg);

        //상점 제목 배경
        titleSprite[0].setTexture(shoptitleTexture);
        // 크기 설정: 가로 500, 세로 100 정도
        titleSprite[0].setScale(500.f / shoptitleTexture.getSize().x, 100.f / shoptitleTexture.getSize().y);

        // 위치 설정: "ITEM SHOP" 글자 뒤로
        titleSprite[0].setPosition(480.f, 40.f);
        window.draw(titleSprite[0]);

        // 상단 타이틀 및 보유 골드 표시
        gametext.SetTitle("ITEM SHOP");
        gametext.SetScore("Your Gold: " + to_string(player.GetGold()));  

        // 연결 리스트를 순회하며 판매 품목 출력
        ItemNode* curr = shopListHead;
        int i = 0;
        while (curr != nullptr)
        {
            //기준 좌표
            float posX = 750.f;
            float posY = 300.f + i * 110.f;

            // [A] 아이템별 배경 UI (titleSprite[1], [2]... 사용)
            titleSprite[i + 1].setTexture(shoptitleTexture);
            titleSprite[i + 1].setScale(650.f / shoptitleTexture.getSize().x, 100.f / shoptitleTexture.getSize().y);
            titleSprite[i + 1].setPosition(posX - 325.f, posY - 45.f);
            window.draw(titleSprite[i + 1]);

            //아이템 텍스트 설정
            string itemStr = curr->info.name + " - " + to_string(curr->info.price) + "G";
            gametext.SetMenuButton(i, itemStr, posX, posY);

            //아이템 이름에 맞춰 애니메이션 스프라이트 위치 잡기
            if (curr->info.name == "Rare Potion")
            {
                rareSprite.setPosition(posX - 260.f, posY - 40.f); 
                rareSprite.setScale(0.8f, 0.8f);
                window.draw(rareSprite);
            }
            else if (curr->info.name == "Unique Potion (Elixir)")
            {
                uniSprite.setPosition(posX - 260.f, posY - 30.f);
                uniSprite.setScale(0.45f, 0.45f);
                window.draw(uniSprite);
            }

            curr = curr->next;
            i++;
        }

        gametext.RenderMenu(window); // 버튼들 그리기
        gametext.Render(window);     // SetScore로 설정한 골드/타이틀 텍스트 그리기

        // 구매/오류 메시지
        if (!message.empty())
        {
            Text msg;
            msg.setFont(GameText::GetFont());
            msg.setString(message);
            msg.setCharacterSize(24);
            msg.setFillColor(Color::Cyan);
            FloatRect mb = msg.getLocalBounds();
            msg.setOrigin(mb.width / 2.f, mb.height / 2.f);
            msg.setPosition(750.f, 540.f);
            window.draw(msg);
        }

        window.display();
    }
}