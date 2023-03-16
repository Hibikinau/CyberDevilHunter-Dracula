/*****************************************************************//**
 * \file   modeBossChoose.cpp
 * \brief  ボス選択クラス
 *
 * \author 脇添敏暉
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"

bool	modeBC::Initialize()
{
	//_cg = LoadGraph("game/res/chooseBoss.png");
	_modeServer->RS.loadDivGraphR("game/res/mission_UI_animation_01/mission_UI_animation_sheet.png", 62, 1, 62, 600, 450, mapAnimHandol);
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("　　　　ボス１\n「騎士」");
	menuMessage.emplace_back("　　　　ボス２\n「ライオンマン」");
	picMenuMaxNum = menuMessage.size() - 1;
	picMenuNum = 0;
	_modeServer->_valData.popBossNum = 0;
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/glitch/apngframe01_sheet.png", 18, 2, 9, 800, 450, glitchAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	newsWindow = _modeServer->RS.loadGraphR("game/res/UI_obi.png");
	newsWindowStr = _modeServer->RS.loadGraphR("game/res/UI_news.png");
	wakuHandle = _modeServer->RS.loadGraphR("game/res/wakuUI.png");
	wakuHandle2 = _modeServer->RS.loadGraphR("game/res/wakuUI3.png");
	titleHandle = _modeServer->RS.loadGraphR("game/res/mission_ui.png");
	checkUiHandle = _modeServer->RS.loadGraphR("game/res/kakunin_ui.png");
	bossUiHandle = _modeServer->RS.loadGraphR("game/res/boss_ui.png");
	menuBarHandle = _modeServer->RS.loadGraphR("game/res/menu_bar.png");
	mapAnimNum = 0, backAnimNum = 0;
	randomNum = (rand() % 8) + 10;
	randomNewsNum = rand() % _modeServer->_valData.news.size();
	newsPosX = 1280;
	PlaySoundMem(_modeServer->_valData.menuSoundHandle[6], DX_PLAYTYPE_BACK);

	return true;
}

bool	modeBC::Process()
{
	if (!CheckMusic())
	{
		if (_modeServer->_valData.bgmSwitchNum % 2) { { PlayMusic("game/res/BGM/menuBGM_01_Rob Gasser - Ricochet [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		else { if (!CheckMusic()) { PlayMusic("game/res/BGM/menuBGM_02_ANGELPLAYA - LET THEM HAVE IT [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		SetVolumeMusic(255 * (0.01 * _modeServer->_valData.soundMasterValume));
	}

	if (isPic) {
		if (_imputInf._gTrgb[KEY_INPUT_LEFT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_LEFT])
		{
			pic ^= true;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}
		if (_imputInf._gTrgb[KEY_INPUT_RIGHT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_RIGHT])
		{
			pic ^= true;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			if (pic)
			{//ゲームへ移行
				_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
				PlaySoundMem(_modeServer->_valData.menuSoundHandle[2], DX_PLAYTYPE_BACK);
				StopMusic();
				return false;
			}
			else
			{//キャンセル
				isPic = false; pic = true;
				PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
			}
		}
		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
		{
			isPic = false; pic = true;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
		}
	}
	else {
		if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
		{
			picMenuNum == picMenuMaxNum ? picMenuNum = 0 : picMenuNum++;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}
		if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
		{
			picMenuNum == 0 ? picMenuNum = picMenuMaxNum : picMenuNum--;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			isPic = true;
			if (picMenuNum == 0)
			{//ボス1
				_modeServer->_valData.popBossNum = 1;
				PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
			}
			else if (picMenuNum == 1)
			{//ボス2
				_modeServer->_valData.popBossNum = 2;
				PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
			}
		}

		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
		{
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
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
	//DrawBox(300, 110, 1260, 560, GetColor(255, 0, 255), true);
	DrawGraph(300, 80, wakuHandle2, true);
	DrawGraph(15, 85, wakuHandle, true);

	//neonAnimNum < 61 ? neonAnimNum++ : neonAnimNum = 0;
	//DrawExtendGraph(-110, -10, 410, 770, neonAnimHandle[neonAnimNum], true);

	DrawGraph(915, 380, bossUiHandle, true);
	DrawExtendGraph(910, 100, 1270, 360, mapAnimHandol[mapAnimNum], true);
	if (randomFrameNum <= 0)
	{
		if (glitchAnimNum < randomNum) { glitchAnimNum++; }
		else
		{
			glitchAnimNum = 0;
			randomFrameNum = (rand() % 120) + 20;
			randomNum = (rand() % 14) + 5;
		}
		DrawExtendGraph(910, 115, 1270, 345, glitchAnimHandle[glitchAnimNum], true);
	}
	else { randomFrameNum--; }

	int defY = 140;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(15, defY + (110 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));

	}

	SetFontSize(30);
	if (picMenuNum == 0) {
		DrawString(320, 140, _modeServer->_valData.boss1Inf.c_str(), GetColor(255, 255, 255));
	}
	else {
		DrawString(320, 140, _modeServer->_valData.boss2Inf.c_str(), GetColor(255, 255, 255));
	}

	DrawGraph(0, 660, newsWindow, true);
	const char* insStr = _modeServer->_valData.news[randomNewsNum].c_str();
	DrawString(newsPosX, 672, insStr, GetColor(255, 255, 255));
	int insStrWidth = GetDrawStringWidth(insStr, strlen(insStr));
	if (newsPosX + insStrWidth < 0)
	{
		randomNewsNum = rand() % _modeServer->_valData.news.size();
		newsPosX = 1280;
	}
	else { newsPosX -= 2; }
	DrawGraph(0, 660, newsWindowStr, false);

	if (isPic)
	{
		DrawGraph(468, 238, checkUiHandle, true);

		if (pic) {
			DrawGraph(540 , 430 , arrowAnimHandle[arrowAnimNum], true);
			//DrawString(1000 - 40, 580, "→", GetColor(255, 255, 255));
		}
		else
		{
			DrawGraph(690 , 430, arrowAnimHandle[arrowAnimNum], true);
			//DrawString(1120 - 40, 580, "→", GetColor(255, 255, 255));
		}
	}
	else {
		DrawGraph(80, defY + (110 * picMenuNum) + 7, arrowAnimHandle[arrowAnimNum], true);
	}

	DrawGraph(0, 20, titleHandle, true);
	DrawGraph(0, 0, menuBarHandle, true);
	//SetFontSize(80);maskData131, 53
	//int insMenuFontSize = GetDrawStringWidth("ミッション選択", strlen("ミッション選択"));
	//DrawString(20, 20, "ミッション選択", GetColor(255, 255, 255));
	return true;
}

bool	modeBC::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}