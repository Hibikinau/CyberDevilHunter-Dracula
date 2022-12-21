#pragma once
#include"../../AppFrame/source/appframe.h"
#include"modelManager.h"
#include"ResourceServerIns.h"

struct statusInf
{//ç≈ëÂílÇë„ì¸
	float hitPoint, bloodPoint, stamina;
};

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
	virtual void setCB(std::map<std::string, std::shared_ptr<CB> > *_CB) { charBox = _CB; }
	virtual int deadCheck(VECTOR PLpos) { return 0; }
	modelInf* getInf() { return &_modelInf; }
	statusInf	getStatus() { return _statusInf; }

	int type;//pl=1, oEnemy=2
	bool isGround;
	float g;
	modelManager _modelManager;
	modelInf _modelInf;
	statusInf _statusInf;
	std::map<std::string, std::shared_ptr<CB> > *charBox;
	valData *_valData;
};