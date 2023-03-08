#include"allMode.h"

bool	modeR::Initialize()
{
	_cg = _modeServer->RS.loadGraphR("game/res/result.png");
	DeffontSize = GetFontSize();
	return true;
}

bool	modeR::Process()
{
	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
		return false;
	}
	return true;
}

bool	modeR::Render()
{
	DrawGraph(0, 0, _cg, true);
	SetFontSize(120);
	DrawString(640 - 240, 20, "討伐完了", GetColor(255, 255, 255));
	SetFontSize(40);
	DrawString(640 - 200, 160, "報酬", GetColor(255, 255, 255));
	DrawString(640 - 200 + 120, 160, "20,000pt", GetColor(255, 255, 255));
	DrawString(300, 250, "新たなアーツが解放可能になりました\n\n「アーツ名」\n\n新たなアビリティが解放可能になりました\n\n「アビリティ名」\n\n\n→ブリーフィングに戻る", GetColor(255, 255, 255));
	return true;
}

bool	modeR::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}