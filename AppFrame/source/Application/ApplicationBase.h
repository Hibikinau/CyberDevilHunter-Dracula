/*****************************************************************//**
 * \file   ApplicationBase.h
 * \brief  �A�v���P�[�V�����̃X�[�p�[�N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <memory>
#include <string>
#include "../Mode//ModeServer.h"

class ApplicationBase
{
public:
	ApplicationBase();
	virtual ~ApplicationBase();

	virtual bool Initialize(HINSTANCE hInstance, ModeServer *ms);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 640; }
	virtual int DispSizeH() { return 480; }

	static	ApplicationBase	*GetInstance() { return _lpInstance; }
	virtual void GetLstick(int* xbuf, int* ybuf) { GetJoypadAnalogInput(xbuf, ybuf, DX_INPUT_PAD1); }
	virtual void GetRstick(int *xbuf, int *ybuf) { GetJoypadAnalogInputRight(xbuf, ybuf, DX_INPUT_PAD1); }
	virtual int GetWard() { return KeyInputString(10, 700, 141, _imputInf.wardBox, true); }//�߂�l1.���� 2.�L�����Z�� 3.���s

protected:
	static	ApplicationBase* _lpInstance;
	imputInf _imputInf;
	std::vector<std::string> debugWardBox;
	ModeServer* _serverMode;
	XINPUT_STATE imputInfX;

};