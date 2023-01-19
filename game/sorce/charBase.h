#pragma once
#include"../../AppFrame/source/appframe.h"
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

struct attackColl
{
	collCapsule capColl;
	std::string attackChar;
	float damage;
	int nonActiveTimeF, activeTimeF;
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
	virtual int	getType() { return type; }
	virtual void setCB(std::map<std::string, std::shared_ptr<CB> >* _CB) { charBox = _CB; }

	virtual int deadCheck(VECTOR PLpos) { return 0; }
	virtual void setCamDir(float Num) { camDir = Num; };
	modelInf* getInf() { return &_modelInf; }
	statusInf	getStatus() { return _statusInf; }

	int type = 0;//pl=1, oEnemy=2
	bool isGround;
	float g, camDir;
	int isDead;//0.�����Ă� 1.���S���[�V������ 2.�C���X�^���X���
	modelManager *_modelManager;
	modelInf _modelInf, *_GrounfInf;
	statusInf _statusInf;
	std::map<std::string, std::shared_ptr<CB> >* charBox;
	valData* _valData;
	MV1_COLL_RESULT_POLY hitCheckGround;
	MV1_COLL_RESULT_POLY_DIM hitCheckWall;
	collCapsule collCap;
	std::vector<attackColl> *allColl;
};