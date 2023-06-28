/*****************************************************************//**
 * \file   modeArtsGet.h
 * \brief  アーツ取得画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeArtsGet : public ModeBase
{
public:
	modeArtsGet(ModeServer* MS) { _modeServer = MS; };
	~modeArtsGet() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, _ui, menuBarHandle;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
};