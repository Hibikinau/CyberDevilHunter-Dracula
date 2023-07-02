#pragma once
#include"appframe.h"
#include"player.h"
#include<memory>

#include"modeTitle.h"
#include"modeGame.h"

class gameMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	bool Initialize(HINSTANCE hInstance, ModeServer* ms) override;
	bool Terminate() override;
	bool Input() override;
	bool Process() override;
	bool Render() override;

	bool AppWindowed() override { return true; }
	int DispSizeW() override { return 1280; }
	int DispSizeH() override { return 720; }
	ModeServer& getModeServer() { return *_modeServer; };

protected:

	int _x, _y;
	int LightHandle01, LightHandle02, shadowMapHandle, animIndexOld;
	float spd, cameraDir, cameraHigh;
	VECTOR cameraPos, cameraFor;
	modelInf plMI, stage;
	ModeServer* _modeServer;
	std::string NS;

	bool debugMode = true;
};
