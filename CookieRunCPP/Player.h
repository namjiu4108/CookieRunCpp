#pragma once
#include "Defiens.h"
#include "Struct.h"
class Player
{
private:
	int Gold;
	int Hp;
	const int BASE_HP = 5; // 기본 체력 고정값

	pinfo info;
	Sprite sprite;
	RenderWindow& window;

	//텍스쳐
	Texture Idle;
	Texture Run[3];
	Texture Jump;
	Texture Doublejump[4];
	Texture Slide[2];
	Texture Die;
	Texture pSkill[8];

	//애니메이션
	PlayerState state = RUN;
	int runFrame = 0;
	float aniTimer = 0.f;
	const float aniSpeed = 0.15f; //숫자가 낮을수록 빠름

	Clock clock;

	//물리변수(점프 로직용)
	float velocityY = 0; //수직 속도
	const float gravity = 0.8f; //중력 가속도
	const float jumpPower = -18.0f; //점프 힘
	bool isJumping = false; //점프 중인지 확인
	bool DJump = false; //더블 점프
	float groundLevel = 240.0f; //바닥 높이

	//히트 박스 전용
	FloatRect hitbox; //히트박스
	float hoffsetX = 85.f; //이미지에서 히트박스 x 오프셋
	float hoffsetY = 150.f; //이미지에서 히트박스 y 오프셋
	float hwidth = 50.f; //히트박스 너비
	float hheight = 60.f; //히트박스 높이

	// 스킬
	float skillCoolTimer = 0.f;
	const float skillCoolTime = 15.f;
	bool skillReady = true;

	//플레이어 무적
	float invincibleTimer = 0.f; //플레이어 무적 타이머
private:
	void SetState(PlayerState newState);
	void Animation(float dt);

public:
	Player(RenderWindow& win); //생성자에서 window 받기
	void HandleEvent(const Event& event);
	void Update(float dt);
	void Render();
	void Hit();
	void AddGold();
	void ClearReward();
	void InstantDie();
	bool UseSkill();
	bool IsInvincible() { return invincibleTimer > 0.f; } // 무적 여부 확인용

	int GetHp() { return Hp; }
	void SetHp(int php) { Hp = php; info.hp = Hp; }
	int GetGold() { return Gold; }
	void SetGold(int pgold) { Gold = pgold; info.Gold = Gold; }
	bool IsDead() { return info.hp <= 0; }
	bool IsSkillReady() { return skillReady; }
	float GetSkillCool() { return skillCoolTimer; }

	// 스테이지 종료 후 HP를 기본값(5)으로 리셋
	void ResetHp() { SetHp(BASE_HP); }

	FloatRect GetBounds(); // 충돌 감지용
	  
};

