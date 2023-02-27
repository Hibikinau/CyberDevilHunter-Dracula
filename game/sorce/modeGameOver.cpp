#include"allMode.h"

bool	modeGO::Initialize()
{
	DeffontSize = GetFontSize();
	menuMessage.emplace_back("戦闘をやり直す");
	menuMessage.emplace_back("ブリーフィングに戻る");
	menuMessage.emplace_back("タイトルに戻る");
	picMenuMaxNum = menuMessage.size() - 1;
	return true;
}

bool	modeGO::Process()
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		_modeServer->activate(MODE_GAME);
		return false;
	}
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
		{//戦闘のやり直し
			_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
			return false;
		}
		else if (picMenuNum == 1)
		{//ブリーフィングへ

		}
		else if (picMenuNum == 2)
		{//タイトルへ
			_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
			return false;
		}
	}
	return true;
}

bool	modeGO::Render()
{
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("GAME OVER", strlen("GAME OVER"));
	DrawString(640 - (insMenuFontSize / 2), 20, "GAME OVER", GetColor(255, 255, 255));
	SetFontSize(40);
	int defY = 300;
	for (int i = 0; i < menuMessage.size(); i++)
	{
		StrWidth = GetDrawStringWidth(menuMessage[i].c_str(), strlen(menuMessage[i].c_str()));
		DrawString(640 - StrWidth, defY + (90 * i), menuMessage[i].c_str(), GetColor(255, 255, 255));
		if (picMenuNum == i)
		{
			DrawString(640 - StrWidth - 40, defY + (90 * i), "→", GetColor(255, 255, 255));
		}
	}

	return true;
}

bool	modeGO::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}