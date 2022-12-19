#pragma once
#include"charBase.h"

////3dƒ‚ƒfƒ‹—p
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
	ATTACK,
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
	virtual void charMove(float Speed, float _Dir);
	bool getInputKey(int* padInput, int* padTrg, char* keyInput, char* keyTrg, float* cameraDir)
	{
		_gKeyp = padInput, _gTrgp = padTrg, _gKeyb = keyInput, _gTrgb = keyTrg, _cameraDir = cameraDir; return true;
	};
	pushButton setAction();
	bool checkTrgImput(int Key, int Pad) { if (_gTrgb[Key] || *_gTrgp & Pad) { return true; } else { return false; } }
	bool checkKeyImput(int Key, int Pad) { if (_gKeyb[Key] || *_gKeyp & Pad) { return true; } else { return false; } }
	float getMoveDir();

protected:
	int _cg, useAnim, attackNumOld, waitNextAttack;
	int _x, _y, stepChargeSec, isStep;
	float spd, animSpd, * _cameraDir;
	int* _gKeyp, *_gTrgp;
	char* _gKeyb, * _gTrgb;
	bool isUseFbx, attackFlag, isDash, isAnimEnd;
	float maxHitPoint, maxBloodPoint, maxStamina;
	float nowActionTime;
	modelManager _modelManager;
	ExclusiveState Estate;
	pushButton nextKey;

};