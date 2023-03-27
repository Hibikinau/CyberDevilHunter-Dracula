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
	if (!CheckMusic()) {
		PlayMusic("game/res/BGM/Jnathyn_-_Dioma_NCS_Release.mp3", DX_PLAYTYPE_BACK);		SetVolumeMusic(255 * (0.01 * 40));
	}

	if (_imputInf._gKeyb[KEY_INPUT_SPACE] || _imputInf._gKeyp[XINPUT_BUTTON_RIGHT_SHOULDER])
	{
		creditsY -= 5;
	}

	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
		return false;
	}

	if (creditsY + static_cast<int>(_modeServer->_valData.credits.size()) * 42 < 0)
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
	StopMusic();
	return true;
}
