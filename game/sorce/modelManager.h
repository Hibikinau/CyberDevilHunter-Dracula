#pragma once
#include"../../AppFrame/source/appframe.h"
//3dƒ‚ƒfƒ‹—p
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
	bool modelImport(const char* dir, const float scale, modelInf* MI);
	bool animChange(int _animHandle, modelInf* MI);
	bool modelRender(modelInf* MI);
};
