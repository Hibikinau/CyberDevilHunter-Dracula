/*****************************************************************//**
 * \file   modeBossChoose.h
 * \brief  �{�X�I���̃N���X
 *
 * \author �e�Y�q��
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeBC : public ModeBase
{
public:
	modeBC(ModeServer* MS) { _modeServer = MS; };
	~modeBC() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, mapAnimNum = 0, mapAnimHandol[62] = { -1 }
	, glitchAnimNum, glitchAnimHandle[19] = { -1 }, randomNum = 0, randomFrameNum = 0, newsWindow = -1, newsWindowStr = -1
		, randomNewsNum, newsPosX, wakuHandle, wakuHandle2, titleHandle, maskData, checkUiHandle, bossUiHandle;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	bool isPic = false, pic = true;
};
