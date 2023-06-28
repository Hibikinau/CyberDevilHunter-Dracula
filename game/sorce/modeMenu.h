/*****************************************************************//**
 * \file   modeMenuenu.h
 * \brief  ���j���[��ʂ̃N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeMenu : public ModeBase
{
public:
	modeMenu(ModeServer* MS) { _modeServer = MS; };
	~modeMenu() {};

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
};
