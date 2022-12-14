#pragma once
#include"../../AppFrame/source/appframe.h"
#include"modelManager.h"
#include"ResourceServerIns.h"

class CB
{
public:
	CB() {};
	virtual ~CB() {};


	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Process();
	virtual bool	Render();
	virtual bool gravity();
	virtual int	getType() { return type; }
	virtual int deadCheck(VECTOR PLpos) { return 0; }
	modelInf getInf() { return _modelInf; }

	int type;//pl=1, oEnemy=2
	bool isGround;
	float g;
	modelManager _modelManager;
	modelInf _modelInf;
};