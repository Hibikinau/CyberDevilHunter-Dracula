#pragma once
#include"../../AppFrame/source/appframe.h"
//3d���f���p
struct modelInf
{
	int modelHandle, animHandleOld = -1, importCnt, attachIndex, attachIndexOld = -1;
	float playTime, totalTime, rate;
	bool isBrending = false, animOldLoop;
	VECTOR pos = VGet(0, 0, 0), dir = VGet(0, 0, 0), vec = VGet(0, 0, 0);
};

struct valData
{
	float plAtkSpd1 = 20.f, plAtkSpd2 = 20.f, plAtkSpd3 = 20.f, plAtkSpd4 = 20.f;
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
};
