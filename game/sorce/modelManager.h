#pragma once
#include"../../AppFrame/source/appframe.h"
//武器モデル用
struct weponModelInf
{
	int weponHandle, weponAttachFrameNum;
	MATRIX weponFrameMatrix;
	bool isActive;
};

//3dモデル用
struct modelInf
{
	int modelHandle, animHandleOld = -1, importCnt, attachIndex, attachIndexOld = -1;
	float playTime, totalTime, rate;
	bool isBrending = false, animOldLoop;
	VECTOR pos = VGet(0, 0, 0), dir = VGet(0, 0, 0), vec = VGet(0, 0, 0);
	std::vector<weponModelInf> wepons;
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
	//3dモデルに武器のアタッチ(武器モデルのパス, 追従させたいmodelInfのポインタ, 追従させたいフレームの名前, 武器モデルの拡大率, 武器モデルの描写を有効にするか)
	bool weponAttach(const char* dir, modelInf* MI, const char* attachFrame, const float scale, bool activate);

};
