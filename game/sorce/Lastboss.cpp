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
	//weponAttach("game/res/Weapon_Saya/Weapon_saya.mv1", &_modelInf, lWeponParentFrame, 2.f, true, "saya", RS);
	weponAttach("game/res/Weapon_noutou/Weapon_noutou.mv1", &_modelInf, lWeponParentFrame, 2.f, false, "noutou", RS);
	status = STATUS::WAIT;
	time = 200;
	stanTime = 200;
	hittime = 0;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 12000;
	_statusInf.stanPoint = 0;
	hitFlag = false;
	_modelInf.pos = VGet(0.0f, 1100.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	ActionFlag = false;
	g = 3.f;
	swingSE = LoadSoundMem("game/res/SE/BOSS_swing/swing3.mp3");
	impactEfcHandle = LoadEffekseerEffect("game/res/effect/衝撃波1/slash_shot.efkefc", 80.f);
	ChangeVolumeSoundMem(120, swingSE);
	Awake = false;
	AwakeDmg = 1;
	AwakeMove = 1;
	AwakeSpd = 1;
	AwakeT = 0;
	AwakeTK = 0;
	return true;
}

bool	LastBoss::Terminate()
{
	CB::Terminate();
	DeleteSoundMem(swingSE);
	return true;
}

bool	LastBoss::Process()
{
	if (status == STATUS::DEAD) {
		animSpd = 0.7f;
		animChange(PL_death, &_modelInf, false, true, false);
		if (isAnimEnd) { isDead = 2; }
		return true;
	}

	if (_statusInf.hitPoint <= 5000) {
		Awake = true;
		AwakeSpd = 1.5f;
		AwakeMove = 1.5f;
		AwakeDmg = 1.5f;
		AwakeT = 30;
		AwakeTK = 30;
	}

	if (status == STATUS::STAN) {
		if (!ActionFlag) {
			animSpd = 1.0;
			animChange(PL_hirumi, &_modelInf, true, true, false);
			ActionFlag = true;
		}
		if (stanTime == 0) {
			status = STATUS::WAIT;
			time = 0;
			stanTime = 100;
			_statusInf.stanPoint = 0;
			ActionFlag = false;
		}
		else { stanTime--; }
		return true;
	}

	for (auto i = charBox->begin(); i != charBox->end(); i++) {
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
			DT = i->second->dodgeTime;
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
	Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	PrangeA = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);

	int insAddNum = 0;
	bool insFreeBool = false;

	switch (status) {
	case STATUS::NONE:break;
	case STATUS::WAIT:
		animSpd = .5f;
		animChange(Boss_idol1, &_modelInf, true, true, false);
		if (time == 0) {
			UtilityJudge();
		}
		else if (time > 0) {
			time--;
		}
		break;
	case STATUS::DAMAGE:
		if (_modelInf.isAnimEnd == true) {
			ActionFlag = false;
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::DAMAGE) { break; }
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = 1.f;
		animChange(PL_damage, &_modelInf, false, true, false);
		//PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = Pdir;
		animSpd = 1.0f * AwakeSpd;
		animChange(PL_run, &_modelInf, true, true, false);
		Move(runSpd * AwakeMove, 0);
		if (PrangeA < 300) { UtilityJudge(); }
		if (time == 0) { UtilityJudge(); }
		else { time--; }
		break;
	case STATUS::FSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::FSTEP) { break; }
		}
		animSpd = 4.f * AwakeSpd;
		animChange(PL_dodge_F, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * AwakeMove, 0.0);
		}
		break;
	case STATUS::BSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::BSTEP) { break; }
		}
		animSpd = 4.f * AwakeSpd;
		animChange(PL_dodge_B, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * AwakeMove, 180.0);
		}
		break;
	case STATUS::RSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::RSTEP) { break; }
		}
		animSpd = 4.f * AwakeSpd;
		animChange(PL_dodge_R, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * AwakeMove, 90.0);
		}
		break;
	case STATUS::LSTEP:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::LSTEP) { break; }
		}
		animSpd = 4.f * AwakeSpd;
		animChange(PL_dodge_L, &_modelInf, false, true, false);
		if (_modelInf.playTime > 15 && _modelInf.playTime < 48)
		{
			Move(80.0 * AwakeMove, 270.0);
		}
		break;
	case STATUS::kick:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::kick) {
				ActionFlag = false;
				break;
			}
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = 1.5f * AwakeSpd;
		animChange(Boss_kick, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 20.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, true, 20.f * AwakeDmg, 0, 131, VGet(0, 0, 0),1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::kaiten:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::kaiten) {
				ActionFlag = false;
				break;
			}
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = 2.0f * AwakeSpd;
		animChange(Boss_kaiten, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, true, 20.f * AwakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	case STATUS::jumpattack:
		if (_modelInf.isAnimEnd == true) {
			attackStep == 0 ? attackStep++ : UtilityJudge();
			if (status != STATUS::jumpattack) {
				ActionFlag = false;
				break;
			}
		}
		if (ActionFlag == true) {
			break;
		}
		animSpd = 2.0f * AwakeSpd;
		animChange(Boss_jumpattack, &_modelInf, false, true, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, true, 20.f * AwakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		ActionFlag = true;
		break;
	//case STATUS::magicK:
	//	if (_modelInf.isAnimEnd == true) {
	//		attackStep == 0 ? attackStep++ : UtilityJudge();
	//		if (status != STATUS::magicK) {
	//			ActionFlag = false;
	//			break;
	//		}
	//	}
	//	if (ActionFlag == true) {
	//		int a2 = PlayEffekseer3DEffect(impactEfcHandle);
	//		SetPosPlayingEffekseer3DEffect(a2, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
	//		SetRotationPlayingEffekseer3DEffect(a2, _modelInf.dir.x * (DX_PI_F / 180), _modelInf.dir.y * (DX_PI_F / 180), _modelInf.dir.z * (DX_PI_F / 180));
	//		break;
	//	}
	//	animSpd = 1.5f * AwakeSpd;
	//	animChange(Boss_magic1, &_modelInf, false, true, true);
	//	auto a = VAdd(_modelInf.pos, getDirVecP(_modelInf.dir.y - 90, 300));
	//	auto b = VAdd(_modelInf.pos, getDirVecP(_modelInf.dir.y + 90, 300));
	//	auto bz = getDirVecP(_modelInf.dir.y, 30);
	//	a.y = b.y = _modelInf.pos.y + 50;

	//	makeAttackCap(a, b, 40.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, false, 300.f, 100000, -1, bz, 1);
	//	//makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 40.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, true, 20.f * AwakeDmg, 0, 8, VGet(0, 0, 0), 1);
	//	PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	ActionFlag = true;
	//	break;
	//case STATUS::magicR:
	//	if (_modelInf.isAnimEnd == true) {
	//		attackStep == 0 ? attackStep++ : UtilityJudge();
	//		if (status != STATUS::magicR) {
	//		ActionFlag = false;
	//		break;
	//	}
	//	}
	//	if (ActionFlag == true) {
	//		//_modelInf.dir.y = Pdir;
	//		auto a = VAdd(_modelInf.pos, getDirVecP(_modelInf.dir.y - 90, 300));
	//		auto b = VAdd(_modelInf.pos, getDirVecP(_modelInf.dir.y + 90, 300));
	//		auto bz = getDirVecP(_modelInf.dir.y, 30);
	//		a.y = b.y = _modelInf.pos.y + 50;
	//		if (_modelInf.playTime == 60 ) {
	//			_modelInf.dir.y = Pdir;
	//			makeAttackCap(a, b, 40.f, 10.f, _modelInf.totalTime* AwakeSpd - 10.f, animSpd, false, 300.f, 100000, -1, bz, 1);
	//		}
	//		if (_modelInf.playTime == 100 ) {
	//			_modelInf.dir.y = Pdir;
	//			makeAttackCap(a, b, 40.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, false, 300.f, 100000, -1, bz, 1);
	//		}
	//		if (_modelInf.playTime ==140 ) {
	//			_modelInf.dir.y = Pdir;
	//			makeAttackCap(a, b, 40.f, 10.f, _modelInf.totalTime * AwakeSpd - 10.f, animSpd, false, 300.f, 100000, -1, bz, 1);
	//		}
	//		break;
	//	}
	//	animSpd = 1.0f * AwakeSpd;
	//	animChange(Boss_magic2, &_modelInf, false, false, true);
	//	makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 240.f, .0f, _modelInf.totalTime * AwakeSpd, animSpd, true, 50.f * AwakeDmg, 0, 3, VGet(0, 0, 0), 1);
	//	PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	ActionFlag = true;
	//	break;
	//case STATUS::quick:
	//	if (isAnimEnd == true) {
	//		ActionFlag = false;
	//		if (attackStep < 10)
	//		{
	//			attackStep++;
	//		}
	//		else {
	//			UtilityJudge();
	//			if (status != STATUS::quick) { break; }
	//		}
	//	}
	//	if (ActionFlag == true) { break; }
	//	if (attackStep == 0 || attackStep == 2|| attackStep == 4)
	//	{
	//		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime* AwakeSpd - 10.f, animSpd, true, 20.f * AwakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
	//		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	}
	//	//animSpd = 1.0f * AwakeSpd;
	//	animChange(PL_jaku_1 + attackStep , &_modelInf, false, false, true);
	//	//makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 0.f), 240.f, .0f, _modelInf.totalTime * AwakeSpd, animSpd, true, 50.f * AwakeDmg, 0, 3, VGet(0, 0, 0), 1);
	//	//PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	ActionFlag = true;
	//	break;
	//case STATUS::STAB:
	//	if (isAnimEnd == true) {
	//		ActionFlag = false;
	//		if (attackStep < 3) { 
	//			attackStep++; }
	//		else {
	//			UtilityJudge();
	//			if (status != STATUS::STAB) { break; }
	//		}
	//	}
	//	if (ActionFlag == true)
	//	{
	//		
	//		if ((attackStep == 1 && _modelInf.playTime > 5) || attackStep == 3) { Move(90.0f * AwakeMove, .0f); }
	//		break;
	//	}
	//	animSpd = 1.0f * AwakeSpd;
	//	animChange(PL_arts_tsuki_1 +attackStep ,&_modelInf, false, false, true);
	//	if (attackStep == 2)
	//	{
	//		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 10.f, _modelInf.totalTime* AwakeSpd - 10.f, animSpd, true, 20.f * AwakeDmg, 0, rWeponParentFrame, VGet(0, 0, 0), 1);
	//		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	}
	//	ActionFlag = true;
	//	break;
	};

	if (isHit && hittime == 0) {
		hitFlag = true;
		hittime = 50;
	}
	else if (hitFlag) {
		hittime--;
		if (hittime == 0) { hitFlag = false; }
	}

	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	return true;
}

