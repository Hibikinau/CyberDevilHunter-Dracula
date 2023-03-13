/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  ���[�h�̊��N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#pragma once
#include	<string>
#include "../AppFrame/source/ResourceServer/ResourceServer.h"

class ModeServer;

struct imputInf
{
	int		_gKeyp[16], _gTrgp[16], _gRelp[16], rStickX, rStickY, lStickX, lStickY, rTriggerX, lTriggerX;
	char _gKeyb[256], _gTrgb[256], _gRelb[256], wardBox[142];
};

class		ModeBase
{
public:
	ModeBase();
	virtual ~ModeBase();

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf);
	virtual bool	Process();
	virtual bool	Render();
	virtual bool	popBoss(int bossType, const char* _nameA) { return false; }

public:
	int	GetModeCount() { return _cntMode; }			// ���̃��[�h���n�܂��Ă���̃J�E���^
	unsigned long GetModeTm() { return _tmMode; }	// ���̃��[�h���n�܂��Ă���̎���ms
	unsigned long GetStepTm() { return _tmStep; }	// �O�t���[������̌o�ߎ���ms

	void SetCallPerFrame(int frame) { _callPerFrame = _callPerFrame_cnt = frame; }	// ���t���[����1��Process()���ĂԂ�(def:1)
	void SetCallOfCount(int count) { _callOfCount = count; }		// 1��̌Ăяo���ɉ���Process()���ĂԂ�(def:1)
	int GetCallPerFrame() { return _callPerFrame; }
	int GetCallOfCount() { return _callOfCount; }

private:
	friend	ModeServer;
	// ModeServer�p
	std::string		_szName;
	std::string		nextStage;
	imputInf _imputInf;
	int				_uid;
	int				_layer;

	void StepTime(unsigned long tmNow);
	void StepCount();
public:
	// ���[�h�������p
	int		_cntMode;	// �{���[�h�ɓ����Ă���̃J�E���^, 0�X�^�[�g
	unsigned long	_tmMode;	// �{���[�h�ɓ����Ă���̎��ԁBms
	unsigned long	_tmStep;	// �O�t���[������̌o�ߎ��ԁBms
	unsigned long	_tmModeBase;	// ���̃��[�h���n�܂������ԁBms
	unsigned long	_tmPauseBase;	// �|�[�Y���̃x�[�X���ԁBms
	unsigned long	_tmPauseStep;	// �|�[�Y���̐ώZ���ԁBms ���쒆�A���̒l�� _tmMode �ɗp����
	unsigned long	_tmOldFrame;	// �O�t���[���̎��ԁBms

	// CallPerFrame / CallOfCount�p
	int		_callPerFrame, _callPerFrame_cnt;
	int		_callOfCount;
	int arrowAnimHandle[16] = { -1 }, arrowAnimNum = 0;
	int backAnimHandle[90] = { -1 }, backAnimNum = 0;
};


