#pragma once
#include "Defiens.h"
#include "Struct.h"

class BossMonster
{
private:
	Sprite sprite;
	RenderWindow& window;

	//텍스쳐
	Texture BossSpawn[6];
	Texture BossIdle[9];
	Texture BossTransform[13];
	Texture BossTransformCancle[3];
	Texture BossUseSkill[4];
	Texture BossDead[5];

	// 기본공격 투사체
	Texture BossSkill[4];
	Sprite bskillSprite[5];
	bool bskillActive[5] = {};
	float bskillAniTimer = 0.f;
	int bskillFrame = 0;
	float battackTimer = 0.f;
	float battackInterval = 3.f;	// 기본공격 간격(초)

	// 레이저 빔
	Texture BossBeamStart[3];
	Texture BossBeam[4];
	Texture BossRing;
	Sprite beamSprite;
	Sprite ringSprite;
	bool beamActive = false;
	float beamAniTimer = 0.f;
	int beamFrame = 0;

	// 상태
	BossState state = BOSSIDLE;
	int aniFrame = 0;
	float aniTimer = 0.f;
	const float aniSpeed = 0.1f;

	//스폰 관련
	bool isSpawning = false; //스폰 중인지 확인
	bool spawnDone = false; //스폰 완료 여부
	int spawnFrame = 0;
	float spawnTimer = 0.f;
	const float spawnSpeed = 0.15f;

	//사망 관련
	bool isDying = false;
	bool deadDone = false; //스폰 완료 여부
	int deadFrame = 0;
	float deadTimer = 0.f;
	const float deadSpeed = 0.15f;

	// 변신 관련
	int transformCount = 0;			// 변신 횟수 (최대 2)
	bool isTransforming = false;	// 변신 중인지
	float bskillWindowTimer = 0.f;	// 플레이어가 스킬 쓸 수 있는 시간
	float bskillWindowTime = 5.f;	// 스킬 무력화 가능 시간(초)
	bool bskillNeutralized = false;	// 스킬 무력화 여부

	// 히트박스
	float hoffsetX = 50.f;
	float hoffsetY = 50.f;
	float hwidth = 100.f;
	float hheight = 100.f;

	void SetState(BossState newState);
	void UpdateAnimation(float dt);
	void UpdateSkillProjectile(float dt);
	void UpdateBeam(float dt);
	void BasicAttack();

public:
	BossMonster(RenderWindow& win);

	void Update(float dt, float timeLeft);
	void Render();

	void StartSpawn();		// 스폰 시작
	void StartDead();		// 사망 시작
	void Transform();		// 변신 시작
	void CancleTransform();	// 변신 취소
	void UseSkill();		// 레이저 발사

	bool IsSpawnDone() { return spawnDone; }	// 스폰 완료 여부
	bool IsDeadDone() { return deadDone; }		// 사망 완료 여부
	bool IsTransforming() { return isTransforming; }
	bool IsBeamActive() { return beamActive; }
	bool IsSkillNeutralized() { return bskillNeutralized; }
	bool IsSkillWindowExpired() { return bskillWindowTimer <= 0.f && beamActive && !bskillNeutralized; }
	bool IsBeamTimerExpired() { return bskillWindowTimer <= 0.f && beamActive && !bskillNeutralized; }
	
	void ResetBeam() { beamActive = false; }  // BossStage에서 호출
	void SetSkillNeutralized(bool b) { bskillNeutralized = b; }

	FloatRect GetBounds()
	{
		Vector2f pos = sprite.getPosition();
		return FloatRect(pos.x + hoffsetX, pos.y + hoffsetY, hwidth, hheight);
	}

	FloatRect GetSkillBounds(int i) { return bskillSprite[i].getGlobalBounds(); }
	bool IsSkillActive(int i) { return bskillActive[i]; }
	void SetSkillActive(int i, bool b) { bskillActive[i] = b; }  // 투사체 충돌시 제거용

	FloatRect GetBeamBounds() { return beamSprite.getGlobalBounds(); }
};