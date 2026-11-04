#include "Gold.h"

Gold::Gold()
{
	//골드 세팅
	goldtexture[0].loadFromFile("UI/Gold_1.png");
	goldtexture[1].loadFromFile("UI/Gold_2.png");
	goldtexture[2].loadFromFile("UI/Gold_3.png");
	goldtexture[3].loadFromFile("UI/Gold_4.png");
	goldtexture[4].loadFromFile("UI/Gold_5.png");
	goldtexture[5].loadFromFile("UI/Gold_6.png");

	goldsprite.setTexture(goldtexture[0]);
	goldsprite.setScale(1.0f, 1.0f); //크기 조정
	isActive = false;
}

void Gold::GoldSpawn(float x, float y)
{
	goldsprite.setPosition(x, y);
	isActive = true;
	goldFrame = 0;
	goldAniTimer = 0.f;
}

void Gold::Move(float dt)
{
	if (!isActive) return;
	goldsprite.move(-goldSpeed * dt, 0);

	//화면 밖으로 나가면 비활성화
	if (goldsprite.getPosition().x <= -100.f)
	{
		isActive = false;
	}
}

void Gold::GoldAni(float dt)
{
	if (!isActive) return;
	goldAniTimer += dt;
	if (goldAniTimer >= goldAniSpeed)
	{
		goldAniTimer = 0.f;
		goldFrame = (goldFrame + 1) % 6; // 6장 순환

		goldsprite.setTexture(goldtexture[goldFrame]); //모든 골드 동시에 프레임함

	}
}

void Gold::Render(RenderWindow& window)
{
	if (!isActive) return;
	window.draw(goldsprite);
}
