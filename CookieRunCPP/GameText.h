#pragma once
#include "Defiens.h"

class GameText
{
private:
	static Font font;
	Text introText;
	Text scoreText;
	Text titleText;
	Text btnText[6];

	//스테이지 선택
	Text stageSelectTitle;
	Text stageText[2];

	//인벤토리
	Text countText;
	Text invenTitleText;
	Text guideText;
	Text msgText;
	Text noPotion;
	Text pt;

public:
	GameText()
	{
		font.loadFromFile("CookieRun Bold.ttf"); //폰트 한번만 로드
		
		//인트로 텍스트
		introText.setFont(font);
		introText.setCharacterSize(24);
		introText.setFillColor(Color::Cyan);
		introText.setPosition(600, 400);

		//점수 텍스트/ 시간 텍스트
		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(Color::White);
		scoreText.setPosition(10, 10);

		//타이틀 텍스트
		titleText.setFont(font);
		titleText.setCharacterSize(48);
		titleText.setFillColor(Color::Magenta);
		titleText.setPosition(600, 50);

		//스테이지 선택 텍스트
		stageSelectTitle.setFont(font);
		stageSelectTitle.setString("SELECT STAGE");
		stageSelectTitle.setCharacterSize(48);
		stageSelectTitle.setFillColor(Color::Yellow);
		FloatRect tb = stageSelectTitle.getLocalBounds();
		stageSelectTitle.setOrigin(tb.width / 2.f, tb.height / 2.f);
		stageSelectTitle.setPosition(750, 100);

		// 스테이지 1 버튼
		stageText[0].setFont(font);
		stageText[0].setString("1 - STAGE 1");
		stageText[0].setCharacterSize(36);
		stageText[0].setFillColor(Color::White);
		stageText[0].setPosition(600, 250);

		// 보스 스테이지 버튼
		stageText[1].setFont(font);
		stageText[1].setCharacterSize(36);
		stageText[1].setPosition(600, 350);

		// 메뉴 버튼 기본 세팅
		for (int i = 0; i < 6; i++) 
		{
			btnText[i].setFont(font);
			btnText[i].setCharacterSize(24);
			btnText[i].setFillColor(Color::Magenta);
		}

		//인벤토리 관련 텍스트 설정
		countText.setFont(font);
		countText.setCharacterSize(18);
		countText.setFillColor(Color::Yellow);
		countText.setOutlineColor(Color::Black);
		countText.setOutlineThickness(2.f);

		invenTitleText.setFont(font);
		invenTitleText.setCharacterSize(40);
		invenTitleText.setFillColor(Color::Yellow);

		guideText.setFont(font);
		guideText.setCharacterSize(22);
		guideText.setFillColor(Color(180, 180, 180));

		msgText.setFont(font);
		msgText.setCharacterSize(24);
		msgText.setFillColor(Color(255, 150, 150));

		noPotion.setFont(font);
		noPotion.setCharacterSize(28);
		noPotion.setFillColor(Color(180, 180, 180));

		pt.setFont(font);
		pt.setCharacterSize(26);
		pt.setFillColor(Color::White);
	}

	void SetIntro(const string& str) { introText.setString(str); }
	void SetScore(const string& str) { scoreText.setString(str); }
	void SetTitle(const string& str) { titleText.setString(str); }



	static Font& GetFont() { return font; }

	// 버튼 텍스트 내용과 위치를 외부에서 정해주는 함수
	void SetMenuButton(int index, const string& str, float x, float y) 
	{
		btnText[index].setString(str);

		// 중앙 정렬 로직도 여기서 처리
		FloatRect bounds = btnText[index].getLocalBounds();
		btnText[index].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		btnText[index].setPosition(x, y);
	}

	// 메뉴 버튼 전용 렌더 함수
	void RenderMenu(RenderWindow& window)
	{
		for (int i = 0; i < 6; i++) {
			window.draw(btnText[i]);
		}
		window.draw(titleText); // 타이틀도 같이 그림
	}

	// 스테이지 선택 텍스트 설정
	void SetStageText(int index, const string& str, Color color = Color::White)
	{
		stageText[index].setString(str);
		stageText[index].setFillColor(color);
	}

	void Render(RenderWindow& window)
	{
		window.draw(introText);
		window.draw(scoreText);
		window.draw(titleText);
	}

	// 스테이지 선택 화면 렌더
	void RenderStageSelect(RenderWindow& window)
	{
		window.draw(stageSelectTitle);
		window.draw(stageText[0]);
		window.draw(stageText[1]);
	}

	//인벤토리 텍스트 관련 함수
	void SetCountText(RenderWindow& window, const string& str, float x, float y)
	{ 
		countText.setString(str); 
		FloatRect tb = countText.getLocalBounds();
		countText.setPosition(x - tb.width - 4.f, y - tb.height - 8.f);
		window.draw(countText);
	}
	void SetInvenTitle(const string& str) 
	{ 
		invenTitleText.setString(str); 
		FloatRect tb = invenTitleText.getLocalBounds();
		invenTitleText.setOrigin(tb.width / 2.f, tb.height / 2.f);
		invenTitleText.setPosition(750.f, 50.f);
	}
	void SetGuide(const string& str) 
	{
		guideText.setString(str); 
		guideText.setPosition(20.f, 560.f);
	}

	void SetMsgText(const string& str) 
	{ 
		msgText.setString(str); 
		msgText.setFillColor(Color::Red);
		FloatRect mb = msgText.getLocalBounds();
		msgText.setOrigin(mb.width / 2.f, mb.height / 2.f);
		msgText.setPosition(750.f, 430.f);
	}
	void SetNopotion(const string& str) 
	{ 
		noPotion.setString(str); 
		FloatRect nb = noPotion.getLocalBounds();
		noPotion.setOrigin(nb.width / 2.f, nb.height / 2.f);
		noPotion.setPosition(750.f, 280.f);
	}

	//개별 포션 항목과 개수 텍스트를 위한 출력 함수
	void Setpt(RenderWindow& window, const string& str, float y)
	{ 
		pt.setString(str); 
		FloatRect ptb = pt.getLocalBounds();
		pt.setOrigin(ptb.width / 2.f, ptb.height / 2.f);
		pt.setPosition(750.f, y);
		window.draw(pt);
	}
	//인벤토리용 렌더 함수 추가
	void RenderInventory(RenderWindow& window) 
	{
		window.draw(invenTitleText);
		window.draw(guideText);
		if (!msgText.getString().isEmpty()) window.draw(msgText);
	}

	//포션 사용 화면용 렌더 함수 (내용이 없을 때 noPotion 출력 포함)
	void RenderPotionSelect(RenderWindow& window, bool isEmpty) 
	{
		window.draw(invenTitleText); // "Use a Potion?" 타이틀로 재사용 가능
		window.draw(guideText);
		if (isEmpty) window.draw(noPotion);
		if (!msgText.getString().isEmpty()) window.draw(msgText);
	}
};