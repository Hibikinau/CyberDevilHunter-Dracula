#pragma once
#include"charBase.h"

////3dモデル用
//struct modelInf
//{
//	int modelHandle, importCnt, attachIndex;
//	float playTime, totalTime;
//	VECTOR pos = VGet(0, 0, 0), dir = VGet(0, 0, 0), vec = VGet(0, 0, 0);
//};

enum class ExclusiveState
{
	NORMAL,
	JUMP,
	quickATTACK,
	slowATTACK,
	changeATTACKX,
	changeATTACKY,
	DODGE,
	GUARD,
};

enum class pushButton
{
	Neutral,
	A,
	B,
	X,
	Y,
	LBX,
	LBY,
	Lstick,
	Irregular,
	R1,
};

class PL : public CB
{
public:
	PL() {};
	~PL() {};
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render(float timeSpeed);
	virtual void charMove(float Speed, float _Dir, bool animChange);
	pushButton setAction();
	bool checkTrgImput(int Key, int Pad) { if (_imputInf->_gTrgb[Key] || _imputInf->_gTrgp[Pad]) { return true; } else { return false; } }
	bool checkKeyImput(int Key, int Pad) { if (_imputInf->_gKeyb[Key] || _imputInf->_gKeyp[Pad]) { return true; } else { return false; } }
	float getMoveDir(bool checkUseCamDir);
	bool HPmath(float math) override;
	bool BPmath(float math) override;
	bool CA_change(std::string name, const char* XorY) override;
	static bool CA_charge(PL* insPL);
	static bool CA_debugAttack(PL* insPL);
	bool (*changeAttackX)(PL* insPL);
	bool (*changeAttackY)(PL* insPL);
	
protected:
	int _cg, useAnim, attackNumOld, waitNextAttack;
	int _x, _y, stepChargeSec, isStep;
	int isCharge;//0.チャージしてない 1.チャージ中 2.解放
	int chargeLevel;
	float spd, animSpd, dodgeDir;
	bool isUseFbx, attackFlag, isDash, isAnimEnd;
	float maxHitPoint, maxBloodPoint, maxStamina;
	float nowActionTime;
	bool bufferedInput;
	int immortalTime;//, dodgeTime;
	int playSoundOld[3];
	std::vector<std::vector<int> > soundHandle;
	modelManager _modelManager;
	ExclusiveState Estate, oldEstate, lastAttackState;
	pushButton nextKey;
	modelInf* Einf;

	float neckDir = 0.f;
};
