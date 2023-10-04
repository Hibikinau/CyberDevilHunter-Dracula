/*****************************************************************//**
 * \file   modeResult.h
 * \brief  リザルト画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"

bool	modeResult::Initialize()
{
	_cg = _modeServer->RS.loadGraphR("game/res/result.png");
	DeffontSize = GetFontSize();
	PlaySoundMem(_modeServer->_valData.menuSoundHandle[7], DX_PLAYTYPE_BACK);
	modeTitle::save("game/res/save.csv", &_modeServer->_valData);
	return true;
}

bool	modeResult::Process()
{
	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		_modeServer->Add(std::make_unique<modeMainMenu>(_modeServer), 1, MODE_MAINMENU);
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
		return false;
	}
	return true;
}

bool	modeResult::Render()
{
	DrawGraph(0, 0, _cg, true);
	SetFontSize(120);
	DrawString(640 - 240, 20, "討伐完了", GetColor(255, 255, 255));
	SetFontSize(40);
	DrawString(640 - 200, 200, "報酬", GetColor(255, 255, 255));
	DrawString(640 - 200 + 120, 200, "20,000pt", GetColor(255, 255, 255));
	DrawString(640 - 220, 650, "→メインメニューに戻る", GetColor(255, 255, 255));
	return true;
}

bool	modeResult::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}