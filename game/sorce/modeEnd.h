/*****************************************************************//**
 * \file   modeEnd.h
 * \brief  �N���W�b�g�`��p�̃N���X
 *
 * \author �R�{介v
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
	int _cg//!�摜�n���h��
		, creditsY//!�N���W�b�g�̍Đ��ʒu
		, DeffontSize;//�f�t�H���g�̃t�H���g�T�C�Y�ۑ�
	ModeServer* _modeServer;
	imputInf _imputInf;
};
