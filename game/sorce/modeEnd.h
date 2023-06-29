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

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool Process();
	virtual bool Render();

protected:
	int _cg//!画像ハンドル
		, creditsY//!クレジットの再生位置
		, DeffontSize;//デフォルトのフォントサイズ保存
	ModeServer* _modeServer;
	imputInf _imputInf;
};
