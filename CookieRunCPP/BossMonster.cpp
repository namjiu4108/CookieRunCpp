#include "BossMonster.h"

BossMonster::BossMonster(RenderWindow& win) 
	: window(win)
{
	// Spawn 텍스처
	for (int i = 0; i < 6; i++)
	{
		BossSpawn[i].loadFromFile("Boss/BossSpawn" + to_string(i + 1) + ".png");
	}
	// Idle 텍스처
	for (int i = 0; i < 9; i++)
	{
		BossIdle[i].loadFromFile("Boss/Bossidle" + to_string(i + 1) + ".png");
	}
	// Transform 텍스처
	for (int i = 0; i < 13; i++)
	{
		BossTransform[i].loadFromFile("Boss/BossTransform" + to_string(i + 1) + ".png");
	}

	// TransformCancle 텍스처
	for (int i = 0; i < 3; i++)
	{
		BossTransformCancle[i].loadFromFile("Boss/BossTransformCancle" + to_string(i + 1) + ".png");
	}

	// UseSkill 텍스처
	for (int i = 0; i < 4; i++)
	{
		BossUseSkill[i].loadFromFile("Boss/BossUseSkill" + to_string(i + 1) + ".png");
	}

	// Dead 텍스처
	for (int i = 0; i < 5; i++)
	{
		BossDead[i].loadFromFile("Boss/BossDead" + to_string(i + 1) + ".png");
	}

	// 기본공격 투사체
	for (int i = 0; i < 4; i++)
	{
		BossSkill[i].loadFromFile("BossBasic/BossSkill" + to_string(i + 1) + ".png");
	}

	// 레이저 빔
	BossBeamStart[0].loadFromFile("BossBasic/BossSkill_start_01.png");
	BossBeamStart[1].loadFromFile("BossBasic/BossSkill_start_02.png");
	BossBeamStart[2].loadFromFile("BossBasic/BossSkill_start_03.png");

	for (int i = 0; i < 4; i++)
	{
		BossBeam[i].loadFromFile("BossBasic/BossSkill_0" + to_string(i + 1) + ".png");
	}
	BossRing.loadFromFile("BossBasic/BossSkill_start_04.png");

	// 스프라이트 초기 설정
	sprite.setTexture(BossSpawn[0]);
	sprite.setScale(0.8f, 0.8f);
	sprite.setPosition(1100.f, 150.f);	// 오른쪽에 보스 배치

	// 투사체 초기 설정
	for (int i = 0; i < 5; i++)
	{
		bskillSprite[i].setTexture(BossSkill[0]);
		bskillSprite[i].setScale(0.5f, 0.5f);
		bskillActive[i] = false;
	}

	// 빔/링 초기 설정
	beamSprite.setTexture(BossBeam[0]);
	ringSprite.setTexture(BossRing);
	ringSprite.setPosition(1050.f, 250.f);

	state = BOSSIDLE;
	isSpawning = false;
	spawnDone = false;
}

void BossMonster::StartSpawn()
{
	isSpawning = true;
	spawnDone = false;
	spawnFrame = 0;
	spawnTimer = 0.f;
	sprite.setTexture(BossSpawn[0]);
}

void BossMonster::StartDead()
{
	isDying = true;
	deadDone = false;
	deadFrame = 0;
	deadTimer = 0.f;
	sprite.setTexture(BossDead[0]);
}

void BossMonster::SetState(BossState newState)
{
	if (state == newState) return;
	state = newState;
	aniFrame = 0;
	aniTimer = 0.f;

	switch (state)
	{
	case BOSSIDLE:
		sprite.setTexture(BossIdle[0]);
		isTransforming = false;
		break;
	case TRANSFORM:
		sprite.setTexture(BossTransform[0]);
		isTransforming = true;
		break;
	case SKILL:
		sprite.setTexture(BossUseSkill[0]);
		beamActive = true;
		bskillWindowTimer = bskillWindowTime;
		bskillNeutralized = false;
		break;
	}
}

void BossMonster::UpdateAnimation(float dt)
{
	//스폰 애니메이션 처리
	if (isSpawning)
	{
		spawnTimer += dt;
		if (spawnTimer >= spawnSpeed)
		{
			spawnTimer = 0.f;
			spawnFrame++;
			if (spawnFrame >= 6)
			{
				// 스폰 완료 → Idle로
				isSpawning = false;
				spawnDone = true;
				SetState(BOSSIDLE);
			}
			else
			{
				sprite.setTexture(BossSpawn[spawnFrame]);
			}
		}
		return;
	}
	// 사망 애니메이션 처리
	if (isDying)
	{
		deadTimer += dt;
		if (deadTimer >= deadSpeed)
		{
			deadTimer = 0.f;
			deadFrame++;
			if (deadFrame >= 5)
			{
				isDying = false;
				deadDone = true;
			}
			else
			{
				sprite.setTexture(BossDead[deadFrame]);
			}
		}
		return;
	}

	aniTimer += dt;
	if (aniTimer >= aniSpeed)
	{
		aniTimer = 0.f;

		switch (state)
		{
		case BOSSIDLE:
			aniFrame = (aniFrame + 1) % 9;
			sprite.setTexture(BossIdle[aniFrame]);
			break;

		case TRANSFORM:
			if (aniFrame < 12)
			{
				aniFrame++;
				sprite.setTexture(BossTransform[aniFrame]);
			}
			else
			{
				// 변신 완료 → 스킬 상태로
				SetState(SKILL);
			}
			break;

		case SKILL:
			aniFrame = (aniFrame + 1) % 4;
			sprite.setTexture(BossUseSkill[aniFrame]);
			break;
		}
	}
}

