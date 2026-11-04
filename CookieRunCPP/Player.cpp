#include "Player.h"

Player::Player(RenderWindow& win)
    :window(win), Gold(0), Hp(5)
{
    //플레이어 기본 스탯
    info.hp = Hp;
    info.Gold = Gold;
    info.name = "Player";

    //쿠키 텍스쳐 및 스프라이트 설정
    Idle.loadFromFile("Cookie/Cookie.png");

    for (int i = 0; i < 3; i++)
    {
        Run[i].loadFromFile("Cookie/cookie_Run" + to_string(i + 1) + ".png");
    }
    for (int i = 0; i < 4; i++)
    {
        Doublejump[i].loadFromFile("Cookie/double_jump" + to_string(i + 1) + ".png");
    }
    for (int i = 0; i < 2; i++)
    {
        Slide[i].loadFromFile("Cookie/cookie_slide" + to_string(i + 1) + ".png");
    }
    for (int i = 0; i < 8; i++)
    {
        pSkill[i].loadFromFile("Cookie/cookie_skill" + to_string(i + 1) + ".png");
    }

    Jump.loadFromFile("Cookie/cookieJump.png");
    Die.loadFromFile("Cookie/cookie_Die.png");
    
    sprite.setTexture(Run[0]);
    sprite.setScale(0.6f, 0.6f);
    sprite.setPosition(100, groundLevel); //시작 위치

    state = RUN;
}
void Player::SetState(PlayerState newState)
{
    if (state == newState) return; //같은 상태면 무시
    state = newState;
    runFrame = 0;
    aniTimer = 0.f;

    switch (state)
    {
    case IDLE:
        sprite.setTexture(Idle);
        break;
    case RUN:
        sprite.setTexture(Run[0]);
        break;
    case JUMP:
        sprite.setTexture(Jump);
        break;
    case DOUBLE_JUMP:
        sprite.setTexture(Doublejump[0]);
        break;
    case SLIDE:
        sprite.setTexture(Slide[0]);
        break;
    case PSKILL:
        sprite.setTexture(pSkill[0]);
        break;
    case DIE:
        sprite.setTexture(Die);
        break;
    default:
        break;
    }
}

//플레이어 애니메이션 기능
void Player::Animation(float dt)
{
    aniTimer += dt;
    if (state == RUN)
    {
        if (aniTimer >= aniSpeed)
        {
            aniTimer = 0.f;
            runFrame = (runFrame + 1) % 3; // 0->1->2->0 순환
            sprite.setTexture(Run[runFrame]);
        }
    }
    else if (state == DOUBLE_JUMP)
    {
        if (aniTimer >= aniSpeed)
        {
            aniTimer = 0.f;
            runFrame = (runFrame + 1) % 4; // 4장 순환
            sprite.setTexture(Doublejump[runFrame]);
        }
    }
    else if (state == SLIDE)
    {
        if (aniTimer >= aniSpeed)
        {
            aniTimer = 0.f;
            runFrame = (runFrame + 1) % 2; // 2장순환
            sprite.setTexture(Slide[runFrame]);
        }
    }
    else if (state == PSKILL)
    {
        if (aniTimer >= aniSpeed)
        {
            aniTimer = 0.f;
            runFrame++;
            //runFrame = (runFrame + 1) % 8; // 8장 순환
            if (runFrame >= 8)
            {
                SetState(RUN);
            }
            else
            {
                sprite.setTexture(pSkill[runFrame]);
            }

        }
    }
}
void Player::HandleEvent(const Event& event) //키를 누른 순간/ 뗸 순간만 감지
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
    {
        if (!isJumping) //바닥에 있을때만 점프하도록
        {
            velocityY = jumpPower;
            isJumping = true;
            DJump = true;
            SetState(JUMP);
        }
        else if (isJumping && DJump)
        {
            velocityY = jumpPower;
            DJump = false;
            SetState(DOUBLE_JUMP);
        }
    }
}

