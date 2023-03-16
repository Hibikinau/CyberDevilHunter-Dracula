/*****************************************************************//**
 * \file   modeArtsGet.h
 * \brief  アーツ取得画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"

bool	modeAG::Initialize()
{
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("・入れ替え1");
	menuMessage.emplace_back("・入れ替え2");
	menuMessage.emplace_back("・入れ替え3");
	menuMessage.emplace_back("・入れ替え4");
	picMenuMaxNum = menuMessage.size() - 1;
	_cg = _modeServer->RS.loadGraphR("game/res/AB.png");
	_ui = _modeServer->RS.loadGraphR("game/res/arts_ui.png");
	return true;
}

bool	modeAG::Process()
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
		_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
		return false;
	}


	return true;
}

bool	modeAG::Render()
{
	arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
	backAnimNum < 89 ? backAnimNum++ : backAnimNum = 0;
	DrawExtendGraph(0, 0, 1280, 720, backAnimHandle[backAnimNum], false);
	SetFontSize(80);
	DrawGraph(30, 670, _cg, true);
	DrawGraph(0, 40, _ui, true);
	SetFontSize(30);
	int defY = 200;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		DrawString(90, defY + (50 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));

	}
	DrawGraph(80 - 65, defY + (50 * picMenuNum) + 7, arrowAnimHandle[arrowAnimNum], true);

	return true;
}

bool	modeAG::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}