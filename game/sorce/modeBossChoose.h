/*****************************************************************//**
 * \file   modeBossChoose.h
 * \brief  É{ÉXëIëÇÃÉNÉâÉX
 *
 * \author òeìYïqùÙ
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeBossChoose : public ModeBase
{
public:
	modeBossChoose(ModeServer* MS) { _modeServer = MS; };
	~modeBossChoose() {};

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, mapAnimNum = 0, mapAnimHandol[62] = { -1 }
		, glitchAnimNum, glitchAnimHandle[19] = { -1 }, randomNum = 0, randomFrameNum = 0, newsWindow = -1, newsWindowStr = -1
		, randomNewsNum, newsPosX, wakuHandle, wakuHandle2, titleHandle, maskData, checkUiHandle, bossUiHandle, menuBarHandle
		, Boss1HoloAnimHandle[7], Boss2HoloAnimHandle[7], memoAnimHandle[29], memoAnimNum, barANimHandle[22], barAimNum
		, othersAnimHandle[30], othersAnimNum, bArrowAnimHandle[18], bArrowAnimNum, Icon2AnimHandle[30], Icon2AnimNum;
	float Boss1HoloAnimNum, Boss2HoloAnimNum;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	bool isPic = false, pic = true;
};
