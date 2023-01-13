#pragma once
#include"../../AppFrame/source/appframe.h"
#include"modelManager.h"
#include"ResourceServerIns.h"

struct statusInf
{//�ő�l����
	float hitPoint, bloodPoint, vampireLevel, stamina;
};

struct collCapsule
{//�ő�l����
	VECTOR underPos, overPos;
	float r;
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
	virtual void setCB(std::map<std::string, std::shared_ptr<CB> >* _CB) { charBox = _CB; }
	virtual int deadCheck(VECTOR PLpos) { return 0; }
	virtual bool CA_change(std::string name, const char* XorY) { return false; };
	virtual bool HPmath(float Num) { return false; };
	virtual bool BPmath(float Num) { return false; };
	modelInf* getInf() { return &_modelInf; }
	statusInf	getStatus() { return _statusInf; }

	int type = 0;//pl=1, oEnemy=2
	bool isGround;
	float g;
	int isDead;//0.�����Ă� 1.���S���[�V������ 2.�C���X�^���X���
	modelManager _modelManager;
	modelInf _modelInf;
	statusInf _statusInf;
	std::map<std::string, std::shared_ptr<CB> >* charBox;
	valData* _valData;
};