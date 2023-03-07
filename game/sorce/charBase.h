#pragma once
#include"appframe.h"
#include"motionDefineList.h"

struct statusInf
{//�ő�l����
	float hitPoint, maxHitPoint, bloodPoint, maxBloodPoint, stamina, maxStamina;
};

struct collCapsule
{//�ő�l����
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
	bool isUseMat, isAlive = true;
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
	void setCB(std::map<std::string, std::shared_ptr<CB> >* _CB) { charBox = _CB; }
	void setRS(Rserver* _RS) { RS = _RS; }

	virtual int deadCheck(VECTOR PLpos) { return 0; }
	virtual bool CA_change(std::string name, const char* XorY) { return false; };
	virtual bool HPmath(float Num) { return false; };
	virtual bool BPmath(float Num) { return false; };
	virtual void setCamDir(float Num) { camDir = Num; };
	virtual void battleEndVoice() { return; }
	virtual void battleStartVoice() { return; }
	void setName(const char* _name) { name = _name; }
	bool hitCheck(const char* name, VECTOR* hitPos, float* damage);
	modelInf* getInf() { return &_modelInf; }
	void setGroundInf(modelInf* GE) { _GrounfInf = GE; }
	statusInf	getStatus() { return _statusInf; }
	void getInputKey(imputInf* iInf, float* cameraDir) { _imputInf = iInf, _cameraDir = cameraDir; };
	bool makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r, int nonActiveTimeF, int activeTimeF, bool isUseMat, float damage, int frameNum, const char* charName);
	void setMasterVolume(int masterValume) { for (int handle : soundHandle) { ChangeVolumeSoundMem(255 * (0.01 * _valData->soundMasterValume), handle); } }

	int type = 0;//pl=1, oEnemy=2
	bool isGround, isHit, isSetSoundValume = false;
	float g, camDir, * _cameraDir, animSpd;
	int isDead;//0.�����Ă� 1.���S���[�V������ 2.�C���X�^���X���
	int isImmortal = false, dodgeTime;
	std::map<std::string, std::shared_ptr<CB> >* charBox;
	std::vector<attackColl>* allColl;
	std::vector<int> soundHandle;
	std::string name, attackChar;
	modelInf _modelInf, * _GrounfInf;
	statusInf _statusInf;
	valData* _valData;
	MV1_COLL_RESULT_POLY hitCheckGround;
	MV1_COLL_RESULT_POLY_DIM hitCheckWall;
	collCapsule collCap;
	imputInf* _imputInf;
	Rserver* RS;
};