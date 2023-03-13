/*****************************************************************//**
 * \file   modeResult.h
 * \brief  ���U���g��ʂ̃N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeR : public ModeBase
{
public:
	modeR(ModeServer* MS) { _modeServer = MS; };
	~modeR() {};

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

