#include"allMode.h"

bool	modeBC::Initialize()
{
	//_cg = LoadGraph("game/res/chooseBoss.png");
	_modeServer->RS.loadDivGraphR("game/res/mission_UI_animation_01/mission_UI_animation_sheet.png", 62, 1, 62, 600, 450, mapAnimHandol);
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("　　　　ボス１\n「クエスト名」");
	menuMessage.emplace_back("　　　　ボス２\n「クエスト名」");
	picMenuMaxNum = menuMessage.size() - 1;
	picMenuNum = 0;
	_modeServer->_valData.popBossNum = 0;
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/glitch/apngframe01_sheet.png", 18, 2, 9, 800, 450, glitchAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	newsWindow = _modeServer->RS.loadGraphR("game/res/UI_obi.png");
	newsWindowStr = _modeServer->RS.loadGraphR("game/res/UI_news.png");
	wakuHandle = _modeServer->RS.loadGraphR("game/res/wakuUI2.png");
	mapAnimNum = 0, backAnimNum = 0;
	randomNum = (rand() % 8) + 10;
	randomNewsNum = rand() % _modeServer->_valData.news.size();
	newsPosX = 1280;
	return true;
}

bool	modeBC::Process()
{
	if (isPic) {
		if (_imputInf._gTrgb[KEY_INPUT_LEFT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_LEFT])
		{
			pic ^= true;
		}
		if (_imputInf._gTrgb[KEY_INPUT_RIGHT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_RIGHT])
		{
			pic ^= true;
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			if (pic)
			{//ゲームへ移行
				_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
				return false;
			}
			else
			{//キャンセル
				isPic = false; pic = true;
			}
		}
		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B]) { isPic = false; pic = true; }
	}
	else {
		if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
		{
			picMenuNum == picMenuMaxNum ? picMenuNum = 0 : picMenuNum++;
		}
		if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
		{
			picMenuNum == 0 ? picMenuNum = picMenuMaxNum : picMenuNum--;
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			isPic = true;
			if (picMenuNum == 0)
			{//ボス1
				_modeServer->_valData.popBossNum = 1;
			}
			else if (picMenuNum == 1)
			{//ボス2
				_modeServer->_valData.popBossNum = 2;
			}

		}

		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
		{
			_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
			return false;
		}
	}

	return true;
}
//680, 120
bool	modeBC::Render()
{
	SetFontSize(36);
	arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
	backAnimNum < 89 ? backAnimNum++ : backAnimNum = 0;
	mapAnimNum < 61 ? mapAnimNum++ : mapAnimNum = 61;
	DrawExtendGraph(0, 0, 1280, 720, backAnimHandle[backAnimNum], true);
	DrawBox(300, 110, 1260, 560, GetColor(255, 0, 255), true);
	DrawExtendGraph(800, 20, 1270, 320, mapAnimHandol[mapAnimNum], true);
	DrawGraph(15, 110, wakuHandle, true);

	//neonAnimNum < 61 ? neonAnimNum++ : neonAnimNum = 0;
	//DrawExtendGraph(-110, -10, 410, 770, neonAnimHandle[neonAnimNum], true);

	if (randomFrameNum <= 0)
	{
		if (glitchAnimNum < randomNum) { glitchAnimNum++; }
		else
		{
			glitchAnimNum = 0;
			randomFrameNum = (rand() % 120) + 20;
			randomNum = (rand() % 14) + 5;
		}
		DrawExtendGraph(800, 20, 1260, 320, glitchAnimHandle[glitchAnimNum], true);
	}
	else { randomFrameNum--; }

	DrawExtendGraph(0, 660, 1280, 700, newsWindow, true);
	const char* insStr = _modeServer->_valData.news[randomNewsNum].c_str();
	DrawString(newsPosX, 662, insStr, GetColor(255, 255, 255));
	int insStrWidth = GetDrawStringWidth(insStr, strlen(insStr));
	if (newsPosX + insStrWidth < 0)
	{
		randomNewsNum = rand() % _modeServer->_valData.news.size();
		newsPosX = 1280;
	}
	else { newsPosX -= 2; }
	DrawExtendGraph(0, 660, 1280, 700, newsWindowStr, true);

	int defY = 120;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(10, defY + (110 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));

	}
	if (isPic)
	{
		if (pic) {
			DrawGraph(900 - 65, 580 + 7, arrowAnimHandle[arrowAnimNum], true);
			//DrawString(1000 - 40, 580, "→", GetColor(255, 255, 255));
		}
		else
		{
			DrawGraph(1090 - 65, 580 + 7, arrowAnimHandle[arrowAnimNum], true);
			//DrawString(1120 - 40, 580, "→", GetColor(255, 255, 255));
		}
	}
	else {
		DrawGraph(70, defY + (110 * picMenuNum) + 7, arrowAnimHandle[arrowAnimNum], true);
	}

	SetFontSize(30);
	if (picMenuNum == 0) {
		DrawString(300, 120, _modeServer->_valData.boss1Inf.c_str(), GetColor(255, 255, 255));
	}
	else {
		DrawString(300, 120, _modeServer->_valData.boss2Inf.c_str(), GetColor(255, 255, 255));
	}

	SetFontSize(40);
	DrawString(300, 580, "この依頼を受注しますか？", GetColor(255, 255, 255));
	DrawString(900, 580, "はい", GetColor(255, 255, 255));
	DrawString(1090, 580, "いいえ", GetColor(255, 255, 255));


	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("討伐ボス選択", strlen("討伐ボス選択"));
	DrawString(20, 20, "討伐ボス選択", GetColor(255, 255, 255));
	return true;
}

bool	modeBC::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}