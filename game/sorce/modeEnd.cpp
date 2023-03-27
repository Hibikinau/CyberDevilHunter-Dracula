#include"allMode.h"
#include <iostream>
#include <string>
#include <sstream>

bool	modeE::Initialize()
{
	modeT::save("game/res/save.csv", &_modeServer->_valData);
	creditsY = 720;
	DeffontSize = GetFontSize();
	SetFontSize(40);

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
	for (int i = 0; i < _modeServer->_valData.credits.size(); i++)
	{
		int sizeX, sizeY, lineCount;
		GetDrawStringSize(&sizeX, &sizeY, &lineCount, _modeServer->_valData.credits[i].c_str(), _modeServer->_valData.credits[i].length());
		DrawString(640 - sizeX / 2, creditsY + i * 42, _modeServer->_valData.credits[i].c_str(), GetColor(255, 255, 255));
	}
	creditsY -= 1;
	return true;
}

bool	modeE::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}
