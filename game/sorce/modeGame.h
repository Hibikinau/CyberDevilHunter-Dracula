/*****************************************************************//**
 * \file   modeGameame.h
 * \brief  ゲーム画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"
#include"player.h"
#include"bossKnight.h"
#include"bossLionman.h"
#include"Lastboss.h"
#include<memory>

struct popDamageInf
{
	VECTOR pos;
	float damage;
	int popTime = 0;
	bool isPl;
};

class modeGame : public ModeBase
{
public:
	modeGame(ModeServer* MS) { _modeServer = MS; }
	~modeGame() {};

	bool Initialize() override;
	bool Terminate() override;
	bool Imput(imputInf* iInf) override { _imputInf = *iInf; return true; };
	bool Process() override;
	bool Render() override;
	void cameraMove();
	int useCommand();
	bool ASyncLoadAnim();
	bool collHitCheck();
	bool drawUI();

public:
	ModeServer* _modeServer;
	int _x, _y;
	int testAttackF, HPgaugeHandle, HPgaugeHandleWaku, BPgaugeHandle, BPgaugeHandleWaku, HPstrHandle
		, BPstrHandle, stunGaugeHandleWaku, stunGaugeHandle, stunGaugeHandle2, stunGaugeHandle3
		, shadowMapHandle, animIndexOld, redTrailHamdle, whiteTrailHamdle, plRecastTimeX, plRecastTimeY, swordIcon, heatIcon
		, swordGlitchAnimHandle[29], swordGlitchAnimNum, stunStrHandle, bossNameStrHandle, bossNamePosX
		, recastMaxNumX, recastMaxNumY, plSetRecastTime, swordRecastIconHandle, sousaHandle, burstStrHandle, nearDeadHandle
		, HPgaugeHandle2, HPgaugeHandle3, HPgaugeHandleWakuB;
	float spd, cameraDir, cameraLockDir, cameraNtDir, cameraHigh;
	int startTime, countTime, FPS, FPScount;
	bool isLockon, isGameOver = false, endVoice = false;
	std::map<std::string, std::shared_ptr<CharBase> > charBox;
	VECTOR cameraPos, cameraFor;
	modelInf* plMI, stage, * bossMI, skyDoom, sakuHandle;
	std::string NS;
	std::vector<std::string> debugWardBox;
	imputInf _imputInf;
	valData* _valData;
	std::vector<attackColl> mAllColl;
	collCapsule testAttackCap;
	statusInf plStatus, bossStatus;
	std::vector<weaponEffect> atkEfc;
	std::vector<popDamageInf> damageNumPopList;
	int UIkari, BGM, insEfcHandle, efcHandle, efcTime;
	int keepout[180] = { -1 }, keepoutNum;
	int lockOnMarkerHandle[30] = { -1 }, LOMarkerNum = 0;
	int gameStartAnimHandle[89] = { -1 }, GSAnimNum;
	int slashLineAnimHandle[39] = { -1 };
	int loadingAnimHandle[27] = { -1 }, loadingAnimNum;
	int StageModelHandle, CharaModelHandle, ShadowMapHandle, BGMdelay;
	float bright = .6f, Atten0 = 0.8f, Atten1 = 0.f, Atten2 = 0.f;
	bool debugMode = false, plDead;
};