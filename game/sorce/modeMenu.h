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
};
