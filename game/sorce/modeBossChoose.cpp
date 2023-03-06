#include"allMode.h"

bool	modeBC::Initialize()
{
	//_cg = LoadGraph("game/res/chooseBoss.png");
	_modeServer->RS.loadDivGraphR("game/res/mission_UI_animation_01/mission_UI_animation_sheet.png", 62, 1, 62, 600, 450, mapAnimHandol);
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("�{�X�P");
	menuMessage.emplace_back("�{�X�Q");
	picMenuMaxNum = menuMessage.size() - 1;
	_modeServer->_valData.popBossNum = 0;
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/glitch/apngframe01_sheet.png", 18, 2, 9, 800, 450, glitchAnimHandle);
	newsWindow = _modeServer->RS.loadGraphR("game/res/UI_obi.png");
	newsWindowStr = _modeServer->RS.loadGraphR("game/res/UI_news.png");
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
			{//�Q�[���ֈڍs
				_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
				return false;
			}
			else
			{//�L�����Z��
				isPic = false; pic = true;
			}


		}
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
			{//�{�X1
				_modeServer->_valData.popBossNum = 1;
			}
			else if (picMenuNum == 1)
			{//�{�X2
				_modeServer->_valData.popBossNum = 2;
			}

		}

		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
		{
			_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
			return false;
		}
	}
}
//680, 120
bool	modeBC::Render()
{
	SetFontSize(36);
	backAnimNum < 89 ? backAnimNum++ : backAnimNum = 0;
	mapAnimNum < 61 ? mapAnimNum++ : mapAnimNum = 61;
	DrawExtendGraph(0, 0, 1280, 720, backAnimHandle[backAnimNum], true);
	DrawExtendGraph(800, 20, 1260, 320, mapAnimHandol[mapAnimNum], true);
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
	else { newsPosX-=5; }
	DrawExtendGraph(0, 660, 1280, 700, newsWindowStr, true);

	SetFontSize(40);
	int defY = 150;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(80, defY + (90 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));

	}
	if (isPic)
	{
		if (pic) {
			DrawString(1000 - 40, 580, "��", GetColor(255, 255, 255));
		}
		else
		{
			DrawString(1120 - 40, 580, "��", GetColor(255, 255, 255));
		}
	}
	else {
		DrawString(80 - 40, defY + (90 * picMenuNum), "��", GetColor(255, 255, 255));
	}
	DrawString(400, 580, "���̈˗����󒍂��܂����H", GetColor(255, 255, 255));
	DrawString(1000, 580, "�͂�", GetColor(255, 255, 255));
	DrawString(1120, 580, "������", GetColor(255, 255, 255));

	SetFontSize(30);
	if (picMenuNum == 0) {
		DrawString(300, 150, _modeServer->_valData.boss1Inf.c_str(), GetColor(255, 255, 255));
	}
	else {
		DrawString(300, 150, _modeServer->_valData.boss2Inf.c_str(), GetColor(255, 255, 255));
	}

	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("�����{�X�I��", strlen("�����{�X�I��"));
	DrawString(20, 20, "�����{�X�I��", GetColor(255, 255, 255));
	return true;
}

bool	modeBC::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}