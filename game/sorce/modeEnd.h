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

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;

protected:
	int _cg//!�摜�n���h��
		, creditsY//!�N���W�b�g�̍Đ��ʒu
		, DeffontSize;//�f�t�H���g�̃t�H���g�T�C�Y�ۑ�
	ModeServer* _modeServer;
	imputInf _imputInf;
};
