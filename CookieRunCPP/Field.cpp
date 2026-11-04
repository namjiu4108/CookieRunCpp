#include "Field.h"
#include <ctime>
#include <algorithm>
Field::Field(RenderWindow& win)
	:window(win)
{
	bgTexture.loadFromFile("UI/Stage1.png");

	for (int i = 0; i < 2; i++)
	{
		bg[i].setTexture(bgTexture);
		//배경을 창 크기에 맞게 조정
		float scaleX = 1500.f / bgTexture.getSize().x;
		float scaleY = 600.f / bgTexture.getSize().y;
		bg[i].setScale(scaleX, scaleY);
	}

	bgWidth = 1500.f; //창 너비와 동일

	//bg[0]은 처음부터, bg[1]은 바로 오른쪽에 붙여서 출력
	bg[0].setPosition(0, 0);
	bg[1].setPosition(bgWidth, 0);
	
	//바닥 타일 세팅
	tileTexture[0].loadFromFile("tiles/tile_07.png");
	tileTexture[1].loadFromFile("tiles/tile_08.png");
	tileTexture[2].loadFromFile("tiles/tile_09.png");
	tileTexture[3].loadFromFile("tiles/tile_10.png");

	float scale = 3.f;
	tileW = tileTexture[0].getSize().x * scale;

	for (int i = 0; i < TILE_COUNT; i++)
	{
		tile[i].setTexture(tileTexture[i % 4]);
		tile[i].setScale(scale, scale);
		tile[i].setPosition(tileW * i, 410);
	}	


}
void Field::ScrollBG(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		bg[i].move(-bgSpeed * dt, 0); //왼쪽으로 이동

		//배경이 완전히 화면 밖으로 나간다면 다른 배경 뒤로 이동
		if (bg[i].getPosition().x <= -bgWidth)
		{
			//다른 배경(i == 0 이면 1, i == 1이면 0)의 오른쪽에 붙임
			int other = (i + 1) % 2;
			bg[i].setPosition(bg[other].getPosition().x + bgWidth, 0);
		}
	}
}
void Field::ScrollTile(float dt)
{
	for (int i = 0; i < TILE_COUNT; i++)
	{
		tile[i].move(-bgSpeed * dt, 0); //배경과 동일한 속도로 이동

		if (tile[i].getPosition().x <= -tileW)
		{
			float maxX = 0;
			for (int j = 0; j < TILE_COUNT; j++)
			{
				maxX = max(maxX, tile[j].getPosition().x);
			}
			tile[i].setPosition(maxX + tileW, 410);
		}
	}
}

void Field::UpdateGold(float dt)
{
	if (!stageClear && !gameover)
	{
		//스폰 타이머
		GspawnTimer += dt;
		if (GspawnTimer >= GspawnInterval)
		{
			GspawnTimer = 0.f;
			SpawnGold();
		}
	}
	for (int i = 0; i < GOLD_COUNT; i++)
	{
		gold[i].Move(dt);
		gold[i].GoldAni(dt);
	}
}

void Field::SpawnGold()
{
	//스폰 위치 y값 - 바닥, 중간, 위
	float Ypositions[] = { 310.f, 200.f, 100.f };

	for (int i = 0; i < GOLD_COUNT; i++)
	{
		if (!gold[i].IsActive()) //비활성화된 몬스터 재사용
		{
			//y위치 랜덤 선택
			float randomY = Ypositions[rand() % 3];

			//바닥 위치(410)에 스폰, x는 화면 오른쪽 밖에서 시작
			gold[i].GoldSpawn(1600.f, randomY);
			break;
		}
	}
}

void Field::Collision(Player& player, float dt)
{
	FloatRect playerBounds = player.GetBounds();  //충돌 감지	

	for (int i = 0; i < GOLD_COUNT; i++)
	{
		if (!gold[i].IsActive()) continue;

		if (playerBounds.intersects(gold[i].GetBounds()))
		{
			gold[i].SetActive(false);  // 골드 먹힘
			player.AddGold();         // 플레이어 골드 추가
		}
	}
}