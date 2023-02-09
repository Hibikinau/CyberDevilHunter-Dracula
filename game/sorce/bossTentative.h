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
	bool HPmath(float Num) override;
	bool UtilityJudge();
	bool RangeJ();

	void Walk();
	void Step();
	void BackStep();
	void LeftStep();
	void RightStep();

	void CRange();
	void MRange();
	void LRange();

	/*enum class STATUS {
		NONE,
		WAIT,
		WALK,
		ATTACK,
		SRASH,
		BACK,
		STEP,
		LEFT,
		RIGHT,
		DEAD
	};
	STATUS status;*/

	enum class STATUS {
		    NONE,
			WAIT,
			DEAD,
			RUN,
			FSTEP,
			BSTEP,
			RSTEP,
			LSTEP,
			SRASH,
			SLAM,
			STAB,
            JAMPA,
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
	int swingSE;
	float PrangeA,PrangeB;
};