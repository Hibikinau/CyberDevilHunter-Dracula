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

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, windowAnimHandle[29] = { -1 }, windowAnimNum = 0
		, menuBarHandle = -1, obiHandle;
	bool isBackTitle = false, isBackTitlePic = true;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
};
