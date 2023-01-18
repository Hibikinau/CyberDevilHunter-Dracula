#pragma once
#include<memory>
#include<string>
#include<vector>
#include"../../../DxLib/DxLib.h"

#define Char_PL "Player"
#define Char_BOSS1 "Boss1"

//���탂�f���p
struct weponModelInf
{
	int weponHandle, weponAttachFrameNum;
	MATRIX weponFrameMatrix;
	bool isActive;
	std::string name;
};

//3d���f���p
struct modelInf
{
	int modelHandle, animHandleOld = -1, importCnt, attachIndex, attachIndexOld = -1;
	float playTime, playTimeOld, totalTime, rate;
	bool isBrending = false, animOldLoop, isAnimEnd;
	VECTOR pos = VGet(0, 0, 0), dir = VGet(0, 0, 0), vec = VGet(0, 0, 0);
	std::vector<weponModelInf> wepons;
};

struct valData
{
	float plAtkSpd1 = 24.f, plAtkSpd2 = 24.f, plAtkSpd3 = 24.f, plAtkSpd4 = 24.f;
	std::vector<int> activateWepon, disableWepon;
};

class modelManager
{
public:
	int a;
	//3d���f���̓ǂݍ���(3d���f���̃p�X, ���f���̊g�嗦, modelInf�̃|�C���^)
	bool modelImport(const char* dir, const float scale, modelInf* MI);
	//�Đ����Ă���A�j���[�V�����̕ύX(�A�j���[�V�����ԍ�, modelInf�̃|�C���^, ���[�v����A�j���[�V�����Ȃ̂�, �A�j���[�V�����u�����h���g����)
	bool animChange(int _animHandle, modelInf* MI, bool isLoop, bool isBlend);
	//3d���f���̕`��(modelInf�̃|�C���^, �A�j���[�V�����̃X�s�[�h)
	//�A�j���[�V�����̍Đ����I�������true��Ԃ�
	bool modelRender(modelInf* MI, float animSpeed);
	//3d���f���ɕ���̃A�^�b�`(���탂�f���̃p�X, �Ǐ]��������modelInf�̃|�C���^, �Ǐ]���������t���[���̖��O, ���탂�f���̊g�嗦, ���탂�f���̕`�ʂ�L���ɂ��邩, ����̖��O)
	bool weponAttach(const char* dir, modelInf* MI, const char* attachFrame, const float scale, bool activate, const char* name);
	//�L�������f���̃��������
	bool modelDelete(modelInf* MI);
};



//SetUseASyncLoadFlag(true);
//SetUseASyncLoadFlag(false);
//SetDrawScreen(DX_SCREEN_BACK);
//int i = 0;
//ProcessMessage();
//int B = GetASyncLoadNum();
//while (GetASyncLoadNum() > 0)
//{
//	ClearDrawScreen();
//	if (i < 20) { DrawString(640, 360, "loading.", GetColor(255, 255, 255)); }
//	else if (i < 40) { DrawString(640, 360, "loading..", GetColor(255, 255, 255)); }
//	else if (i < 60) { DrawString(640, 360, "loading...", GetColor(255, 255, 255)); }
//	else { i = 0; }
//	i++;
//	ScreenFlip();
//}