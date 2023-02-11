#include"bossTentative.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 20.f


bool Boss::Initialize()
{
	_modelManager.modelImport("game/res/Enemy01/MV1/enemy_1_.mv1", 2.0f, &_modelInf);
	useAnim = 0;

	status = STATUS::WAIT;
	time = 300;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 10000;

	MotionFlag = true;
	_modelInf.pos = VGet(0.0f, 0.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	AttackFlag = false;
	g = 1.f;
	swingSE = LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3");
	ChangeVolumeSoundMem(520, swingSE);
	return true;
}

bool	Boss::Terminate()
{
	CB::Terminate();
	return true;
}

bool	Boss::Input()
{

	return true;
}

bool	Boss::Process()
{
	if (status == STATUS::DEAD) {
		_modelManager.animChange(motion_dead, &_modelInf, false, true, false);
		if (isAnimEnd) { isDead = 2; }
		return true;
	}

	for (auto i = charBox->begin(); i != charBox->end(); i++) {
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
		}
	}
	if (CheckHitKey(KEY_INPUT_K))
	{
		_statusInf.hitPoint = 1;
	}

	collCap.r = 70.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 60, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 300, 0));

	//auto xz = plMI->pos;

	//boss‚Æ‹——£ˆê’èˆÈ“às“®•ÏX
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	auto Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	PrangeA = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	//float Prange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);


	if (MotionFlag == true && time == 0) {
		PrangeB = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
		_modelInf.dir.y = Pdir;
		UtilityJudge();

	}
	else if (time > 0)
	{
		time--;
	}


	switch (status) {
	case STATUS::NONE:break;
	case STATUS::WAIT:
		_modelManager.animChange(motion_idel, &_modelInf, true, true, false);
		animSpd = 0.5f;
		break;
	case STATUS::DAMEGE:
		if (isAnimEnd == true) {
			ActionFlag = false;
			UtilityJudge();
		}
		if (ActionFlag == true) {
			break;
		}
		_modelManager.animChange(motion_attack1, &_modelInf, false, false, true);
		animSpd = 0.7f;
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = Pdir;
		_modelManager.animChange(motion_run, &_modelInf, true, true, false);
		animSpd = 0.5f;
		Move(8.5, 0);
		if (PrangeA < 150) { UtilityJudge(); }
		/*if(Prange>100) { Walk(); }
		else {
			int j;
		}*/
		//AttackFlag = false;
		break;
	case STATUS::FSTEP:
		if (isAnimEnd == true) {
			ActionFlag = false;
			UtilityJudge();
		}
		if (ActionFlag == true) {
			break;
		}
		_modelManager.animChange(motion_dodgeF, &_modelInf, false, true, true);
		animSpd = 1.0f;
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (isAnimEnd == true) {
			ActionFlag = false;
			UtilityJudge();
		}
		if (ActionFlag == true) {
			break;
		}
		_modelManager.animChange(motion_dodgeB, &_modelInf, false, true, true);
		animSpd = 1.0f;
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0, 180.0);
		}
		break;
	case STATUS::RSTEP:
		Move(40.0, 90.0);
		break;
	case STATUS::LSTEP:
		Move(40.0, 270.0);
		break;
	case STATUS::SRASH:
		if (isAnimEnd == true) {
			ActionFlag = false;
			UtilityJudge();
		}
		if (ActionFlag == true) {
			break;
		}
		_modelManager.animChange(motion_attack1, &_modelInf, false, false, true);
		animSpd = 0.7f;
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 40.f, 10.f, _modelInf.totalTime / animSpd + 1, true, 5.f, 100, Char_BOSS1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::SLAM:break;
	case STATUS::STAB:break;
	case STATUS::JAMPACT:break;
	};


	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	return true;
}

bool	Boss::Render(float timeSpeed)
{

	DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);

	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd, timeSpeed);

	return true;
}


