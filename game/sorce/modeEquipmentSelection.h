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

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum = 0, picMenuMaxNum, pic = 0, popSelectNum = 0, wakuHandle, wakuHandle2, _ui, menuBarHandle;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	bool isPic = false;
};
