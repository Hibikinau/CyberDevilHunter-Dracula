#include"allMode.h"

bool	modeBC::Initialize()
{
	//_cg = LoadGraph("game/res/chooseBoss.png");
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("ボス１");
	menuMessage.emplace_back("ボス２");
	picMenuMaxNum = menuMessage.size() - 1;
	_modeServer->_valData.popBossNum = 0;
	return true;
}

bool	modeBC::Process()
{
	if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
	{
		picMenuNum == picMenuMaxNum ? picMenuNum = 0 : picMenuNum++;
	}
	if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
	{
		picMenuNum == 0 ? picMenuNum = picMenuMaxNum : picMenuNum--;
	}

	if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
	{
		if (picMenuNum == 0)
		{//ボス1
			_modeServer->_valData.popBossNum = 1;
		}
		else if (picMenuNum == 1)
		{//ボス2

		}
		_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
		return false;
	}
}

bool	modeBC::Render()
{
	//DrawGraph(0, 0, _cg, false);
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("討伐ボス選択", strlen("討伐ボス選択"));
	DrawString(20, 20, "討伐ボス選択", GetColor(255, 255, 255));
	SetFontSize(40);
	int defY = 200;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(80, defY + (120 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));
		if (picMenuNum == i)
		{
			DrawString(80 - 40, defY + (120 * i), "→", GetColor(255, 255, 255));
		}
	}

	return true;
}

bool	modeBC::Terminate()
{
	SetFontSize(DeffontSize);
	//DeleteGraph(_cg);
	return true;
}