bool Boss::UtilityJudge() {

	int Rand = GetRand(100);
	switch (status) {
	case STATUS::NONE:
	case STATUS::WAIT:
		RangeJ();
		if (range == RANGE::CrossRange) {
			if (Rand < 40) { status = STATUS::SRASH; }
			if (Rand >= 40) { status = STATUS::SLAM; }
			break;
		}
		if (range == RANGE::MidRange) {
			if (Rand < 80) { status = STATUS::STAB; }
			if (Rand >= 80) { status = STATUS::RUN; }
		}
		if (range == RANGE::LongRange) {
			if (Rand < 70) { status = STATUS::RUN; }
			if (Rand >= 70) { status = STATUS::JAMPACT; }
			break;
		}
		break;
	case STATUS::DAMEGE:
		status = STATUS::WAIT;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		RangeJ();
		if (range == RANGE::CrossRange) {
			if (Rand < 65) { status = STATUS::SRASH; }
			if (Rand >= 65) { status = STATUS::SLAM; }
			break;
		}
		if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
		if (range == RANGE::LongRange) { time = 300; break; }
		break;
	case STATUS::FSTEP:
		status = STATUS::SRASH;
		break;
	case STATUS::BSTEP:
		RangeJ();
		if (range == RANGE::CrossRange) { status = STATUS::SRASH; break; }
		if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
		if (range == RANGE::LongRange) { status = STATUS::WAIT; time = 50; break; }
		break;
	case STATUS::RSTEP:
		status = STATUS::SLAM;
		break;
	case STATUS::LSTEP:
		status = STATUS::SLAM;
		break;
	case STATUS::SRASH:
		if (Rand > 60) {
			break;
		}
		if (Rand <= 60) { status = STATUS::BSTEP; break; }
		break;
	case STATUS::SLAM:
		status = STATUS::WAIT;
		break;
	case STATUS::STAB:
		if (Rand < 25) { status = STATUS::FSTEP; }
		if (25 <= Rand || Rand < 50) { status = STATUS::BSTEP; }
		if (50 <= Rand || Rand < 75) { status = STATUS::RSTEP; }
		if (75 <= Rand) { status = STATUS::LSTEP; }
		break;
	case STATUS::JAMPACT:
		if (Rand < 25) { status = STATUS::FSTEP; }
		if (25 <= Rand || Rand < 50) { status = STATUS::BSTEP; }
		if (50 <= Rand || Rand < 75) { status = STATUS::RSTEP; }
		if (75 <= Rand) { status = STATUS::LSTEP; }
		break;
	};


	return true;
}

bool Boss::RangeJ() {
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	auto Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	float Prange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	if (Prange < 350)
	{
		range = RANGE::CrossRange;
	}
	if (350 <= Prange && Prange <= 450)
	{
		range = RANGE::MidRange;
	}
	if (Prange > 450)
	{
		range = RANGE::LongRange;
	}
	return true;
}


void Boss::Move(float speed, float radian) {
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}

void Boss::CRange() {

	int AttackRand = GetRand(100);
	if (AttackRand <= 70) {
		//status = STATUS::ATTACK;

	}
	else if (AttackRand > 70) {
		//status = STATUS::BACK;

	}
	MotionFlag = false;
	return;
}

void Boss::MRange() {
	int AttackRand = GetRand(100);
	if (AttackRand <= 70) {
		//status = STATUS::SRASH;
	}
	else if (AttackRand > 70) {
		status = STATUS::SRASH;
		//status = STATUS::STEP;
	}
	MotionFlag = false;
	return;
}

void Boss::LRange() {
	//status = STATUS::WALK;



	return;
}

bool Boss::HPmath(float Num)
{

	_statusInf.hitPoint += Num;
	if (Num <= -200) {
		status = STATUS::DAMEGE;
		ActionFlag = false;
	}
	if (_statusInf.hitPoint <= 0) {
		status = STATUS::DEAD;
	}

	return true;
}
