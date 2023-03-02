#include"allMode.h"

bool	modeMM::Initialize()
{
	DeffontSize = GetFontSize();
	menuMessage.emplace_back("討伐ボス選択");
	menuMessage.emplace_back("装備変更");
	menuMessage.emplace_back("アーツ取得");
	menuMessage.emplace_back("設定");
	picMenuMaxNum = menuMessage.size() - 1;
	return true;
}

bool	modeMM::Process()
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
		{//討伐ボス選択
			_modeServer->Add(std::make_unique<modeBC>(_modeServer), 1, MODE_BC);
			return false;
		}
		else if (picMenuNum == 1)
		{//装備変更
			_modeServer->Add(std::make_unique<modeES>(_modeServer), 1, MODE_ES);
			return false;
		}
		else if (picMenuNum == 2)
		{//アーツ取得
		}
		else if (picMenuNum == 3)
		{//設定
		}
	}
	return true;
}

bool	modeMM::Render()
{
	SetFontSize(80);
	int insMenuFontSize = GetDrawStringWidth("BRIEFING", strlen("BRIEFING"));
	DrawString(20, 20, "BRIEFING", GetColor(255, 255, 255));
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

bool	modeMM::Terminate()
{
	SetFontSize(DeffontSize);
	return true;
}