#include"allMode.h"

bool	modeBC::Initialize()
{
	_cg = LoadGraph("game/res/chooseBoss.png");
	return true;
}

bool	modeBC::Process()
{
	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
		return false;
	}
	return true;
}

bool	modeBC::Render()
{
	DrawGraph(0, 0, _cg, false);

	return true;
}

bool	modeBC::Terminate()
{
	DeleteGraph(_cg);
	return true;
}