#include "BossStage.h"
#include <ctime>

BossStage::BossStage(RenderWindow& win)
	:Field(win), boss(win)
{
	timeLeft = 60.f; //보스스테이지 제한 시간

	// 배경을 BossStage.png로 교체
	bgTexture.loadFromFile("Boss/BossStage.png");
	for (int i = 0; i < 2; i++)
	{
		bg[i].setTexture(bgTexture);
		float scaleX = 2300.f / bgTexture.getSize().x;
		float scaleY = 1000.f / bgTexture.getSize().y;
		bg[i].setScale(scaleX, scaleY);
	}
	bg[0].setPosition(0, 0);
	bg[1].setPosition(1500.f, 0);
}

void BossStage::ShowBossRoom()
{
	Texture roomTexture;
	roomTexture.loadFromFile("UI/BossRoom.png");
	Sprite roomSprite;
	roomSprite.setTexture(roomTexture);
	float scaleX = 1500.f / roomTexture.getSize().x;
	float scaleY = 600.f / roomTexture.getSize().y;
	roomSprite.setScale(scaleX, scaleY);

	Clock roomClock;

	while (window.isOpen())
	{
		float elapsed = roomClock.getElapsedTime().asSeconds();
		if (elapsed >= 3.f) return;  // 5초 후 스테이지 진입

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}

		window.clear();
		window.draw(roomSprite);
		window.display();
	}
}

bool BossStage::Run(Player& player)
{
	ShowBossRoom(); //보스방 입장

	srand((unsigned int)time(NULL));

	GameText gametext;

	// 2. 보스 스폰 애니메이션 시작
	boss.StartSpawn();

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		//스폰 완료 되면 게임 시작
		if (!gameStarted && boss.IsSpawnDone())
		{
			gameStarted = true;

			//타이머 스폰 이후 시작
			clock.restart();
		}

		// 제한시간 업데이트 - 게임 시작 후에만 감소
		if (gameStarted && !stageClear && !gameover)
		{
			timeLeft -= dt;
			if (timeLeft <= 0.f)
			{
				timeLeft = 0.f;
				stageClear = true;
				boss.StartDead();		// 클리어시 보스 사망 애니메이션
				player.ClearReward();
			}
		}

		// 게임오버 체크
		if (player.IsDead()) gameover = true;

		// 빔 시간 초과 → 즉사
		if (boss.IsSkillWindowExpired())
		{
			player.InstantDie();
			gameover = true;
			boss.ResetBeam();
		}

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (!gameover && !stageClear)
			{
				player.HandleEvent(event);

				// Q키 → 보스 스킬 무력화 시도
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q)
				{
					// 쿨타임 체크 및 사용 처리 (보스 상태와 상관없이 실행)
					if (player.UseSkill())
					{
						//만약 보스가 빔을 쏘고 있었다면 무력화 성공
						if (boss.IsBeamActive())
						{
							boss.SetSkillNeutralized(true);
						}
					}
					
				}
			}
			// 클리어: 보스 사망 애니메이션 완료 후 아무키
			// 게임오버: 바로 아무키
			if ((stageClear || gameover) && event.type == Event::KeyPressed)
			{
				if (gameover) return false;
				if (stageClear && boss.IsDeadDone()) return true;
			}	
		}

		if (gameStarted && !gameover)
		{
			if (!stageClear)
			{
				player.Update(dt);
				ScrollBG(dt);
				ScrollTile(dt);
				UpdateGold(dt);
				Collision(player, dt);
			}
			boss.Update(dt, stageClear ? 0.f : timeLeft);

			// 투사체 충돌
			FloatRect playerBounds = player.GetBounds();
			if(!player.IsInvincible())
			{
				for (int i = 0; i < 5; i++)
				{
					if (!boss.IsSkillActive(i)) continue;
					if (playerBounds.intersects(boss.GetSkillBounds(i)))
					{
						boss.SetSkillActive(i, false);	// 투사체 사라짐
						player.Hit();
						break;
					}
				}
			}
		}
		else if (!gameStarted)
		{
			// 스폰 중에는 애니메이션만 업데이트
			boss.Update(dt, timeLeft);
		}

		// HUD 텍스트
		if (gameStarted)
		{
			gametext.SetScore(
				"Time: " + to_string((int)timeLeft) +
				"  Gold: " + to_string(player.GetGold()) +
				"  HP: " + to_string(player.GetHp()) +
				"  Skill: " + (player.IsSkillReady() ? "READY" : to_string((int)player.GetSkillCool()) + "s")
			);
		}
		

		window.clear();
		window.draw(bg[0]);
		window.draw(bg[1]);
		for (int i = 0; i < TILE_COUNT; i++)
		{
			window.draw(tile[i]);
		}
		if (gameStarted)
		{
			for (int i = 0; i < GOLD_COUNT; i++)
			{
				gold[i].Render(window);
			}
		}
		
		boss.Render();
		player.Render();
		gametext.Render(window);

		// 클리어 화면
		if (stageClear&& boss.IsDeadDone())
		{
			gametext.SetTitle("STAGE CLEAR!");
			gametext.SetIntro(
				"Gold: " + to_string(player.GetGold()) +
				"  Press AnyKey..."
			);
			gametext.Render(window);
		}

		// 게임오버 화면
		if (gameover)
		{
			gametext.SetTitle("GAME OVER!");
			gametext.SetIntro("Press AnyKey...");
			gametext.Render(window);
		}

		window.display();
	}
	return false;
}
