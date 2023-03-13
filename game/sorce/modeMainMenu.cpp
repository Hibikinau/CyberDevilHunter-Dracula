/*****************************************************************//**
 * \file   modeMainMenu.h
 * \brief  メインメニュー画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"

bool	modeMM::Initialize()
{
	DeffontSize = GetFontSize();
	menuMessage.emplace_back("討伐ボス選択");
	menuMessage.emplace_back("装備変更");
	menuMessage.emplace_back("アーツ取得");
	menuMessage.emplace_back("設定");
	picMenuNum = _modeServer->_valData.mainMenuPicNum;
	picMenuMaxNum = menuMessage.size() - 1;
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_Window_open/apngframe01_sheet.png", 29, 2, 15, 800, 600, windowAnimHandle);
	if (_modeServer->_valData.menuSoundHandle.size() == 0)
	{
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_select_01.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_decide_01.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_decide_02.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_cancel.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_ng_01.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_gameover.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_boss_select.mp3"));
		_modeServer->_valData.menuSoundHandle.emplace_back(LoadSoundMem("game/res/SE/system/system_result.mp3"));
	}
	isBackTitle = false;
	return true;
}

bool	modeMM::Process()
{
	if (!CheckMusic())
	{
		if (_modeServer->_valData.bgmSwitchNum % 2) { { PlayMusic("game/res/BGM/menuBGM_01_Rob Gasser - Ricochet [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		else { if (!CheckMusic()) { PlayMusic("game/res/BGM/menuBGM_02_ANGELPLAYA - LET THEM HAVE IT [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		SetVolumeMusic(255 * (0.01 * _modeServer->_valData.soundMasterValume));
	}

	if (isBackTitle)
	{
		if (_imputInf._gTrgb[KEY_INPUT_LEFT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_LEFT]
			|| _imputInf._gTrgb[KEY_INPUT_RIGHT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_RIGHT])
		{
			isBackTitlePic ^= true;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
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
	}

	if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
	{
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
		isBackTitle ^= true;
		windowAnimNum = 0;
	}

	if (_imputInf._gTrgb[KEY_INPUT_Z] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		if (isBackTitle)
		{
			if (isBackTitlePic)
			{
				StopMusic();
				_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
				PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
				return false;
			}
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
			isBackTitle ^= true;
		}
		else
		{
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
			if (picMenuNum == 0)
			{//討伐ボス選択
				_modeServer->Add(std::make_unique<modeBC>(_modeServer), 1, MODE_BC);
				return false;
			}
			else if (picMenuNum == 1)
			{//装備変更
				_modeServer->Add(std::make_unique<modeES>(_modeServer), 1, MODE_ES);
				return false;
			}
			else if (picMenuNum == 2)
			{//アーツ取得
				_modeServer->Add(std::make_unique<modeAG>(_modeServer), 1, MODE_AG);
				return false;
			}
			else if (picMenuNum == 3)
			{//設定
				_modeServer->Add(std::make_unique<modeS>(_modeServer), 1, MODE_S);
				return false;
			}
		}
	}
	return true;
}

bool	modeMM::Render()
{
	backAnimNum < 89 ? backAnimNum++ : backAnimNum = 0;
	DrawExtendGraph(0, 0, 1280, 720, backAnimHandle[backAnimNum], false);
	SetFontSize(80);
	ChangeFontType(DX_FONTTYPE_EDGE);
	DrawString(0, 0, "BRIEFING", GetColor(255, 255, 255), GetColor(0, 0, 0));
	ChangeFontType(DX_FONTTYPE_NORMAL);
	SetFontSize(40);
	int defY = 200;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(80, defY + (120 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));
		if (picMenuNum == i)
		{
			arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
			DrawGraph(80 - 65, defY + (120 * i) + 7, arrowAnimHandle[arrowAnimNum], true);
			//DrawString(80 - 40, defY + (120 * i), "→", GetColor(255, 255, 255));
		}
	}
	if (isBackTitle)
	{//240
		DrawBox(350, 235, 930, 485, GetColor(1, 1, 1), true);
		windowAnimNum < 28 ? windowAnimNum++ : windowAnimNum = 28;
		DrawExtendGraph(330, 215, 950, 505, windowAnimHandle[windowAnimNum], true);
		DrawString(420, 280, "タイトルに戻りますか？", GetColor(255, 255, 255));
		DrawString(500, 370, "はい　　いいえ", GetColor(255, 255, 255));
		arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
		if (isBackTitlePic) { DrawGraph(500 - 65, 370 + 7, arrowAnimHandle[arrowAnimNum], true); }
		else { DrawGraph(660 - 65, 370 + 7, arrowAnimHandle[arrowAnimNum], true); }
	}

	return true;
}

bool	modeMM::Terminate()
{
	SetFontSize(DeffontSize);
	if (_modeServer->_valData.bgmSwitchNum > 1) { _modeServer->_valData.bgmSwitchNum--; }
	_modeServer->_valData.mainMenuPicNum = picMenuNum;
	return true;
}