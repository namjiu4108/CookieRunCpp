#pragma once
#include "Defiens.h"
#include "Struct.h"
class Gold
{
private:
	//골드
	Texture goldtexture[6]; //애니메이션 6장
	Sprite goldsprite;
	bool isActive = false; // 활성화 여부
	float goldSpeed = 200.f; //배경과 동일한 속도

	//골드 애니메이션 
	int goldFrame = 0; //현재 골드 프레임
	float goldAniTimer = 0.f; //골드 타이머
	const float goldAniSpeed = 0.1f; //골드 애니메이션의 속도

	//히트박스
	float hoffsetX = 5.f; //이미지에서 히트박스 x 오프셋
	float hoffsetY = 5.f; //이미지에서 히트박스 y 오프셋
	float hwidth = 40.f; //히트박스 너비
	float hheight = 40.f; //히트박스 높이

public:
	Gold();

	void GoldSpawn(float x, float y);
	void Move(float dt);
	void GoldAni(float dt);
	void Render(RenderWindow& window);

	bool IsActive() { return isActive; }
	void SetActive(bool b) { isActive = b; }


	FloatRect GetBounds() // 충돌 감지용
	{
		//스프라이트 위치 기준으로 히트박스 계산
		Vector2f pos = goldsprite.getPosition();
		return FloatRect(pos.x + hoffsetX, pos.y + hoffsetY, hwidth, hheight);
	}
};

