/*****************************************************************//**
* \file    Lastboss.h
* \brief   ラスボスクラスの定義
*
* \author  松田　裕
* \date    March 2023
********************************************************************/
#include"Lastboss.h"
#include <math.h>
#define runSpd 40.f

//武器追従フレーム番号設定
#define rWeponParentFrame 190
#define lWeponParentFrame 165
using namespace model;

bool LastBoss::Initialize()
{
	//モデルの読み込み
	modelImport("game/res/Enemy03/Enemy03.mv1", 1.8f, &_modelInf, RS);
	weponAttach("game/res/Weapon_Katana/Weapon_katana.mv1", &_modelInf, rWeponParentFrame, 2.f, true, "katana", RS);
	weponAttach("game/res/Weapon_noutou/Weapon_noutou.mv1", &_modelInf, lWeponParentFrame, 2.f, false, "noutou", RS);
	status = STATUS::WAIT;
	time = 200;
	stanTime = 200;
	hitTime = 0;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 12000;
	_statusInf.stanPoint = 0;
	hitFlag = false;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	actionFlag = false;
	g = 3.f;
	swingSE = LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3");
	ChangeVolumeSoundMem(120, swingSE);

	//音声データの読み込み
	voiceStartNum = soundHandle.size();
	for (std::string voiceNameList : _valData->bossVoiceList)
	{
		std::string insName = "game/res/voice/boss_voice/" + voiceNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}

	awake = false;
	awakeDmg = 1;
	awakeMove = 1;
	awakeSpd = 1;
	awakeTime = 0;
	awakeAddDistance = 0;
	return true;
}

bool	LastBoss::Terminate()
{
	CharBase::Terminate();
	DeleteSoundMem(swingSE);
	return true;
}

bool	LastBoss::Process()
{
	//マスター音量の適応
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }

	if (status == STATUS::DEAD) 
	{
		animSpd = 0.7f;
		animChange(PL_death, &_modelInf, false, true, false);
		if (isAnimEnd) { isDead = 2; }
		return true;
	}

	if (_statusInf.hitPoint <= 5000) 
	{
		awake = true;
		awakeSpd = 1.5f;
		awakeDmg = 1.5f;
		awakeMove = 1.5f;
		awakeTime = 30;
		awakeAddDistance = 30;
	}

	if (status == STATUS::STAN) 
	{
		if (!actionFlag) 
		{
			animSpd = 1.0;
			animChange(PL_hirumi, &_modelInf, true, true, false);
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
		animChange(Boss_idol1, &_modelInf, true, true, false);
		if (time == 300)
		{
			PlaySoundMem(soundHandle[voiceStartNum + rand() % 8], DX_PLAYTYPE_BACK);
		}
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
		animChange(PL_damage, &_modelInf, false, true, false);
		actionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = playerDir;
		animSpd = 1.0f * awakeSpd;
		animChange(PL_run, &_modelInf, true, true, false);
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
		animSpd = 4.f * awakeSpd;
		animChange(PL_dodge_F, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * awakeMove, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::BSTEP) { break; }
		}
		animSpd = 4.f * awakeSpd;
		animChange(PL_dodge_B, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * awakeMove, 180.0);
		}
		break;
	case STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::RSTEP) { break; }
		}
		animSpd = 4.f * awakeSpd;
		animChange(PL_dodge_R, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * awakeMove, 90.0);
		}
		break;
	case STATUS::LSTEP:
		if (_modelInf.isAnimEnd == true)
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::LSTEP) { break; }
		}
		animSpd = 4.f * awakeSpd;
		animChange(PL_dodge_L, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * awakeMove, 270.0);
		}
		break;
	case STATUS::kick:
		if (_modelInf.isAnimEnd == true) 
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::kick) {
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true)
		{
			break;
		}
		animSpd = 1.5f * awakeSpd;
		animChange(Boss_kick, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 20.f, 25.f, _modelInf.totalTime * awakeSpd - 25.f, animSpd, true, 20.f * awakeDmg, 0, 131, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::kaiten:
		if (_modelInf.isAnimEnd == true) 
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::kaiten) {
				actionFlag = false;
				break;
			}
		}
		if (actionFlag == true) 
		{
			break;
		}
		animSpd = 2.0f * awakeSpd;
		animChange(Boss_kaiten, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime * awakeSpd - 10.f, animSpd, true, 20.f * awakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::jumpattack:
		if (_modelInf.isAnimEnd == true) 
		{
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::jumpattack) 
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
		animChange(Boss_jumpattack, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime * awakeSpd - 10.f, animSpd, true, 20.f * awakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		actionFlag = true;
		break;
	case STATUS::quick:
		if (isAnimEnd == true) 
		{
			actionFlag = false;
			if (attackStep < 10)
			{
				attackStep++;
			}
			else
			{
				UtilityJudge();
				if (status != STATUS::quick) { break; }
			}
		}
		if (attackStep == 0 || attackStep == 2 || attackStep == 4 || attackStep == 8)
		{
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime * awakeSpd - 10.f, animSpd, true, 20.f * awakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
			PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		}
		if (actionFlag == true) { break; }
		animSpd = 3.0f * awakeSpd;
		animChange(PL_jaku_1 + attackStep, &_modelInf, false, false, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 10.f, .0f, _modelInf.totalTime * awakeSpd, animSpd, true, 50.f * awakeDmg, 0, 3, VGet(0, 0, 0), 1);
		actionFlag = true;
		break;
	case STATUS::STAB:
		if (isAnimEnd == true) 
		{
			actionFlag = false;
			if (attackStep < 3) 
			{
				attackStep++;
			}
			else 
			{
				UtilityJudge();
				if (status != STATUS::STAB) { break; }
			}
		}
		if (actionFlag == true)
		{

			if (attackStep == 2)
			{
				if (_modelInf.playTime > 5) { Move(90.0f * awakeMove, .0f); }
			}
			if (attackStep == 3) {
				if (_modelInf.playTime > 5) { Move(30.0f * awakeMove, .0f); }
			}
			break;
		}
		if (attackStep == 1) 
		{
			animSpd = 2.0f * awakeSpd;
		}
		else if (attackStep == 2) { animSpd = 3.5f * awakeSpd; }
		else { animSpd = 1.2f * awakeSpd; }
		animChange(Boss_arts_tsuki_1 + attackStep, &_modelInf, false, false, true);
		if (attackStep == 2 || attackStep == 3)
		{
			_modelInf.dir.y = playerDir;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime * awakeSpd - 10.f, animSpd, true, 20.f * awakeMove, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
			PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		}
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
		PlaySoundMem(soundHandle[voiceStartNum + 8 + rand() % 7], DX_PLAYTYPE_BACK);
		isHit = false;
	}
	return true;
}

