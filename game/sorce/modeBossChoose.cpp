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
	if (isPic) {
		if (_imputInf._gTrgb[KEY_INPUT_LEFT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_LEFT])
		{
			pic ^= true;
		}
		if (_imputInf._gTrgb[KEY_INPUT_RIGHT] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_RIGHT])
		{
			pic ^= true;
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			if (pic)
			{//ボス1
				_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
				return false;
			}
			else
			{//ボス2
				isPic = false;
			}


		}
	}
	else {
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
			isPic = true;
			if (picMenuNum == 0)
			{//ボス1
				_modeServer->_valData.popBossNum = 1;
			}
			else if (picMenuNum == 1)
			{//ボス2
				_modeServer->_valData.popBossNum = 2;
			}

		}
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

	}
	if (isPic)
	{
		if (pic) {
			DrawString(1000 - 40, 580, "→", GetColor(255, 255, 255));
		}
		else
		{
			DrawString(1120 - 40, 580, "→", GetColor(255, 255, 255));
		}


	}
	else {
		DrawString(80 - 40, defY + (120 * picMenuNum), "→", GetColor(255, 255, 255));
	}
	DrawString(400, 580, "この依頼を受注しますか？", GetColor(255, 255, 255));
	DrawString(1000, 580, "はい", GetColor(255, 255, 255));
	DrawString(1120, 580, "いいえ", GetColor(255, 255, 255));

	return true;
}

bool	modeBC::Terminate()
{
	SetFontSize(DeffontSize);
	//DeleteGraph(_cg);
	return true;
}