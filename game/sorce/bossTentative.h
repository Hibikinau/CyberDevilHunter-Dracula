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
	virtual bool Render(float timeSpeed);
	bool HPmath(float Num) override;
	bool UtilityJudge();
	bool RangeJ();

	//キャラクターの移動(1.移動速度,2.移動方向)
	void Move(float speed,float radian); 


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
            JAMPACT,
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
	int useAnim;
	int _x, _y;
	bool isUseFbx;
	float animSpd,*_cameraSpd;
	int time;
	modelManager _modelManager;
	bool AttackFlag, isAnimEnd;
	modelInf* plMI, stage;
	double dir;
	bool MotionFlag;
	bool ActionFlag;
	int swingSE;
	float PrangeA,PrangeB;
};