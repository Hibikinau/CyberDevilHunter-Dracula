#include"allMode.h"

bool	modeE::Initialize()
{
	modeT::save("game/res/save.csv", &_modeServer->_valData);

	return true;
}

bool	modeE::Process()
{
	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
		return false;
	}
	return true;
}

bool	modeE::Render()
{
	DrawString(0, 0, "gameClear!!", GetColor(255, 255, 255));
	return true;
}

bool	modeE::Terminate()
{
	return true;
}
