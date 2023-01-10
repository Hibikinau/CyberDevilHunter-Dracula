#pragma once
#include"../../AppFrame/source/appframe.h"
#include"modelManager.h"
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
	bool getPcInf();
	bool makePL();


protected:
	ModeServer* _modeServer;
	int _x, _y;
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

	bool debugMode = true, plDead;
};