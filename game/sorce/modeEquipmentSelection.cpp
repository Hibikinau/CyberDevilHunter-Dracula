#include"allMode.h"

bool	modeES::Initialize()
{
	_modeServer->RS.loadDivGraphR("game/res/CCF_Cyber_BG_E/apngframe01_sheet.png", 90, 3, 30, 600, 450, backAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	wakuHandle = _modeServer->RS.loadGraphR("game/res/wakuUI.png");
	wakuHandle2 = _modeServer->RS.loadGraphR("game/res/wakuUI3.png");
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
	if (!CheckMusic())
	{
		if (_modeServer->_valData.bgmSwitchNum % 2) { { PlayMusic("game/res/BGM/menuBGM_01_Rob Gasser - Ricochet [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		else { if (!CheckMusic()) { PlayMusic("game/res/BGM/menuBGM_02_ANGELPLAYA - LET THEM HAVE IT [NCS Release].mp3", DX_PLAYTYPE_BACK); _modeServer->_valData.bgmSwitchNum++; } }
		SetVolumeMusic(255 * (0.01 * _modeServer->_valData.soundMasterValume));
	}

	if (isPic) {
		if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
		{
			pic == 2 ? pic = 0 : pic++;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}
		if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
		{
			pic == 0 ? pic = 2 : pic--;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}

		if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B]) { isPic = false;
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
		}
		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			isPic = false;
			if (picMenuNum == 0) { _modeServer->_valData.plChangeAttackX = _modeServer->_valData.changeAttackList[pic]; }
			if (picMenuNum == 1) { _modeServer->_valData.plChangeAttackY = _modeServer->_valData.changeAttackList[pic]; }
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
		}
	}
	else {
		if (_imputInf._gTrgb[KEY_INPUT_DOWN] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_DOWN])
		{
			picMenuNum == picMenuMaxNum ? picMenuNum = 0 : picMenuNum++;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}
		if (_imputInf._gTrgb[KEY_INPUT_UP] || _imputInf._gTrgp[XINPUT_BUTTON_DPAD_UP])
		{
			picMenuNum == 0 ? picMenuNum = picMenuMaxNum : picMenuNum--;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[0], DX_PLAYTYPE_BACK);
		}

		if (_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A])
		{
			isPic = true;
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
		}
		else if (_imputInf._gTrgb[KEY_INPUT_X] || _imputInf._gTrgp[XINPUT_BUTTON_B])
		{
			_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
			PlaySoundMem(_modeServer->_valData.menuSoundHandle[3], DX_PLAYTYPE_BACK);
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
	DrawGraph(15, 110, wakuHandle, true);
	DrawExtendGraph(300, 110, 1260, 560, wakuHandle2, true);
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("装備選択", strlen("装備選択"));
	DrawString(20, 20, "装備選択", GetColor(255, 255, 255));

	SetFontSize(30);
	DrawString(20, 150, "攻撃技選択", GetColor(255, 255, 255));
	int defY = 200;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		std::string insStr = menuMessage[i] + "\n";
		if (i == 0) { insStr += _modeServer->_valData.plChangeAttackX; }
		if (i == 1) { insStr += _modeServer->_valData.plChangeAttackY; }
		DrawString(90, defY + (100 * i), insStr.c_str(), GetColor(255, 255, 255));

	}
	if (isPic)
	{
		DrawGraph(400 - 65, 200 + (pic * 50) + 7, arrowAnimHandle[arrowAnimNum], true);
	}
	else {
		DrawGraph(80 - 65, defY + (100 * picMenuNum) + 7, arrowAnimHandle[arrowAnimNum], true);
	}
	if (picMenuNum == 0 || picMenuNum == 1) {
		for (int i = 0; i < _modeServer->_valData.changeAttackList.size(); i++)
		{
			DrawString(400, 200 + (50 * i), _modeServer->_valData.changeAttackList[i].c_str(), GetColor(255, 255, 255));
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