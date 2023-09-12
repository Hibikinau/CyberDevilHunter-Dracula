/*****************************************************************//**
 * \file   modelManager.h
 * \brief  ���f������N���X
 *
 * \author raide
 * \date   March 2023
 *********************************************************************/

#pragma once
#include<memory>
#include<string>
#include<vector>
#include<map>
#include<DxLib.h>
#include "../AppFrame/source/ResourceServer/ResourceServer.h"

 //���탂�f���p
struct weponModelInf
{
	int weponHandle, weponAttachFrameNum;
	MATRIX weponFrameMatrix;
	bool isActive;
	float scale;
	std::string name;
};

//3d���f���p
struct modelInf
{
	int modelHandle, animHandleOld = -1, attachIndex, attachIndexOld = -1, animHandleNext = -1;
	float playTime, playTimeOld, totalTime, rate, scale, animSpdBuff = 0;
	bool isBrending, isBrendingNext, animOldLoop, animLoopNext, isAnimEnd;
	VECTOR pos = VGet(0, 0, 0), addPos = VGet(0, 0, 0), dir = VGet(0, 0, 0), vec = VGet(0, 0, 0);
	std::vector<std::shared_ptr<weponModelInf> > wepons;
};

namespace model
{
	//3d���f���̓ǂݍ���(3d���f���̃p�X, ���f���̊g�嗦, modelInf�̃|�C���^, Rserver�C���X�^���X�̃|�C���^)
	bool modelImport(const char* dir, const float scale, modelInf* MI, Rserver* _Rserver);
	//�Đ����Ă���A�j���[�V�����̕ύX(�A�j���[�V�����ԍ�, modelInf�̃|�C���^, ���[�v����A�j���[�V�����Ȃ̂�, �A�j���[�V�����u�����h���g����)
	bool animChange(int _animHandle, modelInf* MI, bool isLoop, bool isBlend, bool isOverride);
	//���ɍĐ�����A�j���[�V�����̐ݒ�(�A�j���[�V�����ԍ�, modelInf�̃|�C���^, ���[�v����A�j���[�V�����Ȃ̂�, �A�j���[�V�����u�����h���g����)
	bool setNextAnim(int _animHandle, modelInf* MI, bool isLoop, bool isBlend);
	//3d���f���̕`��(modelInf�̃|�C���^, �A�j���[�V�����̃X�s�[�h, �Đ����x)
	//�A�j���[�V�����̍Đ����I�������true��Ԃ�
	bool modelRender(modelInf* MI, float animSpeed, float timeSpead);
	//3d���f���ɕ���̃A�^�b�`(���탂�f���̃p�X, �Ǐ]��������modelInf�̃|�C���^, �Ǐ]���������t���[���̔ԍ�, ���탂�f���̊g�嗦, ���탂�f���̕`�ʂ�L���ɂ��邩, ����̖��O, Rserver�C���X�^���X�̃|�C���^)
	bool weponAttach(const char* dir, modelInf* MI, int attachFrameNum, const float scale, bool activate, const char* name, Rserver* _Rserver);
	//�L�������f���̃��������
	bool modelDelete(modelInf* MI);
	//���[�h����3d���f���̃T�C�Y�ύX
	bool changeScale(modelInf* MI);
	//�|���S���̕`��(�������W�A������W�A�E�����W�A�E����W�A�`�悷��摜�n���h��)
	bool drawBPolygon(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, int grHandle);
	//3D��Ԃɔ�`�悷��(�������W�A������W�A�E�����W�A�E����W�A���̐F�A���˂̐F)
	bool drawBord(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, DxLib::COLOR_U8 difColor, DxLib::COLOR_U8 spcColor);
	//3D��Ԃɗ����̂�`�悷��(���S���W�A�T�C�Y�A���̐F�A���˂̐F)
	bool drawCube(VECTOR pos, VECTOR siz, DxLib::COLOR_U8 difColor, DxLib::COLOR_U8 spcColor);
};