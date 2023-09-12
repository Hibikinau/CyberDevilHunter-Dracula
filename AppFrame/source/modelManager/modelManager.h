/*****************************************************************//**
 * \file   modelManager.h
 * \brief  モデル制御クラス
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

 //武器モデル用
struct weponModelInf
{
	int weponHandle, weponAttachFrameNum;
	MATRIX weponFrameMatrix;
	bool isActive;
	float scale;
	std::string name;
};

//3dモデル用
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
	//3dモデルの読み込み(3dモデルのパス, モデルの拡大率, modelInfのポインタ, Rserverインスタンスのポインタ)
	bool modelImport(const char* dir, const float scale, modelInf* MI, Rserver* _Rserver);
	//再生しているアニメーションの変更(アニメーション番号, modelInfのポインタ, ループするアニメーションなのか, アニメーションブレンドを使うか)
	bool animChange(int _animHandle, modelInf* MI, bool isLoop, bool isBlend, bool isOverride);
	//次に再生するアニメーションの設定(アニメーション番号, modelInfのポインタ, ループするアニメーションなのか, アニメーションブレンドを使うか)
	bool setNextAnim(int _animHandle, modelInf* MI, bool isLoop, bool isBlend);
	//3dモデルの描画(modelInfのポインタ, アニメーションのスピード, 再生速度)
	//アニメーションの再生が終わったらtrueを返す
	bool modelRender(modelInf* MI, float animSpeed, float timeSpead);
	//3dモデルに武器のアタッチ(武器モデルのパス, 追従させたいmodelInfのポインタ, 追従させたいフレームの番号, 武器モデルの拡大率, 武器モデルの描写を有効にするか, 武器の名前, Rserverインスタンスのポインタ)
	bool weponAttach(const char* dir, modelInf* MI, int attachFrameNum, const float scale, bool activate, const char* name, Rserver* _Rserver);
	//キャラモデルのメモリ解放
	bool modelDelete(modelInf* MI);
	//ロードした3dモデルのサイズ変更
	bool changeScale(modelInf* MI);
	//板ポリゴンの描画(左下座標、左上座標、右下座標、右上座標、描画する画像ハンドル)
	bool drawBPolygon(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, int grHandle);
	//3D空間に板を描画する(左下座標、左上座標、右下座標、右上座標、元の色、反射の色)
	bool drawBord(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, DxLib::COLOR_U8 difColor, DxLib::COLOR_U8 spcColor);
	//3D空間に立方体を描画する(中心座標、サイズ、元の色、反射の色)
	bool drawCube(VECTOR pos, VECTOR siz, DxLib::COLOR_U8 difColor, DxLib::COLOR_U8 spcColor);
};