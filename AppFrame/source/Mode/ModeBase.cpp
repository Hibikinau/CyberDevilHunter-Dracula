/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  ���[�h�̊��N���X�̒�`
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#include "ModeBase.h"

ModeBase::ModeBase() {
	_szName = "";
	_uid = 1;
	_layer = 0;

	_cntMode = 0;
	_tmMode = 0;
	_tmStep = 0;
	_tmModeBase = 0;
	_tmPauseBase = 0;
	_tmPauseStep = 0;
	_tmOldFrame = 0;

	SetCallPerFrame(1);
	SetCallOfCount(1);
}

ModeBase::~ModeBase() {
}


// ModeServer�ɐڑ����AProcess()�̑O�Ɉ�x�����Ă΂��
bool	ModeBase::Initialize() {

	return true;
}

// ModeServer����폜�����ہA��x�����Ă΂��
bool	ModeBase::Terminate() {

	return true;
}


// --------------------------------------------------------------------------
/// @brief ���t���[���Ă΂��B���������L�q
// --------------------------------------------------------------------------
bool	ModeBase::Imput(imputInf* iInf)
{
	_imputInf = *iInf;
	return true;
}

// --------------------------------------------------------------------------
/// @brief ���t���[���Ă΂��B���������L�q
// --------------------------------------------------------------------------
bool	ModeBase::Process()
{
	return	true;
}

// --------------------------------------------------------------------------
/// @brief ���t���[���Ă΂��B�`�敔���L�q
// --------------------------------------------------------------------------
bool	ModeBase::Render()
{
	return	true;
}

// ���Ԍo�߂�������
void ModeBase::StepTime(unsigned long tmNow) {
	// ���Ԍo�ߏ���
	if (_cntMode == 0) {
		_tmMode = 0;
		_tmStep = 0;
		_tmModeBase = tmNow;
		_tmPauseBase = 0;
		_tmPauseStep = 0;
	}
	else {
		_tmMode = tmNow - _tmModeBase + _tmPauseStep;
		_tmStep = tmNow - _tmOldFrame;
	}
	_tmOldFrame = tmNow;
}

// �J�E���g��i�߂�
void ModeBase::StepCount() {
	_cntMode++;
}
