#include"allMode.h"

bool	modeM::Initialize()
{
	DeffontSize = GetFontSize();
	SetFontSize(40);
	_cg = MakeGraph(1280, 720);
	GetDrawScreenGraph(0, 0, 1280, 720, _cg);
	_modeServer->disable(MODE_GAME);
	menuMessage.emplace_back("戦闘をやり直す");
	menuMessage.emplace_back("ブリーフィングに戻る");
	menuMessage.emplace_back("設定");
	menuMessage.emplace_back("タイトルに戻る");
	picMenuMaxNum = menuMessage.size() - 1;
	return true;
}

bool	modeM::Process()
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		_modeServer->activate(MODE_GAME);
		return false;
	}
	if (_imputInf._gTrgb[KEY_INPUT_DOWN])
	{
		picMenuNum == picMenuMaxNum ? picMenuNum = 0: picMenuNum++;
	}
	if (_imputInf._gTrgb[KEY_INPUT_UP])
	{
		picMenuNum == 0 ? picMenuNum = picMenuMaxNum : picMenuNum--;
	}
	return true;
}

bool	modeM::Render()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawGraph(0, 0, _cg, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	int insMenuFontSize = GetDrawStringWidth("MENU", strlen("MENU"));
	DrawString(640 - (insMenuFontSize / 2), 20, "MENU", GetColor(255, 255, 255));
	int defY = 300;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(1280 - StrWidth, defY + (90 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));
		if (picMenuNum == i)
		{
			DrawString(1280 - StrWidth - 40, defY + (90 * i), "→", GetColor(255, 255, 255));
		}
	}

	return true;
}

bool	modeM::Terminate()
{
	DeleteGraph(_cg);
	SetFontSize(DeffontSize);
	return true;
}