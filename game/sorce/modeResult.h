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

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg, DeffontSize;
	ModeServer* _modeServer;
	imputInf _imputInf;
};

