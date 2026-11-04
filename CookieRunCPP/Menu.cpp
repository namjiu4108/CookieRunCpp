#include "Menu.h"

Menu::Menu(RenderWindow& win)
	:window(win)
{
	bgTexture.loadFromFile("UI/Menu.png");
	bg.setTexture(bgTexture);
	//배경을 창 크기에 맞게 조정
	float scaleX = 1500.f / bgTexture.getSize().x;
	float scaleY = 600.f / bgTexture.getSize().y;
	bg.setScale(scaleX, scaleY);

	bg.setPosition(0, 0);
    // 버튼 이미지 로드
    btnTexture.loadFromFile("UI/titles1.png");

    // 버튼 라벨
    string labels[6] = { "1 - STAGE START", "2 - SHOP", "3 - INVENTORY" , "4 - SAVE", "5 - LOAD", "6 - EXIT" };

    // 좌우 배치를 위한 설정값
    float startX = 350.f;  // 첫 번째 열의 X 좌표
    float startY = 180.f;  // 시작 Y 좌표
    float gapX = 500.f;    // 버튼 사이의 가로 간격
    float gapY = 120.f;    // 버튼 사이의 세로 간격

    for (int i = 0; i < 6; i++)
    {
        // 버튼 스프라이트 세팅
        btnSprite[i].setTexture(btnTexture);

        // 1. 행과 열 계산
        int col = i / 3; // 0이면 왼쪽(1~3번), 1이면 오른쪽(4~6번)
        int row = i % 3; // 0, 1, 2 순서로 아래로 배치

        // 2. 버튼 위치 계산
        float btnX = startX + (col * gapX);
        float btnY = startY + (row * gapY);

        // 3. 스프라이트 설정
        float btnW = 300.f;
        float btnH = 60.f;
        btnSprite[i].setScale(btnW / btnTexture.getSize().x, btnH / btnTexture.getSize().y);
        btnSprite[i].setPosition(btnX, btnY);

        // 4. 텍스트 위치 (버튼 중앙)
        gametext.SetMenuButton(i, labels[i], btnX + 150.f, btnY + 20.f);
    }
    gametext.SetTitle("COOKIE RUN...?");
}

MenuResult Menu::Select()
{
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Num1)
                {
                    return Stage;
                }
                else if (event.key.code == Keyboard::Num2)
                {
                    return itemShop;
                }
                else if (event.key.code == Keyboard::Num3)
                {
                    return Inven;
                }
                else if (event.key.code == Keyboard::Num4)
                {
                    return Save;
                }
                else if (event.key.code == Keyboard::Num5)
                {
                    return Load;
                }
                else if (event.key.code == Keyboard::Num6)
                {
                    return Exit;
                }
            }
        }

        window.clear();
        window.draw(bg);
        for (int i = 0; i < 6; i++)
        {
            window.draw(btnSprite[i]);
        }
        gametext.RenderMenu(window);
        window.display();
    }
    return Exit;
}
