#pragma once
#include"../../AppFrame/source/appframe.h"
//3dモデル用
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
	//3dモデルの読み込み(3dモデルのパス, モデルの拡大率, modelInfのポインタ)
	bool modelImport(const char* dir, const float scale, modelInf* MI);
	//再生しているアニメーションの変更(アニメーション番号, modelInfのポインタ, ループするアニメーションなのか, アニメーションブレンドを使うか)
	bool animChange(int _animHandle, modelInf* MI, bool isLoop, bool isBlend);
	//3dモデルの描画(modelInfのポインタ, アニメーションのスピード)
	//アニメーションの再生が終わったらtrueを返す
	bool modelRender(modelInf* MI, float animSpeed);
};
