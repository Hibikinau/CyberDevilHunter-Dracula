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
	DrawString(640 - 240, 20, "��������", GetColor(255, 255, 255));
	SetFontSize(40);
	DrawString(640 - 200, 160, "��V", GetColor(255, 255, 255));
	DrawString(640 - 200 + 120, 160, "20,000pt", GetColor(255, 255, 255));
	DrawString(300, 250, "�V���ȃA�[�c������\�ɂȂ�܂���\n\n�u�A�[�c���v\n\n�V���ȃA�r���e�B������\�ɂȂ�܂���\n\n�u�A�r���e�B���v\n\n\n���u���[�t�B���O�ɖ߂�", GetColor(255, 255, 255));
	return true;
}

bool	modeR::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}