//보스 기본 공격
void BossMonster::BasicAttack()
{
	float Ypositions[] = { 150.f, 250.f, 350.f };
	int spawnCnt = 0;

	for (int i = 0; i < 5; i++)
	{
		if (!bskillActive[i])
		{
			float randomY = Ypositions[rand() % 3];
 			bskillSprite[i].setPosition(1050.f + spawnCnt * 80.f, randomY);
			bskillActive[i] = true;
			spawnCnt++;
			if(spawnCnt >= 3) break;
		}
	}
}

void BossMonster::UpdateSkillProjectile(float dt)
{
	//스폰 완료 후 + IDLE 상태에서만 기본 공격
	if (!spawnDone || state != BOSSIDLE) return;

	// 기본 공격 타이머
	battackTimer += dt;
	if (battackTimer >= battackInterval)
	{
		battackTimer = 0.f;
		BasicAttack();
	}

	// 투사체 이동
	for (int i = 0; i < 5; i++)
	{
		if (!bskillActive[i]) continue;

		bskillSprite[i].move(-400.f * dt, 0);

		if (bskillSprite[i].getPosition().x <= -100.f)
		{
			bskillActive[i] = false;
		}
	}

	// 투사체 애니메이션
	bskillAniTimer += dt;
	if (bskillAniTimer >= 0.1f)
	{
		bskillAniTimer = 0.f;
		bskillFrame = (bskillFrame + 1) % 4;
		for (int i = 0; i < 5; i++)
		{
			if (bskillActive[i])
			{
				bskillSprite[i].setTexture(BossSkill[bskillFrame]);
			}
		}
	}
}

void BossMonster::UpdateBeam(float dt)
{
	if (!beamActive) return;

	// 스킬 무력화 가능 시간 감소
	bskillWindowTimer -= dt;

	// 빔 애니메이션
	beamAniTimer += dt;
	if (beamAniTimer >= 0.1f)
	{
		beamAniTimer = 0.f;
		beamFrame = (beamFrame + 1) % 4;
		beamSprite.setTexture(BossBeam[beamFrame]);
	}

	// 레이저 위치 (화면 전체를 가로지름)
	beamSprite.setPosition(0.f, 230.f);
	beamSprite.setScale(4.f, 1.5f);

	// 무력화 성공
	if (bskillNeutralized)
	{
		CancleTransform();
		return;
	}
}

void BossMonster::Transform()
{
	if (transformCount >= 2) return;
	transformCount++;
	bskillNeutralized = false;
	SetState(TRANSFORM);
}

void BossMonster::CancleTransform()
{
	// 변신 취소 애니메이션 재생
	aniFrame = 0;
	sprite.setTexture(BossTransformCancle[0]);

	beamActive = false;
	isTransforming = false;
	bskillNeutralized = false;
	SetState(BOSSIDLE);
}

void BossMonster::UseSkill()
{
	SetState(SKILL);
}

void BossMonster::Update(float dt, float timeLeft)
{
	if (deadDone) return; // 이미 죽었다면 업데이트 중단 (막 프레임 이미지 고정)
	//스폰 완료 후 변신 타이밍 체크
	if (spawnDone && !isDying)
	{
		// 1차: 60초 남았을때, 2차: 30초 남았을때
		if (transformCount == 0 && timeLeft <= 30.f && state == BOSSIDLE)
		{
			Transform();
		}
		else if (transformCount == 1 && timeLeft <= 15.f && state == BOSSIDLE)
		{
			Transform();
		}
	}
	

	UpdateAnimation(dt);
	UpdateSkillProjectile(dt);
	UpdateBeam(dt);
}

void BossMonster::Render()
{
	window.draw(sprite);

	// 스폰/사망 중이면 투사체/빔 렌더 안함
	if (!spawnDone || isDying) return;

	// 투사체 렌더
	for (int i = 0; i < 5; i++)
	{
		if (bskillActive[i])
		{
			window.draw(bskillSprite[i]);
		}
	}

	// 링 이펙트 (빔 차징중)
	if (beamActive && bskillWindowTimer > 0.f)
	{
		window.draw(ringSprite);
	}

	// 레이저 빔 렌더
	if (beamActive)
	{
		window.draw(beamSprite);

	}
}