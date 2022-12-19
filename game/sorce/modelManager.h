#pragma once
#include"../../AppFrame/source/appframe.h"
//3dモデル用
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
	//3dモデルの読み込み(3dモデルのパス, モデルの拡大率, modelInfのポインタ)
	bool modelImport(const char* dir, const float scale, modelInf* MI);
	//再生しているアニメーションの変更(アニメーション番号, modelInfのポインタ)
	bool animChange(int _animHandle, modelInf* MI);
	//3dモデルの描画(modelInfのポインタ)
	bool modelRender(modelInf* MI);
};