bool	LastBoss::Render(float timeSpeed)
{
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);
	return true;
}


bool LastBoss::UtilityJudge() 
{
	attackStep = 0;
	_modelInf.dir.y = playerDir;
	int Rand = GetRand(100);
	if (!awake) {
		switch (status) {
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			status = STATUS::RUN;
			time = 300;
			break;
		case STATUS::DAMAGE:
			status = STATUS::WAIT;
			break;
		case STATUS::DEAD:break;
		case STATUS::RUN:
			RangeJ();
			if (range == RANGE::CrossRange) 
			{
				if (Rand > 50) { status = STATUS::kick; }
				if (Rand <= 50) { status = STATUS::kaiten; }
			}
			if (range == RANGE::MidRange)
			{
				status = STATUS::FSTEP;
			}
			if (range == RANGE::LongRange) 
			{
				status = STATUS::RUN;
			}
			break;
		case STATUS::FSTEP:
			status = STATUS::kaiten;
			break;
		case STATUS::BSTEP:
			status = STATUS::jumpattack;
			break;
		case STATUS::RSTEP:
			status = STATUS::BSTEP;
			break;
		case STATUS::LSTEP:
			status = STATUS::FSTEP;;
			break;
		case STATUS::kick:
			status = STATUS::BSTEP;
			break;
		case STATUS::kaiten:
			if (Rand > 50) { status = STATUS::LSTEP; }
			if (Rand <= 50) { status = STATUS::RSTEP; }
			break;
		case STATUS::jumpattack:
			RangeJ();
			status = STATUS::quick;
			time = 50;
			break;
		case STATUS::quick:
			RangeJ();
			status = STATUS::STAB;
			break;
		case STATUS::STAB:
			RangeJ();
			status = STATUS::WAIT;
			time = 100;
			break;

		}
	}
	if (awake) 
	{
		switch (status) 
		{
		case STATUS::NONE:
		case STATUS::WAIT:
			RangeJ();
			status = STATUS::RUN;
			time = 300;
			break;
		case STATUS::DAMAGE:
			status = STATUS::WAIT;
			break;
		case STATUS::DEAD:break;
		case STATUS::RUN:
			RangeJ();
			if (range == RANGE::CrossRange)
			{
				if (Rand > 50) { status = STATUS::kick; }
				if (Rand <= 50) { status = STATUS::kaiten; }
			}
			if (range == RANGE::MidRange)
			{
				status = STATUS::FSTEP;
			}
			if (range == RANGE::LongRange) 
			{
				status = STATUS::RUN;
			}
			break;
		case STATUS::FSTEP:
			status = STATUS::kaiten;
			break;
		case STATUS::BSTEP:
			status = STATUS::jumpattack;
			break;
		case STATUS::RSTEP:
			status = STATUS::BSTEP;
			break;
		case STATUS::LSTEP:
			status = STATUS::FSTEP;;
			break;
		case STATUS::kick:
			status = STATUS::BSTEP;
			break;
		case STATUS::kaiten:
			if (Rand > 50) { status = STATUS::LSTEP; }
			if (Rand <= 50) { status = STATUS::RSTEP; }
			break;
		case STATUS::jumpattack:
			RangeJ();
			status = STATUS::WAIT;
			time = 50;
			break;
		case STATUS::quick:
			RangeJ();
			status = STATUS::STAB;
			break;
		case STATUS::STAB:
			RangeJ();
			status = STATUS::WAIT;
			time = 100;
			break;
		}
	}

	return true;
}

bool LastBoss::RangeJ() 
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


void LastBoss::Move(float speed, float radian)
{
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}


bool LastBoss::HPmath(float Num, float Stan)
{
	_statusInf.hitPoint += Num;
	_statusInf.stanPoint += Stan;
	if (Num <= -200)
	{
		status = STATUS::DAMAGE;
		actionFlag = false;
		PlaySoundMem(soundHandle[voiceStartNum + 15 + rand() % 3], DX_PLAYTYPE_BACK);
	}
	if (_statusInf.hitPoint <= 5000) 
	{
		awake = true;
	}
	if (_statusInf.stanPoint >= 150) 
	{
		status = STATUS::STAN;
		_statusInf.stanPoint = 150;
		PlaySoundMem(soundHandle[voiceStartNum + 18 + rand() % 3], DX_PLAYTYPE_BACK);
	}
	if (_statusInf.hitPoint <= 0)
	{
		status = STATUS::DEAD;
	}

	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);

	return true;
}