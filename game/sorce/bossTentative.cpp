#include"bossTentative.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 2
#define motion_run 2
#define motion_attack1 1
#define motion_dodgeR 3
#define motion_dodgeF 4
#define motion_dodgeL 5
#define motion_dodgeB 6
#define motion_dead 7


bool Boss::Initialize()
{
	_modelManager.modelImport("game/res/Enemy01/MV1/enemy_0_0.mv1", 2.0f, &_modelInf);
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
	float addDir = 0.f;

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

	//bossと距離一定以内行動変更
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
	case STATUS::DEAD:break;
	case STATUS::RUN:
		_modelInf.dir.y = Pdir;
		_modelManager.animChange(motion_run, &_modelInf, true, true,false);
		animSpd = 0.5f;
		Walk();
		if (PrangeA < 150) { UtilityJudge(); }
		/*if(Prange>100) { Walk(); }
		else {
			int j;
		}*/
		//AttackFlag = false;
		break;
	case STATUS::FSTEP:
		_modelManager.animChange(motion_dodgeF, &_modelInf, false, true,true);
		animSpd = 1.0f;
		if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
		{
			Step();
		}
		if (isAnimEnd == true) { UtilityJudge(); }
		break;
	case STATUS::BSTEP:break;
	case STATUS::RSTEP:break;
	case STATUS::LSTEP:break;
	case STATUS::SRASH:
		_modelManager.animChange(motion_attack1, &_modelInf, false, false,true);
		animSpd = 0.7f;
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 40.f, 10.f, _modelInf.totalTime / animSpd + 1, true, 5.f, 100, Char_BOSS1);
		PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
		if (isAnimEnd == true) { 
			UtilityJudge(); }
		break;
	case STATUS::SLAM:break;
	case STATUS::STAB:break;
	case STATUS::JAMPA:break;
	};

	//// ステータスに合わせてアニメーションのアタッチ
	//switch (status) {
	//case STATUS::WAIT:
	//	_modelManager.animChange(motion_idel, &_modelInf, true, true);
	//	animSpd = 0.5f;
	//	break;
	//case STATUS::WALK:
	//	_modelManager.animChange(motion_walk, &_modelInf, true, true);
	//	animSpd = 0.5f;
	//	Walk();
	//	AttackFlag = false;
	//	break;
	//case STATUS::ATTACK:
	//	if (AttackFlag == true) { break; }
	//	AttackFlag = true;
	//	_modelManager.animChange(motion_attack1, &_modelInf, false, false);
	//	animSpd = 0.7f;
	//	makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 40.f, 10.f, _modelInf.totalTime / animSpd + 1, true, 5.f, 112, Char_BOSS1);
	//	PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	break;
	//case STATUS::SRASH:
	//	if (AttackFlag == true) { break; }
	//	AttackFlag = true;
	//	_modelManager.animChange(motion_attack1, &_modelInf, false, false);
	//	animSpd = 0.7f;
	//	makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -100.f, 0.f), 40.f, 10.f, _modelInf.totalTime / animSpd + 1, true, 5.f, 112, Char_BOSS1);
	//	PlaySoundMem(swingSE, DX_PLAYTYPE_BACK);
	//	break;
	//case STATUS::BACK:
	//	_modelManager.animChange(motion_dodgeB, &_modelInf, false, true);
	//	animSpd = 1.0f;
	//	if(_modelInf.playTime > 5 && _modelInf.playTime < 27)
	//	{
	//		BackStep();
	//	}
	//	AttackFlag = false;
	//	break;
	//case STATUS::STEP:
	//	_modelManager.animChange(motion_dodgeF, &_modelInf, false, true);
	//	animSpd = 1.0f;
	//	if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
	//	{
	//		Step();
	//	}
	//	AttackFlag = false;
	//	break;
	//case STATUS::LEFT:
	//	_modelManager.animChange(motion_dodgeL, &_modelInf, false, true);
	//	animSpd = 1.0f;
	//	if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
	//	{
	//		Step();
	//	}
	//	AttackFlag = false;
	//	break;
	//case STATUS::RIGHT:
	//	_modelManager.animChange(motion_dodgeR, &_modelInf, false, true);
	//	if (_modelInf.playTime > 5 && _modelInf.playTime < 27)
	//	{
	//		Step();
	//	}
	//	AttackFlag = false;
	//	break;
	//case STATUS::DEAD:
	//	_modelManager.animChange(motion_dead, &_modelInf, false, true);
	//	break;
	//}


	/*if (isAnimEnd == true) {
		if (status == STATUS::DEAD) { 
			isDead = 2;
			return true;
		}
		status = STATUS::WAIT;
		AttackFlag = false;
		for (auto i = 0; i < 800; i++) {
			if (i == 790) {
				i = 0;
				MotionFlag = true;
				while (time < 30 && !_modelInf.animOldLoop) { time = rand() % 40; }
				break;
			}
		}
	}*/

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

