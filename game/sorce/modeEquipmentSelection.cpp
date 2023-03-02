#include"allMode.h"

bool	modeES::Initialize()
{
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("アーツ１");
	menuMessage.emplace_back("アーツ２");
	picMenuMaxNum = menuMessage.size() - 1;
	_modeServer->_valData.popSelectNum = 0;
	return true;
}

bool	modeES::Process()
{
	if (isPic) {
		if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
		{
			pic ^= true;
		}
		if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
		{
			pic ^= true;
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			isPic = false;	
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
				_modeServer->_valData.popSelectNum = 1;
			}
			else if (picMenuNum == 1)
			{//ボス2
				_modeServer->_valData.popSelectNum = 2;
			}

		}else if(_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
		{
			_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
			return false;
		}
	}
	return true;
}

bool	modeES::Render()
{
	//DrawGraph(0, 0, _cg, false);
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("装備選択", strlen("装備選択"));
	DrawString(20, 20, "装備選択", GetColor(255, 255, 255));
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
			DrawString(600 - 40, 200 , "→", GetColor(255, 255, 255));
		}
		else
		{
			DrawString(600 - 40, 400, "→", GetColor(255, 255, 255));
		}


	}
	else {
		DrawString(80 - 40, defY + (120 * picMenuNum), "→", GetColor(255, 255, 255));
	}
	if (picMenuNum == 0) {
		DrawString(600, 200, "ドラクエア", GetColor(255, 255, 255));
		DrawString(600, 400, "サイバー", GetColor(255, 255, 255));

	}
	else {
		DrawString(600, 200, "デビル", GetColor(255, 255, 255));
		DrawString(600, 400, "ハンター", GetColor(255, 255, 255));
	}
	//DrawString(400, 580, "○○にしますか？", GetColor(255, 255, 255));
	//DrawString(1000, 580, "はい", GetColor(255, 255, 255));
	//DrawString(1120, 580, "いいえ", GetColor(255, 255, 255));

	return true;
}

bool	modeES::Terminate()
{
	SetFontSize(DeffontSize);
	//DeleteGraph(_cg);
	return true;
}