bool	LastBoss::Render(float timeSpeed)
{

	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	//_modelInf.animHandleOld == BOSS2_run ? _modelInf.addPos = VGet(0, 80.f, 0) : _modelInf.addPos = VGet(0, 0, 0);
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);

	return true;
}


bool LastBoss::UtilityJudge() {
	attackStep = 0;
	_modelInf.dir.y = Pdir;
	int Rand = GetRand(100);
	if (!Awake) {
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
			if (range == RANGE::CrossRange) {
			if(Rand>50){ status = STATUS::kick; }
			if(Rand<=50){ status = STATUS::kaiten; }
			}
			if (range == RANGE::MidRange) {
				status = STATUS::FSTEP;
			}
			if (range == RANGE::LongRange) {
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
			/*case STATUS::magicK:
				status = STATUS::magicR;
				break;
			case STATUS::magicR:
				status = STATUS::quick;
				break;
		case STATUS::quick:
				RangeJ();
				status = STATUS::STAB;
				break;
		case STATUS::STAB:
				RangeJ();
				status = STATUS::WAIT;
				time = 100;
				break;*/
				
		}
	}

	
	return true;
}

bool LastBoss::RangeJ() {
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


void LastBoss::Move(float speed, float radian) {
	float Speed = speed;
	float Radian = (_modelInf.dir.y + radian) * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(Radian) * Speed;
	_modelInf.pos.z -= cos(Radian) * Speed;
}


bool LastBoss::HPmath(float Num, float Stan)
{
	_statusInf.hitPoint += Num;
	_statusInf.stanPoint += Stan;
	if (Num <= -200) {
		status = STATUS::DAMAGE;
		ActionFlag = false;
	}
	if (_statusInf.hitPoint <= 5000) {
		Awake = true;
	}
	if (_statusInf.stanPoint >= 150) {
		status = STATUS::STAN;
		_statusInf.stanPoint = 150;
	}
	if (_statusInf.hitPoint <= 0) {
		status = STATUS::DEAD;
	}

	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);

	return true;
}