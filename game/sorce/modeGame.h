#pragma once
#include"appframe.h"
#include"player.h"
#include"bossTentative.h"
#include<memory>

class modeG : public ModeBase
{
public:
	modeG(ModeServer* MS) { _modeServer = MS; }
	~modeG(){};

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

public:
	ModeServer* _modeServer;
	int _x, _y;
	int testAttackF, lockOnMarkerHandle, HPgaugeHandle, HPgaugeHandle2, BPgaugeHandle;
	int LightHandle01, LightHandle02, shadowMapHandle, animIndexOld;
	float spd, cameraDir, cameraLockDir, cameraNtDir, cameraHigh;
	int startTime, countTime, FPS, FPScount;
	bool isLockon;
	std::map<std::string, std::shared_ptr<CB> > charBox;
	VECTOR cameraPos, cameraFor;
	modelInf *plMI, stage, *bossMI;
	modelManager _modelManager;
	std::string NS;
	std::vector<std::string> debugWardBox;
	imputInf _imputInf;
	valData _valData;
	std::vector<attackColl> mAllColl;
	collCapsule testAttackCap;
	statusInf plStatus, bossStatus;
	int UIkari, BGM, insEfcHandle, efcHandle, efcTime;
	int keepout[180] = { -1 }, keepoutNum;

	int StageModelHandle;
	int CharaModelHandle;
	int ShadowMapHandle;
	float bright = .6f;
	float Atten0 = 0.8f, Atten1 = 0.f, Atten2 = 0.f;
	bool debugMode = true, plDead;
	int BGMdelay;
	int drawAreaSize = 0;
};