/*****************************************************************//**
 * \file   modeArtsGet.h
 * \brief  アーツ取得画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeArtsGet : public ModeBase
{
public:
	modeArtsGet(ModeServer* MS) { _modeServer = MS; };
	~modeArtsGet() {};

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, _ui, menuBarHandle;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
};