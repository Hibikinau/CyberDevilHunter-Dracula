/*****************************************************************//**
 * \file   bossKnight.cpp
 * \brief  ボス「騎士」クラスの定義
 *
 * \author 松田　裕
 * \date  March  2023
 *********************************************************************/
#include"bossKnight.h"
#include <math.h>
#define runSpd 20.f

using namespace model;

bool BossKnight::Initialize()
{
	modelImport("game/res/Enemy01/MV1/enemy_1_.mv1", 2.5f, &_modelInf, RS);
	status = STATUS::WAIT;
	time = 200;
	stanTime = 200;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 10000;
	_statusInf.stanPoint = 0;
	_statusInf.redHitPoint = 0;
	actionFlag = false;
	posFlag = false;
	STABFlag = false;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	g = 3.f;
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3"));
	awake = false;
	awakeDmg = 1;
	awakeMove = 1;
	awakeSpd = 1;
	awakeWaitTime = 0;
	setMasterVolume(120 * (0.01 * _valData->soundMasterValume));
	newSomenHandle = RS->loadGraphR("game/res/new_soumen.png");
	return true;
}

bool	BossKnight::Terminate()
{
	CharBase::Terminate();
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

bool	BossKnight::Process()
{
	//マスター音量の適応
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }

	if (status == STATUS::DEAD)
	{
		animSpd = 0.7f;
		animChange(BOSS1_DEAD, &_modelInf, false, true, false);
		if (isAnimEnd) { isDead = 2; }
		return true;
	}

	if (_statusInf.hitPoint <= 5000)
	{
		awake = true;
		awakeSpd = 1.5f;
		awakeMove = 1.5f;
		awakeDmg = 1.5f;
		awakeWaitTime = 35;
	}

	if (status == STATUS::STAN)
	{
		animChange(BOSS1_DAMAGE, &_modelInf, true, true, false);
		if (stanTime == 0)
		{
			status = STATUS::WAIT;
			stanTime = 100;
			_statusInf.stanPoint = 0;
		}
		else { stanTime--; }
		return true;
	}

	for (auto i = charBox->begin(); i != charBox->end(); i++)
	{
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
		}
	}
	if (CheckHitKey(KEY_INPUT_K))
	{
		_statusInf.hitPoint = 1;
	}

	collCap.r = 120.f;
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
		animChange(BOSS1_IDLE, &_modelInf, true, true, false);
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
		animChange(BOSS1_DAMAGE, &_modelInf, false, true, false);
		actionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = playerDir;
		animSpd = 0.8f * awakeSpd;
		animChange(BOSS1_RUN, &_modelInf, true, true, false);
		Move(runSpd * awakeMove, 0);
		if (playerDistance < 200) { UtilityJudge(); }
		break;
	case STATUS::FSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::FSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_F, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::BSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_B, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 180.0);
		}
		break;
	case STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::RSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_R, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
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
		animChange(BOSS1_DODGE_L, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 270.0);
		}
		break;
	case STATUS::SRASH:
		if (_modelInf.isAnimEnd == true)
		{
			actionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::SRASH) {
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = .7f * awakeSpd;
		animChange(BOSS1_nagiharai, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 50.f, 10.f + 18.f, _modelInf.totalTime - 10.f, animSpd, true, 20.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::SLAM:
		if (isAnimEnd == true)
		{
			actionFlag = false;
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
				if (attackStep == 3 && range == RANGE::MidRange)
				{
					attackStep = 0;
					status = STATUS::JUMPACT;
					break;
				}
				attackStep++;
			}
			else
			{
				UtilityJudge();
				if (status != STATUS::SLAM) { break; }
			}
		}
		if (actionFlag == true) { break; }
		if (attackStep == 1 || attackStep == 4 || attackStep == 7)
		{
			makeAttackCap(VGet(0.f, 60.f, 0.f), VGet(0.f, -100.f, 0.f), 50.f, 0.f, _modelInf.totalTime, animSpd, true, 15.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
			PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		}
		attackStep > 3 ? animSpd = 1.3f * awakeSpd : animSpd = .7f * awakeSpd;
		attackStep > 5 ? insAddNum = 12 : insAddNum = 0;
		animChange(BOSS1_SLAP_R1 + attackStep + insAddNum, &_modelInf, false, false, true);
		actionFlag = true;
		break;
	case STATUS::STAB:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 4) { attackStep++; }
			else
			{
				UtilityJudge();
				if (status != STATUS::STAB) { break; }
			}
		}
		if (actionFlag == true)
		{
			STABFlag = true;
			if (attackStep == 0) { posFlag = true; }
			else if ((attackStep == 2 && _modelInf.playTime > 5) || attackStep == 3) { Move(90.0f * awakeMove, .0f); }
			break;
		}
		if (attackStep == 0) { animSpd = 0.18f * awakeSpd; }
		else { animSpd = 0.7f * awakeSpd; }
		animChange(BOSS1_THRUST1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 3)
		{
			makeAttackCap(VGet(-20.f, 80.f, 0.f), VGet(-20.f, -100.f, 0.f), 100.f, .0f, _modelInf.totalTime, animSpd, true, 30.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
		}
		actionFlag = true;
		break;
	case STATUS::ROBES:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 3) { attackStep++; }
			else
			{
				UtilityJudge();
				if (status != STATUS::ROBES) { break; }
			}
		}
		if (actionFlag == true) { break; }

		animSpd = 0.7f * awakeSpd;
		animChange(BOSS1_kesa1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 2)
		{
			makeAttackCap(VGet(0.f, 50.f, 0.f), VGet(0.f, -120.f, 0.f), 50.f, .0f, _modelInf.totalTime, animSpd, true, 20.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
		}
		actionFlag = true;
		break;
	case STATUS::JUMPACT:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 3)
			{
				if (isGround && attackStep == 2)
				{
					makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 500.f, 5.0f, _modelInf.totalTime - 5, animSpd, true, 50.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
				}
				if (_modelInf.vec.y > 0 && attackStep == 1) { actionFlag = true; }
				else if (!isGround && attackStep == 2) { actionFlag = true; }
				else { attackStep++; }

			}
			else
			{
				UtilityJudge();
				if (status != STATUS::JUMPACT) { break; }
			}
		}
		if (actionFlag == true)
		{
			STABFlag = true;
			if (attackStep == 0) { posFlag = true; }
			if ((attackStep == 1 && _modelInf.playTime > 5) || attackStep == 2)
			{
				if (!jumpActFlag) { _modelInf.vec.y = 45, jumpActFlag = true; }
				Move(playerRange / 40, .0f);
			}
			else if (attackStep == 1 && _modelInf.playTime <= 5) { _modelInf.dir.y = playerDir; RangeJ(); }
			break;
		}
		animSpd = 0.7f * awakeSpd;
		animChange(BOSS1_JUMP_A1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 1) { RangeJ(); }
		actionFlag = true;
		break;
	};

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