void Player::Update(float dt)
{
    //무적 타이머 감소
    if (invincibleTimer > 0.f) invincibleTimer -= dt;

    // 스킬 쿨타임 감소
    if (!skillReady)
    {
        skillCoolTimer -= dt;
        if (skillCoolTimer <= 0.f)
        {
            skillCoolTimer = 0.f;
            skillReady = true;
        }
    }

    //키를 누르고 있어야 slide가 됨
    if (Keyboard::isKeyPressed(Keyboard::LControl))
    {
        if(state != SLIDE) SetState(SLIDE);
    }
    else
    {
        if (state == SLIDE) SetState(RUN);
    }

    //물리 업데이트
    velocityY += gravity; //매 프레임 중력 적용
    sprite.move(0, velocityY); //계산된 속도 만큼 쿠키이동

    if (sprite.getPosition().y >= groundLevel)
    {
        sprite.setPosition(100, groundLevel);
        velocityY = 0;
       
        if (isJumping)
        {
            isJumping = false;
            DJump = false; //착지하면 초기화
            SetState(RUN); //착지하면 다시 바로 달리기 실행
        }
    }
    Animation(dt);
}

void Player::Render()
{
    // 0. 최우선 순위: 죽었다면 깜빡임 무시하고 그냥 그리기
    if (state == DIE)
    {
        sprite.setColor(Color::White); // 투명도 제거
        window.draw(sprite);
        return; // 죽었으면 아래 무적/평상시 체크 안 함
    }

    // 1. 스킬 무적 (3초 ~ 1.5초 사이) : 깜빡임 없이 색상 유지
    if (invincibleTimer > 1.5f)
    {
        sprite.setColor(Color(255, 255, 200, 255)); // 고휘도 화이트-골드
        window.draw(sprite);
    }
    // 2. 피격 무적 (1.5초 이하) : 기존처럼 깜빡임 적용
    else if (invincibleTimer > 0.f)
    {
        if (static_cast<int>(invincibleTimer * 10) % 2 == 0)
        {
            sprite.setColor(Color(255, 255, 255, 150)); // 일반 피격 (약간 투명하게)
            window.draw(sprite);
        }
    }
    // 3. 평상시
    else
    {
        sprite.setColor(Color::White);
        window.draw(sprite);
    }

    //히트박스 보이게
    RectangleShape debugBox;
    FloatRect hb = GetBounds();
    debugBox.setPosition(hb.left, hb.top);
    debugBox.setSize(Vector2(hb.width, hb.height));
    debugBox.setFillColor(Color(255, 0, 0, 80));
    debugBox.setOutlineColor(Color::Red);
    debugBox.setOutlineThickness(1.f);
    window.draw(debugBox);
}

void Player::Hit()
{
    if (IsInvincible()) return; // 이미 무적이면 데미지 무시

    SetHp(Hp - 1);
    invincibleTimer = 1.5f;
    if (Hp <= 0)
    {
        // 중복 실행 방지를 위해 즉시 체력을 0으로 고정
        Hp = 0;
        info.hp = 0;
        SetState(DIE);
    }
}

void Player::AddGold()
{
    SetGold(Gold + 1);
}

void Player::ClearReward()
{
    int rewardGold = 100; //클리어 보상 골드
    // Getter를 사용하여 현재 체력을 가져옴
    // 포션을 먹어서 5가 넘더라도 그 값이 그대로 sum에 들어감.
    int sum = GetHp(); //배율
    if (sum<0)
    {
        sum = 0;
    }

    //최종 골드 계산
    int currentGold = GetGold();
    SetGold(currentGold + (rewardGold * sum));
}

void Player::InstantDie()
{
    SetHp(0);
    SetState(DIE);
}

bool Player::UseSkill()
{
    //쿨 타임 체크
    if (!skillReady) return false;

    //스킬 사용 처리
    skillReady = false;
    skillCoolTimer = skillCoolTime;

    //무적 부여
    invincibleTimer = 3.0f;

    //상태 변ㄴ경
    SetState(PSKILL);

    return true;
}

FloatRect Player::GetBounds()
{
    //스프라이트 위치 기준으로 히트박스 계산
    Vector2f pos = sprite.getPosition();

    //1 - 슬라이드 상태
    if (state == SLIDE)
    {
        // 서 있을 때(150.f)보다 아래로 내려야 하므로 offset 증가, 높이는 감소
        float slideOffsetY = hoffsetY + 30.f;
        float slideHeight = hheight - 30.f;

        return FloatRect(pos.x + hoffsetX, pos.y + slideOffsetY, hwidth, slideHeight);

    }
    // 2. 일반 상태 (RUN, JUMP 등 기본 히트박스)
    return FloatRect(pos.x + hoffsetX, pos.y + hoffsetY, hwidth, hheight);
}