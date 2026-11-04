#include "Main.h"
Font GameText::font;

Main::Main()
	:window(VideoMode(1500,600), "Cookie Run"), inventory(window)
{
	window.setFramerateLimit(60); //초당 프레임 고정 
}
void Main::Intro()
{
	GameText gametext;
	introTexture.loadFromFile("UI/Intro.png");
	introbg.setTexture(introTexture);
	float scaleX = 1500.f / introTexture.getSize().x;
	float scaleY = 600.f / introTexture.getSize().y;
	introbg.setScale(scaleX, scaleY);

	//텍스트는 루프 시작 전에 미리 세팅
	gametext.SetTitle("Cookie Run...?");
	gametext.SetIntro("Press to AnyKey GAME START!!!");

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			//close를 누르면 루프 종료
			if (event.type == Event::Closed)
			{
				window.close();
			}
			// 아무 키나 누르면 인트로 종료 → Start()로 복귀 → Field 시작
			else if (event.type == Event::KeyPressed)
			{
				return;
			}
		}
		window.clear();
		window.draw(introbg); //배경
		gametext.Render(window); //텍스트
		window.display(); //화면 출력
	}

}

void Main::Start()
{
	Player player(window);
	Intro(); //인트로 먼저

	bool stage1Cleared = false; //클리어 여부


	while (window.isOpen())
	{
		Menu menu(window);
		MenuResult result = menu.Select();

		if (result == MenuResult::Stage)
		{
			//스테이지 선택 화면
			StageSelect stageSelect(window, stage1Cleared, inventory);
			StageResult sr = stageSelect.Select(player);

			if (sr == StageResult::Stage1Clear)
			{
				Stage1 stage(window);
				bool clear = stage.Run(player);
				//클리어시 보스 스테이지 잠금 해제
				if (clear)
				{
					stage1Cleared = true;
				}
				player.ResetHp();
			}
			else if (sr == StageResult::GoBossStage)
			{
				BossStage boss(window);
				boss.Run(player);
				player.ResetHp();
			}
			// BackToMenu면 그냥 메뉴로 복귀
		}
		else if (result == MenuResult::itemShop)
		{
			Shop shop(window, inventory);
			shop.ItemSelect(player);
		}
		else if (result == MenuResult::Inven)
		{
			inventory.invenSelect(player);
		}
		else if (result == MenuResult::Save)
		{
			FileSave::SaveGame(player, stage1Cleared, inventory);
		}
		else if (result == MenuResult::Load)
		{
			FileSave::LoadGame(player, stage1Cleared, inventory);
		}
		else if (result == MenuResult::Exit)
		{
			window.close();
			break;
		}
	}
}