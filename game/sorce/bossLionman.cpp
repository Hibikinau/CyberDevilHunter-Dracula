#include"bossLionman.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 22.f

using namespace model;

bool BossLion::Initialize()
{
	modelImport("game/res/Enemy02_mv1/Enemy02.mv1", 2.5f, &_modelInf, RS);
	status = STATUS::WAIT;
	time = 300;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 15000;

	MotionFlag = true;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	AttackFlag = false;
	g = 3.f;
	swingSE = LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3");
	ChangeVolumeSoundMem(520, swingSE);
	Awake = false;
	AwakeDmg = 1;
	AwakeMove = 1;
	AwakeSpd = 1;
	AwakeT = 0;
	return true;
}

bool	BossLion::Terminate()
{
	CB::Terminate();
	DeleteSoundMem(swingSE);
	return true;
}

bool	BossLion::Process()
{
	if (_statusInf.hitPoint <= 5000) {
		AwakeSpd = 1.5f;
		AwakeMove = 1.5f;
		AwakeDmg = 1.5f;
		AwakeT = 25;
	}

	if (status == STATUS::DEAD) {
		animChange(BOSS2_dead, &_modelInf, false, true, false);
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

	collCap.r = 80.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 60, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 300, 0));

	//boss‚Æ‹——£ˆê’èˆÈ“às“®•ÏX
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	PrangeA = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);

	int insAddNum = 0;
	bool insFreeBool = false;

	switch (status) {
	case STATUS::NONE:break;
	case STATUS::WAIT:
		animSpd = .5f;
		animChange(BOSS2_idel, &_modelInf, true, true, false);
		if (time == 0) {
			UtilityJudge();
		}
		else if (time > 0) {
			time--;
		}
		break;
	case STATUS::DAMEGE:
		if (_modelInf.isAnimEnd == true) {
			ActionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::DAMEGE) { break; }
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = 1.f;
		animChange(BOSS2_dead, &_modelInf, false, true, false);
		//PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = Pdir;
		animSpd = 1.0f * AwakeSpd;
		animChange(BOSS2_run, &_modelInf, true, true, false);
		Move(runSpd * AwakeMove, 0);
		if (PrangeA < 300) { UtilityJudge(); }
		break;
	case STATUS::FSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::FSTEP) { break; }
		}
		animSpd = 1.f * AwakeSpd;
		animChange(BOSS2_dodge, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * AwakeMove, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::BSTEP) { break; }
		}
		animSpd = 1.f * AwakeSpd;
		animChange(BOSS2_dodge, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * AwakeMove, 180.0);
		}
		break;
	case STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::RSTEP) { break; }
		}
		animSpd = 1.f * AwakeSpd;
		animChange(BOSS2_dodge, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * AwakeMove, 90.0);
		}
		break;
	case STATUS::LSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::LSTEP) { break; }
		}
		animSpd = 1.f * AwakeSpd;
		animChange(BOSS2_dodge, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * AwakeMove, 270.0);
		}
		break;
	case STATUS::ATTACK:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::ATTACK) {
				ActionFlag = false;
				break;
			}
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = 1.5f * AwakeSpd;
		animChange(BOSS2_attack1, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 40.f, 10.f, (_modelInf.totalTime / animSpd + 1) - 10.f, true, 20.f * AwakeDmg, 100, Char_BOSS1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	};


	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	return true;
}

bool	BossLion::Render(float timeSpeed)
{

	DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	_modelInf.animHandleOld == BOSS1_run ? _modelInf.addPos = VGet(0, 80.f, 0) : _modelInf.addPos = VGet(0, 0, 0);
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);

	return true;
}


bool BossLion::UtilityJudge() {
	attackStep = 0, jumpActFlag = false;
	_modelInf.dir.y = Pdir;
	int Rand = GetRand(100);
	if (!Awake) {
		switch (status) {
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			status = STATUS::RUN;
			break;
		case STATUS::DAMEGE:
			status = STATUS::WAIT;
			break;
		case STATUS::DEAD:break;
		case STATUS::RUN:
			RangeJ();
			if (range==RANGE::CrossRange) {
				status = STATUS::ATTACK;
			}
			break;
		case STATUS::FSTEP:
			status = STATUS::WAIT;
			time = 100;
			break;
		case STATUS::BSTEP:
			status = STATUS::WAIT;
			time = 100;
			break;
		case STATUS::RSTEP:
			status = STATUS::WAIT;
			time = 100;
			break;
		case STATUS::LSTEP:
			status = STATUS::WAIT;
			time = 100;
			break;
		case STATUS::ATTACK:
			if (Rand >= 0 || Rand < 25) {
				status = STATUS::FSTEP;
			}
			if (Rand >= 25 || Rand < 50) {
				status = STATUS::BSTEP;
			}
			if (Rand >= 50 || Rand < 75) {
				status = STATUS::RSTEP;
			}
			if (Rand >= 75 || Rand <= 100) {
				status = STATUS::LSTEP;
			}
			break;
		};
	}




	return true;
}

bool BossLion::RangeJ() {
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	Prange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	if (Prange < 300)
	{
		range = RANGE::CrossRange;
	}
	if (300 <= Prange && Prange <= 2000)
	{
		range = RANGE::MidRange;
	}
	if (Prange > 2000)
	{
		range = RANGE::LongRange;
	}
	return true;
}


void BossLion::Move(float speed, float radian) {
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}


bool BossLion::HPmath(float Num)
{
	_statusInf.hitPoint += Num;
	if (Num <= -200) {
		status = STATUS::DAMEGE;
		ActionFlag = false;
	}
	if (_statusInf.hitPoint <= 5000) {
		Awake = true;
	}
	if (_statusInf.hitPoint <= 0) {
		status = STATUS::DEAD;
	}/*
	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);*/

	return true;
}