#pragma once
#include"../../AppFrame/source/appframe.h"
//3d���f���p
struct modelInf
{
	int modelHandle, animHandleOld = -1, importCnt, attachIndex, attachIndexOld = -1;
	float playTime, totalTime, rate;
	bool isBrending = false;
	VECTOR pos = VGet(0, 0, 0), dir = VGet(0, 0, 0), vec = VGet(0, 0, 0);
};

struct valData
{
	float plAtkSpd = 0.5f;
};

class modelManager
{
public:
	int a;
	//3d���f���̓ǂݍ���(3d���f���̃p�X, ���f���̊g�嗦, modelInf�̃|�C���^)
	bool modelImport(const char* dir, const float scale, modelInf* MI);
	//�Đ����Ă���A�j���[�V�����̕ύX(�A�j���[�V�����ԍ�, modelInf�̃|�C���^)
	bool animChange(int _animHandle, modelInf* MI);
	//3d���f���̕`��(modelInf�̃|�C���^)
	bool modelRender(modelInf* MI);
};
