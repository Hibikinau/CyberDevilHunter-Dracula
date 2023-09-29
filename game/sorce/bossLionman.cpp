/*****************************************************************//**
 * \file    bossLionman.h
 * \brief   ボス「ライオンマン」クラスの定義
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#include"bossLionman.h"
#include <math.h>
#define runSpd 25.f

using namespace model;

bool BossLion::Initialize()
{
	modelImport("game/res/Enemy02_mv1/Enemy02.mv1", 2.5f, &_modelInf, RS);
	status = STATUS::WAIT;
	time = 200;
	stanTime = 200;
	hitTime = 0;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 12000;
	_statusInf.redHitPoint = 0;
	_statusInf.stanPoint = 0;
	hitFlag = false;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	actionFlag = false;
	g = 3.f;
	swingSE = LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3");
	ChangeVolumeSoundMem(120, swingSE);
	awake = false;
	awakeDmg = 1;
	awakeMove = 1;
	awakeSpd = 1;
	awakeTime = 0;
	awakeAddDistance = 0;
	return true;
}

bool	BossLion::Terminate()
{
	CharBase::Terminate();
	DeleteSoundMem(swingSE);
	return true;
}

bool	BossLion::Process()
{
	//マスター音量の適応
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }

	if (status == STATUS::DEAD)
	{
		animSpd = 0.7f;
		animChange(BOSS2_DEAD, &_modelInf, false, true, false);
		if (isAnimEnd) { isDead = 2; }
		return true;
	}

	if (_statusInf.hitPoint <= 5000)
	{
		awake = true;
		awakeSpd = 1.5f;
		awakeMove = 1.5f;
		awakeDmg = 1.5f;
		awakeTime = 30;
		awakeAddDistance = 30;
	}

	if (status == STATUS::STAN)
	{
		if (!actionFlag)
		{
			animSpd = 1.0;
			animChange(BOSS2_DOWN, &_modelInf, true, true, false);
			actionFlag = true;
		}
		if (stanTime == 0)
		{
			status = STATUS::WAIT;
			time = 0;
			stanTime = 100;
			_statusInf.stanPoint = 0;
			actionFlag = false;
		}
		else { stanTime--; }
		return true;
	}

	for (auto i = charBox->begin(); i != charBox->end(); i++)
	{
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
			dodgeTime = i->second->dodgeTime;
		}
	}
	if (CheckHitKey(KEY_INPUT_K))
	{
		_statusInf.hitPoint = 1;
	}

	collCap.r = 80.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 60, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 300, 0));

	//bossと距離一定以内行動変更
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerDistance = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);

	int insAddNum = 0;
	bool insFreeBool = false;

	switch (status)
	{
	case STATUS::NONE:break;
	case STATUS::WAIT:
		animSpd = .5f;
		animChange(BOSS2_IDLE, &_modelInf, true, true, false);
		if (time == 0)
		{
			UtilityJudge();
		}
		else if (time > 0)
		{
			time--;
		}
		break;
	case STATUS::DAMAGE:
		if (_modelInf.isAnimEnd == true)
		{
			actionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::DAMAGE) { break; }
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 1.f;
		animChange(BOSS2_DAMAGE, &_modelInf, false, true, false);
		actionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = playerDir;
		animSpd = 1.0f * awakeSpd;
		animChange(BOSS2_RUN, &_modelInf, true, true, false);
		Move(runSpd * awakeMove, 0);
		if (playerDistance < 300) { UtilityJudge(); }
		if (time == 0) { UtilityJudge(); }
		else { time--; }
		break;
	case STATUS::FSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::FSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS2_DODGE, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 30)
		{
			Move(60.0 * awakeMove, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::BSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS2_DODGE, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 30)
		{
			Move(60.0 * awakeMove, 180.0);
		}
		break;
	case STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::RSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS2_DODGE, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 30)
		{
			Move(40.0 * awakeMove, 90.0);
		}
		break;
	case STATUS::LSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::LSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS2_DODGE, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 30)
		{
			Move(40.0 * awakeMove, 270.0);
		}
		break;
	case STATUS::ATTACK:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::ATTACK) {
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 1.5f * awakeSpd;
		animChange(BOSS2_ATTACK1, &_modelInf, false, true, true);
		makeAttackCap(VGet(-20.f, 0.f, -0.f), VGet(50.f, 0.f, 0.f), 40.f, 28.f, _modelInf.totalTime * awakeSpd - 28.f, animSpd, true, 20.f * awakeDmg, 0, 41, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::ATTACK2:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::ATTACK2)
			{
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 2.0f * awakeSpd;
		animChange(BOSS2_ATTACK2, &_modelInf, false, true, true);
		makeAttackCap(VGet(-20.f, 0.f, 0.f), VGet(50.f, 0.f, 0.f), 40.f, 75.f, _modelInf.totalTime * awakeSpd - 75.f, animSpd, true, 20.f * awakeDmg, 0, 18, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::ATTACK3:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::ATTACK3)
			{
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 2.0f * awakeSpd;
		animChange(BOSS2_ATTACK3, &_modelInf, false, true, true);
		makeAttackCap(VGet(-20.f, 0.f, 0.f), VGet(50.f, 0.f, 0.f), 40.f, 75.f, _modelInf.totalTime * awakeSpd - 75.f, animSpd, true, 20.f * awakeDmg, 0, 41, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::HANIATTACK:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::HANIATTACK)
			{
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 1.5f * awakeSpd;
		animChange(BOSS2_KNEE, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 60.f, 10.f, _modelInf.totalTime * awakeSpd - 10.f, animSpd, true, 20.f * awakeDmg, 0, 8, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::TACKLE:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			actionFlag = false;
			if (status != STATUS::TACKLE)
			{
				break;
			}
		}
		if (actionFlag == true)
		{
			if (_modelInf.playTime > 5 && _modelInf.playTime < 8)
			{
				Move(200 + awakeAddDistance, .0f);
			}
			if (_modelInf.playTime > 8 && _modelInf.playTime < 15)
			{
				Move(80 + awakeAddDistance, .0f);
			}
			if (_modelInf.playTime > 15 && _modelInf.playTime < 25)
			{
				Move(30 + awakeAddDistance, .0f);
			}
			break;
		}
		animSpd = 1.0f * awakeSpd;
		animChange(BOSS2_TACKLE, &_modelInf, false, false, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 240.f, .0f, _modelInf.totalTime * awakeSpd, animSpd, true, 50.f * awakeDmg, 0, 3, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::SLAM:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::SLAM)
			{
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true)
		{
			if (_modelInf.playTime > 24 && _modelInf.playTime < 71)
			{
				Move(playerRange / 30, .0f);
			}
			break;
		}
		animSpd = 1.0f * awakeSpd;
		animChange(BOSS2_SLAP, &_modelInf, false, false, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 240.f, .0f, _modelInf.totalTime * awakeSpd, animSpd, true, 50.f * awakeDmg, 0, 3, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::DIVE:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			actionFlag = false;
			if (status != STATUS::DIVE)
			{
				break;
			}
		}
		if (actionFlag == true)
		{
			if (_modelInf.playTime > 0 && _modelInf.playTime < 30)
			{
				RangeJ();
				_modelInf.dir.y = playerDir;
				Move(15, .0f);
			}
			if (_modelInf.playTime > 29 && _modelInf.playTime < 80)
			{
				if (dodgeTime == 0 && !hitFlag)
				{
					_modelInf.dir.y = playerDir;
				}
				Move(playerRange / 25, .0f);
			}
			if (_modelInf.playTime > 79 && _modelInf.playTime < 135)
			{
				Move(15, .0f);
			}
			break;
		}
		animSpd = 1.0f * awakeSpd;
		animChange(BOSS2_DIVE, &_modelInf, false, false, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 240.f, .0f, _modelInf.totalTime * awakeSpd, animSpd, true, 50.f * awakeDmg, 0, 3, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	};

	if (isHit && hitTime == 0)
	{
		hitFlag = true;
		hitTime = 50;
	}
	else if (hitFlag)
	{
		hitTime--;
		if (hitTime == 0) { hitFlag = false; }
	}

	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	//攻撃ヒット時のSE再生
	if (isHit)
	{
		isHit = false;
	}

	if (_statusInf.redHitPointDelayTime > 0) { _statusInf.redHitPointDelayTime--; }
	else { _statusInf.redHitPoint > 0 ? _statusInf.redHitPoint -= _statusInf.maxHitPoint / 600.f : _statusInf.redHitPoint = 0; }

	return true;
}

bool	BossLion::Render(float timeSpeed)
{
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);
	return true;
}


bool BossLion::UtilityJudge()
{
	attackStep = 0;
	_modelInf.dir.y = playerDir;
	int Rand = GetRand(100);
	if (!awake)
	{
		switch (status)
		{
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				status = STATUS::ATTACK2;
			}
			else
			{
				if (Rand > 50) { status = STATUS::DIVE; }
				if (Rand <= 50) { status = STATUS::RUN; time = 100; }
			}
			break;
		case STATUS::DAMAGE:
			status = STATUS::WAIT;
			break;
		case STATUS::DEAD:break;
		case STATUS::RUN:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand > 70) { status = STATUS::ATTACK; }
				if (Rand <= 70 || Rand > 40) { status = STATUS::ATTACK2; }
				if (Rand <= 40 || Rand > 10) { status = STATUS::HANIATTACK; }
				if (Rand <= 10) { status = STATUS::WAIT; }
			}
			if (range == RANGE::MidRange)
			{
				if (Rand < 50) { status = STATUS::SLAM; }
				if (Rand >= 50) { status = STATUS::RUN; }
			}
			if (range == RANGE::LongRange)
			{
				if (Rand < 50) { status = STATUS::DIVE; }
				if (Rand >= 50) { status = STATUS::RUN; }
				break;
			}
			time = 60;
			break;
		case STATUS::FSTEP:
			status = STATUS::TACKLE;
			break;
		case STATUS::BSTEP:
			if (Rand > 50) { status = STATUS::TACKLE; }
			if (Rand <= 50) { status = STATUS::HANIATTACK; }
			break;
		case STATUS::RSTEP:
			status = STATUS::RUN;
			time = 100;
			break;
		case STATUS::LSTEP:
			status = STATUS::RUN;
			time = 100;
			break;
		case STATUS::ATTACK:
			if (Rand >= 0 && Rand < 30)
			{
				status = STATUS::HANIATTACK;
			}
			if (Rand >= 30 && Rand < 50)
			{
				status = STATUS::BSTEP;
			}
			if (Rand >= 50 && Rand < 80)
			{
				status = STATUS::TACKLE;
			}
			if (Rand >= 80 && Rand <= 100)
			{
				status = STATUS::FSTEP;
			}
			break;
		case STATUS::ATTACK2:
			RangeJ();
			status = STATUS::ATTACK3;
			break;
		case STATUS::ATTACK3:
			RangeJ();
			if (range == RANGE::LongRange || range == RANGE::MidRange)
			{
				status = STATUS::DIVE;
			}
			if (range == RANGE::CrossRange)
			{
				if (Rand > 50)
				{
					status = STATUS::WAIT;
					time = 80;
				}
				if (Rand <= 50) { status = STATUS::TACKLE; }
				break;
			}
		case STATUS::HANIATTACK:
			if (Rand > 50) { status = STATUS::SLAM; }
			if (Rand <= 50) { status = STATUS::TACKLE; }
			break;
		case STATUS::TACKLE:
			if (Rand > 50)
			{
				status = STATUS::WAIT;
				time = 80;
			}
			if (Rand <= 50) { status = STATUS::ATTACK2; }
			break;
		case STATUS::SLAM:
			RangeJ();
			if (Rand > 50)
			{
				status = STATUS::WAIT;
				time = 80;
			}
			if (Rand <= 50) { status = STATUS::TACKLE; }
			break;
		case STATUS::DIVE:
			RangeJ();
			status = STATUS::WAIT;
			time = 100;
			break;
		};
	}
	if (awake)
	{
		switch (status)
		{
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				status = STATUS::ATTACK2;
			}
			else
			{
				if (Rand > 50) { status = STATUS::DIVE; }
				if (Rand <= 50) { status = STATUS::RUN; time = 50; }
			}
			break;
		case STATUS::DAMAGE:
			status = STATUS::WAIT;
			break;
		case STATUS::DEAD:break;
		case STATUS::RUN:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand > 70) { status = STATUS::ATTACK; }
				if (Rand <= 70 || Rand > 40) { status = STATUS::ATTACK2; }
				if (Rand <= 40 || Rand > 10) { status = STATUS::HANIATTACK; }
				if (Rand <= 10) { status = STATUS::WAIT; }
			}
			if (range == RANGE::MidRange)
			{
				if (Rand < 50) { status = STATUS::SLAM; }
				if (Rand >= 50)
				{
					status = STATUS::RUN;
				}
			}
			if (range == RANGE::LongRange)
			{
				if (Rand < 50) { status = STATUS::DIVE; }
				if (Rand >= 50)
				{
					status = STATUS::RUN;
				}
				break;
			}
			time = 60;
			break;
		case STATUS::FSTEP:
			status = STATUS::TACKLE;
			break;
		case STATUS::BSTEP:
			if (Rand > 50) { status = STATUS::TACKLE; }
			if (Rand <= 50) { status = STATUS::HANIATTACK; }
			break;
		case STATUS::RSTEP:
			status = STATUS::RUN;
			time = 100;
			break;
		case STATUS::LSTEP:
			status = STATUS::RUN;
			time = 100;
			break;
		case STATUS::ATTACK:
			if (Rand >= 0 && Rand < 30)
			{
				status = STATUS::HANIATTACK;
			}
			if (Rand >= 30 && Rand < 50)
			{
				status = STATUS::BSTEP;
			}
			if (Rand >= 50 && Rand < 80)
			{
				status = STATUS::TACKLE;
			}
			if (Rand >= 80 && Rand <= 100)
			{
				status = STATUS::FSTEP;
			}
			break;
		case STATUS::ATTACK2:
			RangeJ();
			status = STATUS::ATTACK3;
			break;
		case STATUS::ATTACK3:
			RangeJ();
			if (range == RANGE::LongRange || range == RANGE::MidRange)
			{
				status = STATUS::DIVE;
			}
			if (range == RANGE::CrossRange)
			{
				if (Rand > 50)
				{
					status = STATUS::WAIT;
					time = 80 - awakeTime;
				}
				if (Rand <= 50) { status = STATUS::TACKLE; }
				break;
			}
			break;
		case STATUS::HANIATTACK:
			if (Rand > 50) { status = STATUS::SLAM; }
			if (Rand <= 50) { status = STATUS::TACKLE; }
			break;
		case STATUS::TACKLE:
			if (Rand > 19)
			{
				status = STATUS::TACKLE;
			}
			if (Rand <= 19)
			{
				status = STATUS::ATTACK2;
			}
			break;
		case STATUS::SLAM:
			RangeJ();
			if (Rand > 50)
			{
				status = STATUS::WAIT;
				time = 80 - awakeTime;
			}
			if (Rand <= 50) { status = STATUS::TACKLE; }
			break;
		case STATUS::DIVE:
			RangeJ();
			status = STATUS::WAIT;
			time = 80 - awakeTime;
			break;
		};
	}


	return true;
}

bool BossLion::RangeJ()
{
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerRange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	if (playerRange < 300)
	{
		range = RANGE::CrossRange;
	}
	if (300 <= playerRange && playerRange <= 2000)
	{
		range = RANGE::MidRange;
	}
	if (playerRange > 2000)
	{
		range = RANGE::LongRange;
	}
	return true;
}


void BossLion::Move(float speed, float radian)
{
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}


bool BossLion::HPmath(float Num, float Stan)
{
	_statusInf.hitPoint += Num;
	if (_statusInf.redHitPoint <= 0) { _statusInf.redHitPointDelayTime = 100; }
	_statusInf.redHitPoint += std::abs(Num);
	_statusInf.stanPoint += Stan;
	if (Num <= -200)
	{
		status = STATUS::DAMAGE;
		actionFlag = false;
	}
	if (_statusInf.hitPoint <= 5000)
	{
		awake = true;
	}
	if (_statusInf.stanPoint >= 150)
	{
		status = STATUS::STAN;
		_statusInf.stanPoint = 150;
		actionFlag = false;
	}
	if (_statusInf.hitPoint <= 0)
	{
		status = STATUS::DEAD;
	}

	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);

	return true;
}