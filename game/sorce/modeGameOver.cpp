#include"allMode.h"

bool	modeGO::Initialize()
{
	_modeServer->RS.loadDivGraphR("game/res/deadEnd/apngframe01_sheet.png", 80, 3, 27, 600, 240, gameOverAnimHandle);
	_modeServer->RS.loadDivGraphR("game/res/arrow/apngframe01_sheet.png", 16, 13, 2, 75, 25, arrowAnimHandle);
	GOAnimNum = 0;
	_cg = MakeGraph(1280, 720);
	GetDrawScreenGraph(0, 0, 1280, 720, _cg);
	_modeServer->disable(MODE_GAME);
	DeffontSize = GetFontSize();
	SetFontSize(40);
	menuMessage.emplace_back("戦闘をやり直す");
	menuMessage.emplace_back("ブリーフィングに戻る");
	menuMessage.emplace_back("タイトルに戻る");
	picMenuMaxNum = menuMessage.size() - 1;
	PlaySoundMem(_modeServer->_valData.menuSoundHandle[5], DX_PLAYTYPE_BACK);
	return true;
}

bool	modeGO::Process()
{
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

	if (_imputInf._gRelb[KEY_INPUT_RETURN] || _imputInf._gRelp[XINPUT_BUTTON_A])
	{
		if (picMenuNum == 0)
		{//戦闘のやり直し
			_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
		}
		else if (picMenuNum == 1)
		{//ブリーフィングへ
			_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
		}
		else if (picMenuNum == 2)
		{//タイトルへ
			_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
		}
		_modeServer->Del(MODE_GAME);
		PlaySoundMem(_modeServer->_valData.menuSoundHandle[1], DX_PLAYTYPE_BACK);
		return false;
	}
	return true;
}

bool	modeGO::Render()
{/*
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("GAME OVER", strlen("GAME OVER"));
	DrawString(640 - (insMenuFontSize / 2), 20, "GAME OVER", GetColor(255, 255, 255));
	*/
	//360
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawGraph(0, 0, _cg, false);
	//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	GOAnimNum < 79 ? GOAnimNum += 0.5f : GOAnimNum = 79 - rand() % 10;
	DrawExtendGraph(280, 60, 1000, 260, gameOverAnimHandle[static_cast<int>(GOAnimNum / 1)], true);

	int defY = 300;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(640, defY + (90 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));
		if (picMenuNum == i)
		{
			arrowAnimNum < 16 ? arrowAnimNum++ : arrowAnimNum = 0;
			DrawGraph(640 - 65, defY + (90 * i) + 7, arrowAnimHandle[arrowAnimNum], true);
			//DrawString(640 - 40, defY + (90 * i), "→", GetColor(255, 255, 255));
		}
	}

	return true;
}

bool	modeGO::Terminate()
{
	DeleteGraph(_cg);
	SetFontSize(DeffontSize);
	return true;
}