/*****************************************************************//**
 * \file   modeResult.h
 * \brief  リザルト画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeResult : public ModeBase
{
public:
	modeResult(ModeServer* MS) { _modeServer = MS; };
	~modeResult() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize;
	ModeServer* _modeServer;
	imputInf _imputInf;
};

