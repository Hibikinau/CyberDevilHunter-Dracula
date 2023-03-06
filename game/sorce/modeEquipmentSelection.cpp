#include"allMode.h"

bool	modeES::Initialize()
{
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	wakuHandle = _modeServer->RS.loadGraphR("game/res/wakuUI2.png");
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("アーツ１(X)");
	menuMessage.emplace_back("アーツ２(Y)");
	picMenuMaxNum = menuMessage.size() - 1;
	popSelectNum = 0;
	return true;
}

bool	modeES::Process()
{
	if (isPic) {
		if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
		{
			pic == 2 ? pic = 0 : pic++;
		}
		if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
		{
			pic == 0 ? pic = 2 : pic--;
		}

		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B]) { isPic = false; }
		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			isPic = false;
			if (picMenuNum == 0) { _modeServer->_valData.plChangeAttackX = _modeServer->_valData.changeAttackList[pic]; }
			if (picMenuNum == 1) { _modeServer->_valData.plChangeAttackY = _modeServer->_valData.changeAttackList[pic]; }
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
		}
		else if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
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
	arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
	backAnimNum < 89 ? backAnimNum++ : backAnimNum = 0;
	DrawExtendGraph(0, 0, 1280, 720, backAnimHandle[backAnimNum], false);
	//DrawGraph(15, 110, wakuHandle, true);
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("装備選択", strlen("装備選択"));
	DrawString(20, 20, "装備選択", GetColor(255, 255, 255));

	SetFontSize(40);
	DrawString(20, 150, "攻撃技選択", GetColor(255, 255, 255));
	int defY = 200;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		std::string insStr = menuMessage[i] + "\n";
		if (i == 0) { insStr += _modeServer->_valData.plChangeAttackX; }
		if (i == 1) { insStr += _modeServer->_valData.plChangeAttackY; }
		DrawString(80, defY + (120 * i), insStr.c_str(), GetColor(255, 255, 255));

	}
	if (isPic)
	{
		DrawGraph(450 - 65, 200 + (pic * 100) + 7, arrowAnimHandle[arrowAnimNum], true);
	}
	else {
		DrawGraph(80 - 65, defY + (120 * picMenuNum) + 7, arrowAnimHandle[arrowAnimNum], true);
	}
	if (picMenuNum == 0 || picMenuNum == 1) {
		for (int i = 0; i < _modeServer->_valData.changeAttackList.size(); i++)
		{
			DrawString(450, 200 + (100 * i), _modeServer->_valData.changeAttackList[i].c_str(), GetColor(255, 255, 255));
		}
	}
	//else {	}


	return true;
}

bool	modeES::Terminate()
{
	SetFontSize(DeffontSize);
	modeT::save("game/res/save.csv", &_modeServer->_valData);
	return true;
}