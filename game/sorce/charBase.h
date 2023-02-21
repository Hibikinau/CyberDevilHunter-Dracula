#pragma once
#include"appframe.h"
#include"ResourceServerIns.h"
#include"motionDefineList.h"

struct statusInf
{//最大値を代入
	float hitPoint, maxHitPoint, bloodPoint, maxBloodPoint, stamina, maxStamina;
};

struct collCapsule
{//最大値を代入
	VECTOR underPos, overPos;
	float r = -1;
	int parentModelHandle, frameNum;
};

struct weaponEffect
{
	std::vector<VECTOR> upCornerPos, downCornerPos;
	float lifeTime = 10, maxLifeTime = 10, overrideTime, nonOverrideTime;
	int efcPicHandle;
};

struct attackColl
{
	collCapsule capColl, capCollOld;
	std::string attackChar;
	std::vector<collCapsule> capCollTs;
	MATRIX Mold;
	float damage;
	int nonActiveTimeF, activeTimeF;
	bool isUseMat, isAlive;
	weaponEffect rightingEfc;
}; 

class CB
{
public:
	CB() {};
	virtual ~CB() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Process();
	virtual bool	Render(float timeSpeed);
	//virtual bool	atkEfcRender(float timeSpeed);
	virtual bool gravity();
	virtual int	getType() { return type; }
	virtual void setCB(std::map<std::string, std::shared_ptr<CB> >* _CB) { charBox = _CB; }

	virtual int deadCheck(VECTOR PLpos) { return 0; }
	virtual bool CA_change(std::string name, const char* XorY) { return false; };
	virtual bool HPmath(float Num) { return false; };
	virtual bool BPmath(float Num) { return false; };
	virtual void setCamDir(float Num) { camDir = Num; };
	void setName(const char* _name) { name = _name; }
	bool hitCheck(const char* name);
	modelInf* getInf() { return &_modelInf; }
	void setGroundInf(modelInf* GE) { _GrounfInf = GE; }
	statusInf	getStatus() { return _statusInf; }
	void getInputKey(imputInf* iInf, float* cameraDir) { _imputInf = iInf, _cameraDir = cameraDir; };
	bool makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r, int nonActiveTimeF, int activeTimeF, bool isUseMat, float damage, int frameNum, const char* charName);

	int type = 0;//pl=1, oEnemy=2
	bool isGround, isHit;
	float g, camDir, * _cameraDir;
	int isDead;//0.生きてる 1.死亡モーション中 2.インスタンス解放
	int isImmortal = false, dodgeTime;
	std::string name, attackChar;
	modelManager* _modelManager;
	modelInf _modelInf, * _GrounfInf;
	statusInf _statusInf;
	std::map<std::string, std::shared_ptr<CB> >* charBox;
	valData* _valData;
	MV1_COLL_RESULT_POLY hitCheckGround;
	MV1_COLL_RESULT_POLY_DIM hitCheckWall;
	collCapsule collCap;
	std::vector<attackColl>* allColl;
	imputInf* _imputInf;
};