VECTOR getDirVec(float dir, int powar)
{
	float _Dir = dir - 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	VECTOR dirZ = VGet(0, 0, 0);
	dirZ.x = sin(radian) * powar;
	dirZ.z = cos(radian) * powar;
	dirZ.y = 11;
	return dirZ;
}

bool	BossKnight::Render(float timeSpeed)
{
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	_modelInf.animHandleOld == BOSS1_RUN ? _modelInf.addPos = VGet(0, 80.f, 0) : _modelInf.addPos = VGet(0, 0, 0);
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);
	return true;
}


bool BossKnight::UtilityJudge()
{
	attackStep = 0, jumpActFlag = false;
	STABFlag = false;
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
				if (Rand < 40) { status = STATUS::SRASH; }
				if (Rand >= 40)
				{
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange)
			{
				if (Rand < 80) { status = STATUS::STAB; }
				if (Rand >= 80) { status = STATUS::RUN; }
			}
			if (range == RANGE::LongRange)
			{
				if (Rand < 70) { status = STATUS::JUMPACT; }
				if (Rand >= 70) { status = STATUS::RUN; }
				break;
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
				if (Rand < 65) { status = STATUS::SRASH; }
				if (Rand >= 65)
				{
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
			if (range == RANGE::LongRange)
			{
				status = STATUS::JUMPACT;
				break;
			}
			break;
		case STATUS::FSTEP:
			if (Rand < 70) { status = STATUS::SRASH; }
			if (Rand >= 70) { status = STATUS::SLAM; }
			break;
		case STATUS::BSTEP:
			RangeJ();
			if (range == RANGE::CrossRange) { status = STATUS::SRASH; break; }
			if (range == RANGE::MidRange) { status = STATUS::STAB; break; }
			if (range == RANGE::LongRange) { status = STATUS::JUMPACT; break; }
			break;
		case STATUS::RSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::LSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::SRASH:
			if (Rand > 80)
			{
				break;
			}
			if (80 >= Rand && Rand > 40)
			{
				status = STATUS::BSTEP;
				break;
			}
			if (40 >= Rand)
			{
				status = STATUS::ROBES;
				break;
			}
			break;
		case STATUS::SLAM:
			status = STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case STATUS::STAB:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::JUMPACT; break; }
			break;
		case STATUS::ROBES:
			status = STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case STATUS::JUMPACT:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::ROBES; break; }
			break;
		case STATUS::ONESLASH:
			status = STATUS::BSTEP; break;
			break;
		};
	}
	else if (awake)
	{
		switch (status)
		{
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand < 40) { status = STATUS::SRASH; }
				if (Rand >= 40)
				{
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange)
			{
				if (Rand < 80) { status = STATUS::STAB; }
				if (Rand >= 80) { status = STATUS::RUN; }
			}
			if (range == RANGE::LongRange)
			{
				if (Rand < 70) { status = STATUS::JUMPACT; }
				if (Rand >= 70) { status = STATUS::RUN; }
				break;
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
				if (Rand < 65) { status = STATUS::SRASH; }
				if (Rand >= 65)
				{
					status = STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
			if (range == RANGE::LongRange)
			{
				status = STATUS::JUMPACT;
				break;
			}
			break;
		case STATUS::FSTEP:
			if (Rand < 70) { status = STATUS::SRASH; }
			if (Rand >= 70) { status = STATUS::SLAM; }
			break;
		case STATUS::BSTEP:
			RangeJ();
			if (range == RANGE::CrossRange) { status = STATUS::SRASH; break; }
			if (range == RANGE::MidRange) { status = STATUS::STAB; break; }
			if (range == RANGE::LongRange) { status = STATUS::JUMPACT; break; }
			break;
		case STATUS::RSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::LSTEP:
			status = STATUS::WAIT;
			break;
		case STATUS::SRASH:
			if (Rand > 80)
			{
				break;
			}
			if (80 >= Rand && Rand > 40)
			{
				status = STATUS::BSTEP;
				break;
			}
			if (40 >= Rand)
			{
				status = STATUS::ROBES;
				break;
			}
			break;
		case STATUS::SLAM:
			status = STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case STATUS::STAB:
			if (Rand < 25) { status = STATUS::FSTEP; break; }
			if (25 <= Rand) { status = STATUS::JUMPACT; break; }
			break;
		case STATUS::ROBES:
			status = STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case STATUS::JUMPACT:
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

bool BossKnight::RangeJ()
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


void BossKnight::Move(float speed, float radian)
{
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;
	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}


bool BossKnight::HPmath(float Num, float Stan)
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
	}
	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);

	if (_statusInf.hitPoint <= 0)
	{
		status = STATUS::DEAD;
	}
	return true;
}
