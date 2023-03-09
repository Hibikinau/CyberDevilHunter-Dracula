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
	return true;
}

bool	modeMM::Process()
{
	if (isBackTitle && _imputInf._gRelb[KEY_INPUT_Z] || _imputInf._gRelp[XINPUT_BUTTON_A])
	{
		_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
		return false;
	}
	if (!CheckMusic())
	{
		if (_modeServer->_valData.bgmSwitchNum % 2) { { PlayMusic("game/res/BGM/menuBGM_01_Rob Gasser - Ricochet [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		else { if (!CheckMusic()) { PlayMusic("game/res/BGM/menuBGM_02_ANGELPLAYA - LET THEM HAVE IT [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		SetVolumeMusic(255 * (0.01 * _modeServer->_valData.soundMasterValume));
	}

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

	 if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
	{
		isBackTitle = true;
	}

	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
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
	{
		DrawBox(340, 300, 900, 420, GetColor(255, 255, 255), true);
		DrawString(360, 310, "タイトルに戻りますか？", GetColor(0, 0, 0));
		DrawString(420, 370, "はい　　いいえ", GetColor(0, 0, 0));
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