/*****************************************************************//**
 * \file   bossKnight.cpp
 * \brief  ボス「騎士」クラスの定義
 *
 * \author 松田　裕
 * \date  March  2023
 *********************************************************************/
#include"bossKnight.h"
#include <math.h>
using namespace model;
using namespace BOSSKNIGHT;

bool BossKnight::Initialize()
{
	BossBase::Initialize();
	modelImport("game/res/Enemy01/MV1/enemy_1_.mv1", 2.5f, &_modelInf, RS);
	status = K_STATUS::WAIT;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 10000;
	actionFlag = false;
	posFlag = false;
	STABFlag = false;
	awakeWaitTime=0;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	Set3DSoundListenerPosAndFrontPos_UpVecY(cameraPosP, cameraForP);
	soundHandle.emplace_back(Load3DSoundMem("game/res/SE/BOSS_swing/swing3.mp3",2000));
	Set3DPositionSoundMem(_modelInf.pos, soundHandle[0]);
	setMasterVolume(20000 * (0.01 * _valData->soundMasterValume));
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
	BossBase::Process();
	//マスター音量の適応
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	Set3DPositionSoundMem(_modelInf.pos, soundHandle[0]);
	Set3DSoundListenerPosAndFrontPos_UpVecY(cameraPosP, cameraForP);

	if (status == K_STATUS::DEAD)
	{
		animSpd = 0.7f;
		animChange(BOSS1_DEAD, &_modelInf, false, true, false);
		if (_modelInf.isAnimEnd) { isDead = 2; }
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

	if (status == K_STATUS::STAN)
	{
		animChange(BOSS1_DAMAGE, &_modelInf, true, true, false);
		if (stanTime == 0)
		{
			status = K_STATUS::WAIT;
			stanTime = 100;
			_statusInf.stanPoint = 0;
		}
		else { stanTime--; }
		return true;
	}

	collCap.r = 120.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 60, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 300, 0));


	//bossと距離一定以内行動変更
	Pvector = VSub(plMI->pos, _modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerDistance = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);

	int insAddNum = 0;
	bool insFreeBool = false;

	switch (status)
	{
	case K_STATUS::NONE:break;
	case K_STATUS::WAIT:
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
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		break;
	case K_STATUS::DAMAGE:
		if (_modelInf.isAnimEnd == true)
		{
			actionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != K_STATUS::DAMAGE) { break; }
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 1.f;
		animChange(BOSS1_DAMAGE, &_modelInf, false, true, false);
		actionFlag = true;
		break;
	case K_STATUS::DEAD:break;
	case K_STATUS::RUN:
		_modelInf.dir.y = playerDir;
		animSpd = 0.8f * awakeSpd;
		animChange(BOSS1_RUN, &_modelInf, true, true, false);
		Move(runSpd * awakeMove, 0);
		if (playerDistance < 200) { UtilityJudge(); }
		break;
	case K_STATUS::FSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != K_STATUS::FSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_F, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 0.0);
		}
		break;
	case K_STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != K_STATUS::BSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_B, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 180.0);
		}
		break;
	case K_STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != K_STATUS::RSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_R, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 90.0);
		}
		break;
	case K_STATUS::LSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != K_STATUS::LSTEP) { break; }
		}
		animSpd = 1.f * awakeSpd;
		animChange(BOSS1_DODGE_L, &_modelInf, false, true, false);
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Move(40.0 * awakeMove, 270.0);
		}
		break;
	case K_STATUS::SRASH:
		if (_modelInf.isAnimEnd == true)
		{
			actionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != K_STATUS::SRASH) {
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = .7f * awakeSpd;
		animChange(BOSS1_MOWING, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 50.f, 10.f + 18.f, _modelInf.totalTime - 10.f, animSpd, true, 20.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case K_STATUS::SLAM:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 8)
			{
				RangeJ();
				if (attackStep == 1 && (rand() % 100) < 50)
				{
					attackStep++;
					status = K_STATUS::ONESLASH;
					UtilityJudge();
					break;
				}
				if (attackStep == 3 && range == RANGE::MidRange)
				{
					attackStep = 0;
					status = K_STATUS::JUMPACT;
					break;
				}
				attackStep++;
			}
			else
			{
				UtilityJudge();
				if (status != K_STATUS::SLAM) { break; }
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
	case K_STATUS::STAB:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 4) { attackStep++; }
			else
			{
				UtilityJudge();
				if (status != K_STATUS::STAB) { break; }
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
		animChange(BOSS1_THRUST_1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 3)
		{
			makeAttackCap(VGet(-20.f, 80.f, 0.f), VGet(-20.f, -100.f, 0.f), 100.f, .0f, _modelInf.totalTime, animSpd, true, 30.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
			PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		}
		actionFlag = true;
		break;
	case K_STATUS::ROBES:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 3) { attackStep++; }
			else
			{
				UtilityJudge();
				if (status != K_STATUS::ROBES) { break; }
			}
		}
		if (actionFlag == true) { break; }

		animSpd = 0.7f * awakeSpd;
		animChange(BOSS1_CUT_1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 2)
		{
			makeAttackCap(VGet(0.f, 50.f, 0.f), VGet(0.f, -120.f, 0.f), 50.f, .0f, _modelInf.totalTime, animSpd, true, 20.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
			PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
		}
		actionFlag = true;
		break;
	case K_STATUS::JUMPACT:
		if (isAnimEnd == true)
		{
			actionFlag = false;
			if (attackStep < 3)
			{
				if (isGround && attackStep == 2)
				{
					makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 500.f, 5.0f, _modelInf.totalTime - 5, animSpd, true, 50.f * awakeDmg, 0, 100, VGet(0, 0, 0), 1);
					PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
				}
				if (_modelInf.vec.y > 0 && attackStep == 1) { actionFlag = true; }
				else if (!isGround && attackStep == 2) { actionFlag = true; }
				else { attackStep++; }

			}
			else
			{
				UtilityJudge();
				if (status != K_STATUS::JUMPACT) { break; }
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
			else if (attackStep == 1 && _modelInf.playTime <= 5) { _modelInf.dir.y = playerDir; playerDir=RangeJ(); }
			break;
		}
		animSpd = 0.7f * awakeSpd;
		animChange(BOSS1_JUMP_A1 + attackStep - 1, &_modelInf, false, false, true);
		if (attackStep == 1) { playerDir = RangeJ(); }
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
		case K_STATUS::NONE:
		case K_STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand < 40) { status = K_STATUS::SRASH; }
				if (Rand >= 40)
				{
					status = K_STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange)
			{
				if (Rand < 80) { status = K_STATUS::STAB; }
				if (Rand >= 80) { status = K_STATUS::RUN; }
			}
			if (range == RANGE::LongRange)
			{
				if (Rand < 70) { status = K_STATUS::JUMPACT; }
				if (Rand >= 70) { status = K_STATUS::RUN; }
				break;
			}
			break;
		case K_STATUS::DAMAGE:
			status = K_STATUS::WAIT;
			break;
		case K_STATUS::DEAD:break;
		case K_STATUS::RUN:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand < 65) { status = K_STATUS::SRASH; }
				if (Rand >= 65)
				{
					status = K_STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) { status = K_STATUS::FSTEP; break; }
			if (range == RANGE::LongRange)
			{
				status = K_STATUS::JUMPACT;
				break;
			}
			break;
		case K_STATUS::FSTEP:
			if (Rand < 70) { status = K_STATUS::SRASH; }
			if (Rand >= 70) { status = K_STATUS::SLAM; }
			break;
		case K_STATUS::BSTEP:
			RangeJ();
			if (range == RANGE::CrossRange) { status = K_STATUS::SRASH; break; }
			if (range == RANGE::MidRange) { status = K_STATUS::STAB; break; }
			if (range == RANGE::LongRange) { status = K_STATUS::JUMPACT; break; }
			break;
		case K_STATUS::RSTEP:
			status = K_STATUS::WAIT;
			break;
		case K_STATUS::LSTEP:
			status = K_STATUS::WAIT;
			break;
		case K_STATUS::SRASH:
			if (Rand > 80)
			{
				break;
			}
			if (80 >= Rand && Rand > 40)
			{
				status = K_STATUS::BSTEP;
				break;
			}
			if (40 >= Rand)
			{
				status = K_STATUS::ROBES;
				break;
			}
			break;
		case K_STATUS::SLAM:
			status = K_STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case K_STATUS::STAB:
			if (Rand < 25) { status = K_STATUS::FSTEP; break; }
			if (25 <= Rand) { status = K_STATUS::JUMPACT; break; }
			break;
		case K_STATUS::ROBES:
			status = K_STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case K_STATUS::JUMPACT:
			if (Rand < 25) { status = K_STATUS::FSTEP; break; }
			if (25 <= Rand) { status = K_STATUS::ROBES; break; }
			break;
		case K_STATUS::ONESLASH:
			status = K_STATUS::BSTEP; break;
			break;
		};
	}
	else if (awake)
	{
		switch (status)
		{
		case K_STATUS::NONE:
		case K_STATUS::WAIT:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand < 40) { status = K_STATUS::SRASH; }
				if (Rand >= 40)
				{
					status = K_STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange)
			{
				if (Rand < 80) { status = K_STATUS::STAB; }
				if (Rand >= 80) { status = K_STATUS::RUN; }
			}
			if (range == RANGE::LongRange)
			{
				if (Rand < 70) { status = K_STATUS::JUMPACT; }
				if (Rand >= 70) { status = K_STATUS::RUN; }
				break;
			}
			break;
		case K_STATUS::DAMAGE:
			status = K_STATUS::WAIT;
			break;
		case K_STATUS::DEAD:break;
		case K_STATUS::RUN:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand < 65) { status = K_STATUS::SRASH; }
				if (Rand >= 65)
				{
					status = K_STATUS::SLAM;
				}
				break;
			}
			if (range == RANGE::MidRange) { status = K_STATUS::FSTEP; break; }
			if (range == RANGE::LongRange)
			{
				status = K_STATUS::JUMPACT;
				break;
			}
			break;
		case K_STATUS::FSTEP:
			if (Rand < 70) { status = K_STATUS::SRASH; }
			if (Rand >= 70) { status = K_STATUS::SLAM; }
			break;
		case K_STATUS::BSTEP:
			RangeJ();
			if (range == RANGE::CrossRange) { status = K_STATUS::SRASH; break; }
			if (range == RANGE::MidRange) { status = K_STATUS::STAB; break; }
			if (range == RANGE::LongRange) { status = K_STATUS::JUMPACT; break; }
			break;
		case K_STATUS::RSTEP:
			status = K_STATUS::WAIT;
			break;
		case K_STATUS::LSTEP:
			status = K_STATUS::WAIT;
			break;
		case K_STATUS::SRASH:
			if (Rand > 80)
			{
				break;
			}
			if (80 >= Rand && Rand > 40)
			{
				status = K_STATUS::BSTEP;
				break;
			}
			if (40 >= Rand)
			{
				status = K_STATUS::ROBES;
				break;
			}
			break;
		case K_STATUS::SLAM:
			status = K_STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case K_STATUS::STAB:
			if (Rand < 25) { status = K_STATUS::FSTEP; break; }
			if (25 <= Rand) { status = K_STATUS::JUMPACT; break; }
			break;
		case K_STATUS::ROBES:
			status = K_STATUS::WAIT;
			time = 50 - awakeWaitTime;
			break;
		case K_STATUS::JUMPACT:
			if (Rand < 25) { status = K_STATUS::FSTEP; break; }
			if (25 <= Rand) { status = K_STATUS::ROBES; break; }
			break;
		case K_STATUS::ONESLASH:
			status = K_STATUS::BSTEP; break;
			break;
		};
	}
	return true;
}


bool BossKnight::HPmath(float Num, float Stan)
{
	BossBase::HPmath(Num,Stan);
	if (Num <= -200)
	{
		status = K_STATUS::DAMAGE;
		actionFlag = false;
	}
	if (_statusInf.hitPoint <= 5000)
	{
		awake = true;
	}
	if (_statusInf.stanPoint >= 150)
	{
		status = K_STATUS::STAN;
		_statusInf.stanPoint = 150;
	}
	if (_statusInf.hitPoint <= 0)
	{
		status = K_STATUS::DEAD;
	}
	return true;
}
