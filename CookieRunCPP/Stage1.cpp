#include "Stage1.h"
#include <ctime>

Stage1::Stage1(RenderWindow& win)
	:Field(win)
{

}
void Stage1::UpdateMonsters(float dt)
{
	if (!stageClear && !gameover)
	{
		//스폰 타이머
		spawnTimer += dt;
		if (spawnTimer >= spawnInterval)
		{
			spawnTimer = 0.f;
			SpawnMonster();
		}
	}

	//몬스터 이동
	for (int i = 0; i < MONSTER_COUNT; i++)
	{
		monsters[i].Move(dt);
	}
}

void Stage1::SpawnMonster()
{
	//몬스터 랜덤 스폰
	int spawnCount = rand() % 4;

	//스폰 위치 y값 - 바닥, 중간, 위
	float Ypositions[] = { 310.f, 200.f, 100.f };

	for (int i = 0; i < spawnCount; i++)
	{
		for (int j = 0; j < MONSTER_COUNT; j++)
		{
			if (!monsters[j].IsActive()) //비활성화된 몬스터 재사용
			{
				//y위치 랜덤 선택
				float randomY = Ypositions[rand() % 3];

				//바닥 위치(410)에 스폰, x는 화면 오른쪽 밖에서 시작
				monsters[j].Spawn(1600.f + i * 150.f, randomY);
				break;
			}
		}
	}
}
bool Stage1::Run(Player& player)
{
	srand((unsigned int)time(NULL));  // 랜덤 시드 초기화
	GameText gametext;
	//창 루프
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		// 제한시간 업데이트
		if (!stageClear && !gameover)
		{
			timeLeft -= dt;
			if (timeLeft <= 0.f)
			{
				timeLeft = 0.f;
				stageClear = true;  // 클리어!
				player.ClearReward();
			}
		}

		if (player.IsDead()) gameover = true;

		Event event;
		while (window.pollEvent(event))
		{
			//close를 누르면 루프 종료
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (!gameover && !stageClear)
			{
				player.HandleEvent(event);
			}
			//클리어 / 게임오버 후 아무키나 누르면 종료
			if ((stageClear || gameover) && event.type == Event::KeyPressed) return stageClear;
		}

		if (!gameover && !stageClear)
		{
			player.Update(dt);
			ScrollBG(dt); //배경 스크롤
			ScrollTile(dt); //타일 스크롤
			UpdateMonsters(dt); //몬스터 업데이트 추가
			UpdateGold(dt);
			Collision(player, dt);  // 충돌 체크

			FloatRect playerBounds = player.GetBounds();  //충돌 감지	
			if (!player.IsInvincible())
			{
				for (int i = 0; i < MONSTER_COUNT; i++)
				{
					if (!monsters[i].IsActive()) continue;

					// 플레이어랑 몬스터 충돌 확인
					if (playerBounds.intersects(monsters[i].GetBounds()))
					{
						monsters[i].setActive(false);  // 해당 몬스터만 제거
						player.Hit();                  // 플레이어 Hit
						break;
					}
				}
			}
		}

		// 남은 시간 텍스트 업데이트
		gametext.SetScore("Time: " + to_string((int)timeLeft) + "\t Gold : " + to_string(player.GetGold()) + "\t HP : " + to_string(player.GetHp()));

		window.clear();
		window.draw(bg[0]); //배경 1
		window.draw(bg[1]); //배경 2
		for (int i = 0; i < TILE_COUNT; i++)
		{
			window.draw(tile[i]);
		}
		for (int i = 0; i < MONSTER_COUNT; i++)
		{
			monsters[i].Render(window); //몬스터 랜더 
		}
		for (int i = 0; i < GOLD_COUNT; i++)
		{
			gold[i].Render(window); //골드 랜더
		}
		player.Render(); //플레이어는 맨위
		gametext.Render(window); //텍스트

		// 클리어 화면
		if (stageClear)
		{
			gametext.SetTitle("STAGE CLEAR!");
			gametext.SetIntro("ADD Gold : " + to_string(player.GetGold()) +  "\n" + "Press Any Key...");
			gametext.Render(window);
		}
		// 게임 오버
		if (gameover)
		{
			gametext.SetTitle("GAME OVER!");
			gametext.SetIntro("Press Any Key...");
			gametext.Render(window);
		}

		window.display(); //이것까지 해야 보임
	}
	return false;
}
