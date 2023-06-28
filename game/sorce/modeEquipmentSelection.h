/*****************************************************************//**
 * \file   modeEquipmentSelection.h
 * \brief  �����I����ʂ̃N���X
 *
 * \author �e�Y�q��
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeEquipmentSelection : public ModeBase
{
public:
	modeEquipmentSelection(ModeServer* MS) { _modeServer = MS; };
	~modeEquipmentSelection() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum = 0, picMenuMaxNum, pic = 0, popSelectNum = 0, wakuHandle, wakuHandle2, _ui, menuBarHandle;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	bool isPic = false;
};
