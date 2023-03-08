#include"bossKnight.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 20.f

using namespace model;

bool BossKnight::Initialize()
{
	modelImport("game/res/Enemy01/MV1/enemy_1_.mv1", 2.5f, &_modelInf, RS);
	status = STATUS::WAIT;
	time = 300;
	stanTime = 150;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 10000;
	_statusInf.stanPoint = 0;

	MotionFlag = true;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	AttackFlag = false;
	g = 3.f;
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3"));
	Awake = false;
	AwakeDmg = 1;
	AwakeMove = 1;
	AwakeSpd = 1;
	AwakeT = 0;
	setMasterVolume(420 * ( 0.01 * _valData->soundMasterValume));
	return true;
}

bool	BossKnight::Terminate()
{
	CB::Terminate();
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

bool	BossKnight::Process()
{
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	if (_statusInf.hitPoint <= 5000) {
		AwakeSpd = 1.5f;
		AwakeMove = 1.5f;
		AwakeDmg = 1.5f;
		AwakeT = 35;
	}

	if (status == STATUS::STAN) {
		animChange(BOSS1_hidan, &_modelInf, true, true, false);
		if (stanTime == 0) {
			status = STATUS::WAIT;
			stanTime = 100;
			_statusInf.stanPoint = 0;
		}
		else { stanTime--; }
		return true;
	}

	if (status == STATUS::DEAD) {
		animChange(BOSS1_dead, &_modelInf, false, true, false);
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

	//auto xz = plMI->pos;

	//boss‚Æ‹——£ˆê’èˆÈ“às“®•ÏX
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	PrangeA = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	//float Prange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);



	int insAddNum = 0;
	bool insFreeBool = false;

	switch (status) {
	case STATUS::NONE:break;
	case STATUS::WAIT:
		animSpd = .5f;
		animChange(BOSS1_idel, &_modelInf, true, true, false);
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
		animChange(BOSS1_hidan, &_modelInf, false, true, false);
		ActionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = Pdir;
		animSpd = 0.8f*AwakeSpd;
		animChange(BOSS1_run, &_modelInf, true, true, false);
		Move(runSpd*AwakeMove, 0);
		if (PrangeA < 200) { UtilityJudge(); }
		break;
	case STATUS::FSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::FSTEP) { break; }
		}
		animSpd = 1.f*AwakeSpd;
		animChange(BOSS1_dodgeF, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0*AwakeMove, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::BSTEP) { break; }
		}
		animSpd = 1.f*AwakeSpd;
		animChange(BOSS1_dodgeB, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0*AwakeMove, 180.0);
		}
		break;
	case STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::RSTEP) { break; }
		}
		animSpd = 1.f*AwakeSpd;
		animChange(BOSS1_dodgeR, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0*AwakeMove, 90.0);
		}
		break;
	case STATUS::LSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::LSTEP) { break; }
		}
		animSpd = 1.f*AwakeSpd;
		animChange(BOSS1_dodgeL, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0*AwakeMove, 270.0);
		}
		break;
	case STATUS::SRASH:
		if (_modelInf.isAnimEnd == true) {
			ActionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::SRASH) {
				break;
			}
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = .7f*AwakeSpd;
		animChange(BOSS1_nagiharai, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 50.f, 10.f + 18.f, _modelInf.totalTime * AwakeSpd, true, 20.f * AwakeDmg, 100, Char_BOSS1);
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::SLAM:
		if (isAnimEnd == true) {
			ActionFlag = false;
			if (attackStep < 8)
			{
				RangeJ();
				if (attackStep == 1 && (rand() % 100) < 50)
				{
					attackStep++;
					status = STATUS::ONESLASH;
					UtilityJudge();
					break;
				}
				if (attackStep == 3 && range==RANGE::MidRange)
				{
					attackStep=0;
					status = STATUS::JAMPACT;
					//UtilityJudge();
					break;
				}
				attackStep++;
			}
			else {
				UtilityJudge();
				if (status != STATUS::SLAM) { break; }
			}
		}
		if (ActionFlag == true) { break; }
		if (attackStep == 1 || attackStep == 4 || attackStep == 7)
		{
			makeAttackCap(VGet(0.f, 50.f, 0.f), VGet(0.f, -100.f, 0.f), 50.f, 0.f, _modelInf.totalTime, true, 15.f*AwakeDmg, 100, Char_BOSS1);
			PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		}
		attackStep > 3 ? animSpd = 1.3f*AwakeSpd : animSpd = .7f*AwakeSpd;
		attackStep > 5 ? insAddNum = 12 : insAddNum = 0;
		animChange(BOSS1_tatakituke_r1 + attackStep + insAddNum, &_modelInf, false, false, true);
		ActionFlag = true;
		break;
	case STATUS::STAB:
		if (isAnimEnd == true) {
			ActionFlag = false;
			if (attackStep < 4) { attackStep++; }
			else {
				UtilityJudge();
				if (status != STATUS::STAB) { break; }
			}
		}
		if (ActionFlag == true)
		{
			if ((attackStep == 2 && _modelInf.playTime > 5) || attackStep == 3) { Move(90.0f*AwakeMove, .0f); }
			break;
		}

		animSpd = 0.7f*AwakeSpd;
		animChange(BOSS1_tuki1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 2 || attackStep == 3)
		{
			makeAttackCap(VGet(0.f, 50.f, 0.f), VGet(0.f, -100.f, 0.f), 100.f, .0f, _modelInf.totalTime , true, 30.f*AwakeDmg, 100, Char_BOSS1);
		}
		ActionFlag = true;
		break;
	case STATUS::ROBES:
		if (isAnimEnd == true) {
			ActionFlag = false;
			if (attackStep < 3) { attackStep++; }
			else {
				UtilityJudge();
				if (status != STATUS::ROBES) { break; }
			}
		}
		if (ActionFlag == true) { break; }

		animSpd = 0.7f*AwakeSpd;
		animChange(BOSS1_kesa1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 2)
		{
			makeAttackCap(VGet(0.f, 50.f, 0.f), VGet(0.f, -120.f, 0.f), 50.f, .0f, _modelInf.totalTime , true, 20.f*AwakeDmg, 100, Char_BOSS1);
		}
		ActionFlag = true;
		break;
	case STATUS::JAMPACT:
		if (isAnimEnd == true) {
			ActionFlag = false;
			if (attackStep < 3)
			{
				if (isGround && attackStep == 2) { makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 500.f, .0f, _modelInf.totalTime , true, 50.f * AwakeDmg, 100, Char_BOSS1); }
				if (_modelInf.vec.y > 0 && attackStep == 1) { ActionFlag = true; }
				else if (!isGround && attackStep == 2) { ActionFlag = true; }
				else { attackStep++; }

			}
			else {
				UtilityJudge();
				if (status != STATUS::JAMPACT) { break; }
			}
		}
		if (ActionFlag == true)
		{
			if ((attackStep == 1 && _modelInf.playTime > 5) || attackStep == 2)
			{
				if (!jumpActFlag) { _modelInf.vec.y = 45, jumpActFlag = true; }
				Move(Prange / 40, .0f);
			}
			else if (attackStep == 1 && _modelInf.playTime <= 5) { _modelInf.dir.y = Pdir; RangeJ(); }
			break;
		}
		animSpd = 0.7f*AwakeSpd;
		animChange(BOSS1_jumpA1 + attackStep - 1, &_modelInf, false, false, true);
		//if (attackStep == 2 || attackStep == 3)
		/*{
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 500.f, .0f, _modelInf.totalTime / animSpd + 1, true, 50.f*AwakeDmg, 100, Char_BOSS1);

		}*/
		if (attackStep == 1) { RangeJ(); }
		ActionFlag = true;
		break;
	};


	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	return true;
}

