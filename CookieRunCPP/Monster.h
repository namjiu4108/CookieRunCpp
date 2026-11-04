#pragma once
#include "Defiens.h"
class Monster
{
private:
	Sprite sprite;
	Texture Mon1texture;
	float speed = 300.f; //날아오는 속도
	bool isActive = false; // 활성화 여부

	//히트 박스 전용
	FloatRect hitbox; //히트박스
	float hoffsetX = 10.f; //이미지에서 히트박스 x 오프셋
	float hoffsetY = 10.f; //이미지에서 히트박스 y 오프셋
	float hwidth = 30.f; //히트박스 너비
	float hheight = 30.f; //히트박스 높이

public:
	Monster();
	void Spawn(float x, float y); //몬스터 생성위치
	void Move(float dt);
	void Render(RenderWindow& window);

	bool IsActive() { return isActive; }
	void setActive(bool b) { isActive = b; }

	FloatRect GetBounds() // 충돌 감지용
	{
		//스프라이트 위치 기준으로 히트박스 계산
		Vector2f pos = sprite.getPosition();
		return FloatRect(pos.x + hoffsetX, pos.y + hoffsetY, hwidth, hheight);
	}
};

