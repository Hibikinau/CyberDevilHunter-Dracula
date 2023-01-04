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
	chargeATTACK,
	DODGE,
};

enum class pushButton
{
	Neutral,
	A,
	B,
	X,
	Y,
	Lstick,
	Irregular,
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
	virtual bool Render();
	virtual void charMove(float Speed, float _Dir, bool animChange);
	bool getInputKey(imputInf *iInf, float* cameraDir)
	{
		_imputInf = iInf, _cameraDir = cameraDir; return true;
	};
	pushButton setAction();
	bool checkTrgImput(int Key, int Pad) { if (_imputInf->_gTrgb[Key] || _imputInf->_gTrgp[Pad]) { return true; } else { return false; } }
	bool checkKeyImput(int Key, int Pad) { if (_imputInf->_gKeyb[Key] || _imputInf->_gKeyp[Pad]) { return true; } else { return false; } }
	float getMoveDir();

protected:
	int _cg, useAnim, attackNumOld, waitNextAttack;
	int _x, _y, stepChargeSec, isStep;
	int isCharge;//0.チャージしてない 1.チャージ中 2.解放
	int chargeLevel;
	float spd, animSpd, * _cameraDir, dodgeDir;
	bool isUseFbx, attackFlag, isDash, isAnimEnd;
	float maxHitPoint, maxBloodPoint, maxStamina;
	float nowActionTime;
	bool bufferedInput;
	int immortalTime;
	modelManager _modelManager;
	ExclusiveState Estate, oldEstate;
	pushButton nextKey;
	imputInf *_imputInf;
	modelInf *Einf;
};