#define Jkkkkkkkk 0

bool Boss::UtilityJudge() {
	//int J = 0;
	//int Wt = 0;
	//int Wk = 0;
	int Rand=GetRand(100);
	switch (status) {
	case STATUS::NONE:
	case STATUS::WAIT:
		RangeJ();
		if (range == RANGE::CrossRange) {
			if (Rand < 40) { status = STATUS::SRASH; }
			if (Rand >= 40) { status = STATUS::SLAM; }
			break;
		}
		if (range == RANGE::MidRange || range == RANGE::LongRange) {
			status = STATUS::RUN;
			break;
		}
		break;
	case STATUS::DEAD:break;
	case STATUS::RUN:
		RangeJ();
		if (range == RANGE::CrossRange) { status = STATUS::SRASH; break; }
		if (range == RANGE::MidRange) { status = STATUS::FSTEP; break; }
		if (range == RANGE::LongRange) { break; }
		break;
	case STATUS::FSTEP:break;
	case STATUS::BSTEP:break;
	case STATUS::RSTEP:break;
	case STATUS::LSTEP:break;
	case STATUS::SRASH:
		if (Rand > 60) { 
			break; }
		if (Rand <= 60) { status = STATUS::BSTEP; break; }
		break;
	case STATUS::SLAM:break;
	case STATUS::STAB:break;
	case STATUS::JAMPA:break;
	};

	/*int J[100] = { 0 };

	if (J[Jkkkkkkkk] > Wt) {
		status = STATUS::WAIT;
	}
	if (J[Jkkkkkkkk] >Wk) {
		status = STATUS::WALK;
	}*/

	time = 1000;

	return true;
}

bool Boss::RangeJ() {
	auto Pvector = VSub(plMI->pos, _modelInf.pos);
	auto Pdir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	float Prange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	if(Prange < 300)
	{
		range = RANGE::CrossRange;
	}
	if (300 <= Prange && Prange <= 400)
	{
		range = RANGE::MidRange;
	}
	if (Prange > 400)
	{
	    range = RANGE::LongRange;
	}
	return true;
}

void Boss::Walk() {
	float Speed = 8.5;
	//auto c = VSub(x, _modelInf.pos);
	//sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	float radian = _modelInf.dir.y * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(radian) * Speed;
	_modelInf.pos.z -= cos(radian) * Speed;

}

void Boss::BackStep() {
	float Speed = 40.0;
	//auto c = VSub(x, _modelInf.pos);
	//sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	float radian = _modelInf.dir.y * DX_PI_F / 180.0f;

	_modelInf.pos.x += sin(radian) * Speed;
	_modelInf.pos.z += cos(radian) * Speed;
}

void Boss::Step() {
	float Speed = 5.0;
	//auto c = VSub(x, _modelInf.pos);
	//sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	float radian = _modelInf.dir.y * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(radian) * Speed;
	_modelInf.pos.z -= cos(radian) * Speed;

}

void Boss::LeftStep() {

}

void Boss::RightStep() {

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
	if (_statusInf.hitPoint <= 0) {
		status = STATUS::DEAD;
	}

	return true;
}
