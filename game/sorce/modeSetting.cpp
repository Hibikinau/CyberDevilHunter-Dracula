/*****************************************************************//**
 * \file   modeSetting.h
 * \brief  設定画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"

bool	modeSetting::Initialize()
{
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("操作設定");
	menuMessage.emplace_back("音量");
	menuMessage.emplace_back("データ初期化");
	menuMessage.emplace_back("パスワード");
	picMenuMaxNum = menuMessage.size() - 1;
	_cg = _modeServer->RS.loadGraphR("game/res/AB.png");
	_ui = _modeServer->RS.loadGraphR("game/res/config_ui.png");
	menuBarHandle = _modeServer->RS.loadGraphR("game/res/menu_bar.png");
	return true;
}

bool	modeSetting::Process()
{
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

	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[4], DX_PLAYTYPE_BACK);

	}
	else if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
	{
		_modeServer->Add(std::make_unique<modeMainMenu>(_modeServer), 1, MODE_MAINMENU);
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
		return false;
	}


	return true;
}

bool	modeSetting::Render()
{
	arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
	backAnimNum < 89 ? backAnimNum++ : backAnimNum = 0;
	DrawExtendGraph(0, 0, 1280, 720, backAnimHandle[backAnimNum], false);
	SetFontSize(80);
	DrawGraph(30, 670, _cg, true);
	DrawGraph(0, 20, _ui, true);
	SetFontSize(30);
	int defY = 200;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		DrawString(90, defY + (100 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));

	}
	DrawGraph(80 - 65, defY + (100 * picMenuNum) + 7, arrowAnimHandle[arrowAnimNum], true);

	DrawGraph(0, 0, menuBarHandle, true);
	return true;
}

bool	modeSetting::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}