#pragma once
#include"charBase.h"
#include"player.h"



class Boss : public CB
{
public:
	Boss() {};
	~Boss() {};
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();
	virtual void charMove(float Speed, float _Dir);
	bool HPmath(float Num) override;
	bool step();
	void Walk(VECTOR x);
	void Backwalk(VECTOR x);
	void CRange();
	void MRange();
	void LRange();

	enum class STATUS {
		NONE,
		WAIT,
		WALK,
		KICK,
		SRASH,
		BACK
	};
	STATUS status;

	enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

	bool MO;

	int AttackNo;
	bool Attack;

protected:
	int _cg, useAnim;
	int _x, _y, stepChargeSec, isStep;
	float spd, animSpd, * _cameraDir;
	bool isUseFbx;
	int time;
	modelManager _modelManager;
	bool attackFlag, isDash, isAnimEnd;
	modelInf* plMI, stage;
	double dir;
};