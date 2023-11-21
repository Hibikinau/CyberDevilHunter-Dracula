/*****************************************************************//**
 * \file    bossLionman.h
 * \brief   ボス「ライオンマン」クラスの定義
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#include"bossLionman.h"
#include <math.h>
namespace 
{
	constexpr float runSpd = 25.0f;
}
using namespace model;

bool BossLion::Initialize()
{
	BossBase::Initialize();
	modelImport("game/res/Enemy02_mv1/Enemy02.mv1", 2.5f, &_modelInf, RS);
	status = STATUS::WAIT;
	hitTime = 0;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 12000;
	hitFlag = false;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	actionFlag = false;
	Set3DSoundListenerPosAndFrontPos_UpVecY(cameraPosP, cameraForP);
	soundHandle.emplace_back(Load3DSoundMem("game/res/SE/BOSS_swing/swing3.mp3", 2000));
	Set3DPositionSoundMem(_modelInf.pos, soundHandle[0]);
	setMasterVolume(120 * (0.01 * _valData->soundMasterValume));
	return true;
}

bool	BossLion::Terminate()
{
	CharBase::Terminate();
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

bool	BossLion::Process()
{
	BossBase::Process();
	//マスター音量の適応
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	//3Dサウンドの位置設定
	Set3DPositionSoundMem(_modelInf.pos, soundHandle[0]);
	Set3DSoundListenerPosAndFrontPos_UpVecY(plMI->pos, VAdd(_modelInf.pos, VGet(0, 100, 0)));

	//死亡判定
	if (status == STATUS::DEAD)
	{
		animSpd = 0.7f;
		animChange(BOSS2_DEAD, &_modelInf, false, true, false);
		if (_modelInf.isAnimEnd) { isDead = 2; }
		return true;
	}
	//スタン判定
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
	//体力一定以下になると強化
	if (_statusInf.hitPoint <= 5000)
	{
		awake = true;
		awakeSpd = 1.5f;
		awakeMove = 1.5f;
		awakeDmg = 1.5f;
		awakeTime = 30;
		awakeAddDistance = 30;
	}

	collCap.r = 80.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 60, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 300, 0));

	//bossと距離一定以内行動変更
	Pvector = VSub(plMI->pos, _modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerDistance = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);

	int insAddNum = 0;
	bool insFreeBool = false;
	//行動毎の処理部分
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
		animChange(BOSS2_ATTACK_1, &_modelInf, false, true, true);
		makeAttackCap(VGet(-20.f, 0.f, -0.f), VGet(50.f, 0.f, 0.f), 40.f, 28.f, _modelInf.totalTime * awakeSpd - 28.f, animSpd, true, 20.f * awakeDmg, 0, 41, VGet(0, 0, 0), 1);
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
		animChange(BOSS2_ATTACK_2, &_modelInf, false, true, true);
		makeAttackCap(VGet(-20.f, 0.f, 0.f), VGet(50.f, 0.f, 0.f), 40.f, 75.f, _modelInf.totalTime * awakeSpd - 75.f, animSpd, true, 20.f * awakeDmg, 0, 18, VGet(0, 0, 0), 1);
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
		animChange(BOSS2_ATTACK_3, &_modelInf, false, true, true);
		makeAttackCap(VGet(-20.f, 0.f, 0.f), VGet(50.f, 0.f, 0.f), 40.f, 75.f, _modelInf.totalTime * awakeSpd - 75.f, animSpd, true, 20.f * awakeDmg, 0, 41, VGet(0, 0, 0), 1);
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
		PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
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
	playerDir = RangeJ();
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
				break;
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
			break;
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

bool BossLion::HPmath(float Num, float Stan)
{
	BossBase::HPmath(Num, Stan);
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
	return true;
}