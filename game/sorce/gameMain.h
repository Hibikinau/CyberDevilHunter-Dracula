#pragma once
#include"../../AppFrame/source/appframe.h"
#include"player.h"
#include<memory>

#include"modeTitle.h"
#include"modeGame.h"

class gameMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool Initialize(HINSTANCE hInstance, ModeServer* ms);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 1280; }
	virtual int DispSizeH() { return 720; }
	ModeServer& getModeServer() { return *_modeServer; };

protected:

	int _x, _y;
	int LightHandle01, LightHandle02, shadowMapHandle, animIndexOld;
	float spd, cameraDir, cameraHigh;
	VECTOR cameraPos, cameraFor;
	modelInf plMI, stage;
	modelManager _modelManager;
	ModeServer* _modeServer;
	std::string NS;

	bool debugMode = true;
};
