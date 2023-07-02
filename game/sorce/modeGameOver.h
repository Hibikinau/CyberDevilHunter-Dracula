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

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	int gameOverAnimHandle[89] = { -1 };
	float GOAnimNum = 0;
	bool isGameEnd = false;
};
