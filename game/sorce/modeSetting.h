/*****************************************************************//**
 * \file   modeSetting.h
 * \brief  �ݒ��ʂ̃N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeSetting : public ModeBase
{
public:
	modeSetting(ModeServer* MS) { _modeServer = MS; };
	~modeSetting() {};

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