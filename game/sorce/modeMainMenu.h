/*****************************************************************//**
 * \file   modeMainMenu.h
 * \brief  メインメニュー画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeMainMenu : public ModeBase
{
public:
	modeMainMenu(ModeServer* MS) { _modeServer = MS; };
	~modeMainMenu() {};

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, windowAnimHandle[29] = { -1 }, windowAnimNum = 0
		, menuBarHandle = -1, obiHandle;
	bool isBackTitle = false, isBackTitlePic = true;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
};
