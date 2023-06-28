/*****************************************************************//**
 * \file   modeGameOver.h
 * \brief  ゲームオーバー画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeGameOver : public ModeBase
{
public:
	modeGameOver(ModeServer* MS) { _modeServer = MS; };
	~modeGameOver() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	int gameOverAnimHandle[89] = { -1 };
	float GOAnimNum = 0;
	bool isGameEnd = false;
};