bool	BossKnight::Render(float timeSpeed)
{

	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	_modelInf.animHandleOld == BOSS1_run ? _modelInf.addPos = VGet(0, 80.f, 0) : _modelInf.addPos = VGet(0, 0, 0);
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);

	return true;
}


bool BossKnight::UtilityJudge() {
	attackStep = 0, jumpActFlag = false;
	_modelInf.dir.y = Pdir;
	int Rand = GetRand(100);

	if (!Awake) {
		switch (status) {
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange) {
				if (Rand < 40) { status = STATUS::SRASH; }
				if (Rand >= 40) {
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) {
				if (Rand < 80) { status = STATUS::STAB; }
				if (Rand >= 80) { status = STATUS::RUN; }
			}
			if (range == RANGE::LongRange) {
				if (Rand < 70) { status = STATUS::JAMPACT; }
				if (Rand >= 70) { status = STATUS::RUN; }
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
				if (Rand >= 65) {
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
			if (range == RANGE::LongRange) {
				status = STATUS::JAMPACT;
				break;
			}
			break;
		case STATUS::FSTEP:
			if (Rand < 70) { status = STATUS::SRASH; }
			if (Rand >= 70) { status = STATUS::SLAM; }
			break;
		case STATUS::BSTEP:
			//status = STATUS::ROBES; break;
			RangeJ();
			if (range == RANGE::CrossRange) { status = STATUS::SRASH; break; }
			if (range == RANGE::MidRange) { status = STATUS::STAB; break; }
			if (range == RANGE::LongRange) { status = STATUS::JAMPACT; break; }
			break;
		case STATUS::RSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::LSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::SRASH:
			if (Rand > 80) {
				break;
			}
			if (80 >= Rand && Rand > 40) {
				status = STATUS::BSTEP;
				break;
			}
			if (40 >= Rand) {
				status = STATUS::ROBES;
				break;
			}
			break;
		case STATUS::SLAM:
			status = STATUS::WAIT;
			time = 50 - AwakeT;
			break;
		case STATUS::STAB:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::JAMPACT; break; }
			break;
		case STATUS::ROBES:
			status = STATUS::WAIT;
			time = 50 - AwakeT;
			break;
		case STATUS::JAMPACT:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::ROBES; break; }
			break;
		case STATUS::ONESLASH:
			status = STATUS::BSTEP; break;
			break;
		};
	}
	else if (Awake) {
		switch (status) {
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange) {
				if (Rand < 40) { status = STATUS::SRASH; }
				if (Rand >= 40) {
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) {
				if (Rand < 80) { status = STATUS::STAB; }
				if (Rand >= 80) { status = STATUS::RUN; }
			}
			if (range == RANGE::LongRange) {
				if (Rand < 70) { status = STATUS::JAMPACT; }
				if (Rand >= 70) { status = STATUS::RUN; }
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
				if (Rand >= 65) {
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
			if (range == RANGE::LongRange) {
				status = STATUS::JAMPACT;
				break;
			}
			break;
		case STATUS::FSTEP:
			if (Rand < 70) { status = STATUS::SRASH; }
			if (Rand >= 70) { status = STATUS::SLAM; }
			break;
		case STATUS::BSTEP:
			//status = STATUS::ROBES; break;
			RangeJ();
			if (range == RANGE::CrossRange) { status = STATUS::SRASH; break; }
			if (range == RANGE::MidRange) { status = STATUS::STAB; break; }
			if (range == RANGE::LongRange) { status = STATUS::JAMPACT; break; }
			break;
		case STATUS::RSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::LSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::SRASH:
			if (Rand > 80) {
				break;
			}
			if (80 >= Rand && Rand > 40) {
				status = STATUS::BSTEP;
				break;
			}
			if (40 >= Rand) {
				status = STATUS::ROBES;
				break;
			}
			break;
		case STATUS::SLAM:
			status = STATUS::WAIT;
			time = 50 - AwakeT;
			break;
		case STATUS::STAB:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::JAMPACT; break; }
			break;
		case STATUS::ROBES:
			status = STATUS::WAIT;
			time = 50 - AwakeT;
			break;
		case STATUS::JAMPACT:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::ROBES; break; }
			break;
		case STATUS::ONESLASH:
			status = STATUS::BSTEP; break;
			break;
		};
	}


	return true;
}

bool BossKnight::RangeJ() {
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


void BossKnight::Move(float speed, float radian) {
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;
	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}


bool BossKnight::HPmath(float Num,float Stan)
{
	_statusInf.hitPoint += Num;
	_statusInf.stanPoint -= Num;
	if (Num <= -200) {
		status = STATUS::DAMEGE;
		ActionFlag = false;
	}
	if (_statusInf.hitPoint <=5000) {
		Awake = true;
	}
	if (_statusInf.hitPoint <= 0) {
		status = STATUS::DEAD;
	}
	if (_statusInf.stanPoint >= 100) {
		status = STATUS::STAN;
	}
	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);*/

	return true;
}
