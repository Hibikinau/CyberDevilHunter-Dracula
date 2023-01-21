#include"bossTentative.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 1
#define motion_run 2
#define motion_attack1 2


bool Boss::Initialize()
{
	_modelManager.modelImport("game/res/Enemy01/MV1/Enemy01_.mv1", 2.0f, &_modelInf);
	useAnim = 0;

	status = STATUS::WAIT;
	time = 100;
	_statusInf.hitPoint = 10000;

	MO = true;
	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(0.0f, 0.0f, 100.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	// 腰位置の設定
	_colSubY = 40.f;
	Attack = false;
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

	collCap.r = 60.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 60, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 300, 0));

	auto xz = plMI->pos;

	//bossと距離一定以内行動変更
	auto a = VSub(plMI->pos, _modelInf.pos);
	auto b = (std::atan2(-a.x, -a.z) * 180.f) / DX_PI_F;
	float c = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);


	if (MO == true && time == 0) {
		_modelInf.dir.y = b;
		if (c < 200)
		{
			CRange();
		}
		if (200 <= c && c <= 350)
		{
			MRange();
		}
		if (c > 350)
		{
			LRange();
			Walk(xz);
		}
	}
	else if (time > 0)
	{
		time--;
	}


	// ステータスに合わせてアニメーションのアタッチ
	switch (status) {
	case STATUS::WAIT:
		_modelManager.animChange(motion_idel, &_modelInf, true, true);
		spd = 0.f;
		animSpd = 0.5f;
		break;
	case STATUS::WALK:
		_modelManager.animChange(motion_walk, &_modelInf, true, true);
		animSpd = 0.5f;

		Attack = false;
		break;
	case STATUS::KICK:
		if (Attack == true) { break; }
		Attack = true;
		_modelManager.animChange(motion_attack1, &_modelInf, false, false);
		animSpd = 1.0f;
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -120.f, 0.f), 40.f, 10.f, _modelInf.totalTime / animSpd + 1, true, 5.f, 201, Char_BOSS1);
		break;
	case STATUS::SRASH:
		if (Attack == true) { break; }
		Attack = true;
		_modelManager.animChange(motion_attack1, &_modelInf, false, false);
		animSpd = 1.0f;
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, -120.f, 0.f), 40.f, 10.f, _modelInf.totalTime / animSpd + 1 , true, 5.f, 201, Char_BOSS1);

		break;
	case STATUS::BACK:
		_modelManager.animChange(motion_walk, &_modelInf, true, true);
		_modelInf.totalTime = 100;
		animSpd = 0.5f;
		Backwalk(xz);
		Attack = false;
		break;
	}


	if (isAnimEnd == true) {
		status = STATUS::WAIT;
		Attack = false;
		for (auto i = 0; i < 800; i++) {
			if (i == 790) {
				i = 0;
				MO = true;
				while (time < 20) { time = rand() % 40; }
				break;
			}
		}
	}

	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;



	collCap.r = 30.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 30, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 170, 0));

	return true;
}

bool	Boss::Render()
{
	//if (isAnimEnd == true) {
		DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	//}
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd);
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	return true;
}

void Boss::charMove(float Speed, float _Dir)
{

}

bool Boss::step()
{
	return true;
}

void Boss::Walk(VECTOR x) {
	float xz = 7.0;
	auto c = VSub(x, _modelInf.pos);
	//sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	float radian = _modelInf.dir.y * DX_PI_F / 180.0f;

	_modelInf.pos.x -= sin(radian) * xz;
	_modelInf.pos.z -= cos(radian) * xz;

}

void Boss::Backwalk(VECTOR x) {
	float xz = 0.5;
	auto c = VSub(x, _modelInf.pos);
	//sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	float radian = _modelInf.dir.y * DX_PI_F / 180.0f;

	_modelInf.pos.x += sin(radian) * xz;
	_modelInf.pos.z += cos(radian) * xz;
}

void Boss::CRange() {

	int AttackRand = GetRand(100);
	if (AttackRand <= 75) {
		status = STATUS::KICK;

	}
	else if (AttackRand > 75) {
		status = STATUS::BACK;

	}
	MO = false;
	if (allColl->size() == 0)
	{
		attackColl Acoll;
		Acoll.nonActiveTimeF = 0;
		Acoll.activeTimeF = 100;
		Acoll.attackChar = Char_BOSS1;
		Acoll.damage = 20.f;
		Acoll.capColl = collCap;
		allColl->emplace_back(std::move(Acoll));
	}
	return;
}

void Boss::MRange() {
	int AttackRand = GetRand(100);
	if (AttackRand <= 80) {
		status = STATUS::SRASH;
	}
	else if (AttackRand > 80) {

	}
	MO = false;
	return;
}

void Boss::LRange() {
	status = STATUS::WALK;
	return;
}

bool Boss::HPmath(float Num)
{
	_statusInf.hitPoint += Num;

	return true;
}
