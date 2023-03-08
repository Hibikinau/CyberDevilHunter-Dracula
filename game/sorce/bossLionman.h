#pragma once
#include"charBase.h"
#include"player.h"

class BossLion : public CB
{
public:
	BossLion() {};
	~BossLion() {};
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Input() { return true; }
	virtual bool Process();
	virtual bool Render(float timeSpeed);
	bool HPmath(float Num, float Stan) override;
	bool UtilityJudge();
	bool RangeJ();

	//キャラクターの移動(1.移動速度,2.移動方向)
	void Move(float speed, float radian);


	enum class STATUS {
		NONE,
		WAIT,
		DAMEGE,
		STAN,
		DEAD,
		RUN,
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,
		ATTACK,
		ATTACK2,
		ATTACK3,
		HANIATTACK,
		TACKLE,
		SLAM,
		DIVE,
	};
	STATUS status;

	enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

protected:
	float animSpd, stanTime;
	int time, attackStep;
	bool AttackFlag, isAnimEnd;
	modelInf* plMI, stage;
	double dir;
	bool MotionFlag;
	bool ActionFlag;
	bool jumpActFlag;
	int  swingSE;
	float PrangeA, PrangeB, Pdir, Prange, atkDir;
	bool Awake;
	float AwakeSpd, AwakeDmg, AwakeMove, AwakeT;
};
