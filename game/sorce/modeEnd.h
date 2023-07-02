/*****************************************************************//**
 * \file   modeEnd.h
 * \brief  クレジット描画用のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"

class modeEnd : public ModeBase
{
public:
	modeEnd(ModeServer* MS) { _modeServer = MS; };
	~modeEnd() {};

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg//!画像ハンドル
		, creditsY//!クレジットの再生位置
		, DeffontSize;//デフォルトのフォントサイズ保存
	ModeServer* _modeServer;
	imputInf _imputInf;
};
