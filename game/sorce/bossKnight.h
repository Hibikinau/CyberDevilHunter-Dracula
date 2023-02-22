#pragma once
#include"charBase.h"
#include"player.h"



class BossKnight : public CB
{
public:
	BossKnight() {};
	~BossKnight() {};
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Input(){ return true; }
	virtual bool Process();
	virtual bool Render(float timeSpeed);
	bool HPmath(float Num) override;
	bool UtilityJudge();
	bool RangeJ();

	//キャラクターの移動(1.移動速度,2.移動方向)
	void Move(float speed, float radian);


	enum class STATUS {
		NONE,
		WAIT,
		DAMEGE,
		DEAD,
		RUN,
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,
		SRASH,
		SLAM,
		STAB,
		ROBES,
		JAMPACT,
		ONESLASH,
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
	float animSpd;
	int time, attackStep;
	modelManager _modelManager;
	bool AttackFlag, isAnimEnd;
	modelInf* plMI, stage;
	double dir;
	bool MotionFlag;
	bool ActionFlag;
	bool jumpActFlag;
	int  swingSE;
	float PrangeA, PrangeB, Pdir, Prange, atkDir;
	bool Awake;
	float AwakeSpd, AwakeDmg,AwakeMove,AwakeT;
};