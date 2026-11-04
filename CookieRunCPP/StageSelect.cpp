#include "StageSelect.h"

StageSelect::StageSelect(RenderWindow& win, bool cleared, Inventory& inven)
	:window(win), stage1Cleared(cleared), inventory(inven)
{
	// 배경 설정
	SbgTexture.loadFromFile("UI/StageSelect_BG.png");
	stagebg.setTexture(SbgTexture);

	float scaleX = 1500.f / SbgTexture.getSize().x;
	float scaleY = 600.f / SbgTexture.getSize().y;
	stagebg.setScale(scaleX, scaleY);
    // 클리어 여부에 따라 잠금 표시
    if (stage1Cleared)
        gametext.SetStageText(1, "2 - BOSS STAGE", Color::White);
    else
        gametext.SetStageText(1, "2 - BOSS STAGE [LOCKED]", Color(100, 100, 100));
}

StageResult StageSelect::Select(Player& player)
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
					// 포션 선택 화면 → 반환값으로 hp 증가
					int addHp = inventory.PotionSelect();
					if (addHp > 0)
					{
						player.SetHp(player.GetHp() + addHp);
					}
					// addHp == 0: 포션 안 먹고 입장
					// addHp == -1: 창 닫힘 (입장 취소)
					if (addHp == -1)
					{
						return BackToMenu;
					}
					return Stage1Clear;
				}

				// 클리어해야만 보스 스테이지 진입 가능
				if (event.key.code == Keyboard::Num2 && stage1Cleared)
				{
					int addHp = inventory.PotionSelect();
					if (addHp > 0)
					{
						player.SetHp(player.GetHp() + addHp);
					}
					if (addHp == -1)
					{
						return BackToMenu;
					}
					return GoBossStage;
				}

				// ESC 누르면 메뉴로 복귀
				if (event.key.code == Keyboard::Escape)
				{
					return BackToMenu;
				}
			}
		}

        window.clear();
		window.draw(stagebg);
		gametext.RenderStageSelect(window);
        window.display();
    }
    return BackToMenu;
}