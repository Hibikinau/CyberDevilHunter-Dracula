/*****************************************************************//**
 * \file   modeGame.h
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
#include<memory>

struct popDamageInf
{
	VECTOR pos;
	float damage;
	int popTime = 0;
};

class modeG : public ModeBase
{
public:
	modeG(ModeServer* MS) { _modeServer = MS; }
	~modeG() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();
	void cameraMove();
	int useCommand();
	bool ASyncLoadAnim();
	bool collHitCheck();
	bool drawUI();
	bool	popBoss(int bossType, const char* _nameA)override;

public:
	ModeServer* _modeServer;
	int _x, _y;
	int testAttackF, HPgaugeHandle, HPgaugeHandleWaku, BPgaugeHandle, BPgaugeHandleWaku, HPstrHandle, BPstrHandle, stunGaugeHandleWaku, stunGaugeHandle
		, shadowMapHandle, animIndexOld, insEfcHamdle, plRecastTimeX, plRecastTimeY, swordIcon, heatIcon, swordGlitchAnimHandle[29], swordGlitchAnimNum
		, recastMaxNumX, recastMaxNumY, plSetRecastTime, swordRecastIconHandle;
	float spd, cameraDir, cameraLockDir, cameraNtDir, cameraHigh;
	int startTime, countTime, FPS, FPScount;
	bool isLockon, isGameOver = false, endVoice = false;
	std::map<std::string, std::shared_ptr<CB> > charBox;
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

	int StageModelHandle;
	int CharaModelHandle;
	int ShadowMapHandle;
	float bright = .6f;
	float Atten0 = 0.8f, Atten1 = 0.f, Atten2 = 0.f;
	bool debugMode = false, plDead;
	int BGMdelay;
	int drawAreaSize = 0;
};