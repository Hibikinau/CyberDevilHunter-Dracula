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
	void Walk();
	void Backwalk();
	void Step();
	void CRange();
	void MRange();
	void LRange();

	enum class STATUS {
		NONE,
		WAIT,
		WALK,
		KICK,
		SRASH,
		BACK,
		STEP
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
};