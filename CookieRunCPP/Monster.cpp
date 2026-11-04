#include "Monster.h"

Monster::Monster()
{
	Mon1texture.loadFromFile("Monster/DevilMonster.png");
	sprite.setTexture(Mon1texture);
	sprite.setScale(0.3f, 0.3f);
	isActive = false;
}

void Monster::Spawn(float x, float y)
{
	sprite.setPosition(x, y);
	isActive = true;
}

void Monster::Move(float dt)
{
	if (!isActive) return;
	sprite.move(-speed * dt, 0); //왼쪽으로 날아감

	//화면 밖으로 나가면 비활성화
	if (sprite.getPosition().x <= -100.f)
	{
		isActive = false;
	}
}

void Monster::Render(RenderWindow& window)
{
	if (!isActive) return;
	window.draw(sprite);
}