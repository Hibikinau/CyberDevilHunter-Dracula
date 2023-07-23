#include"targetEnemy.h"

using namespace model;

/**
 * @
 *
 *
 */

bool targetEnemy::Initialize()
{
	_statusInf.maxHitPoint = _statusInf.hitPoint = 20000;
	_modelInf.pos = VGet(0.f, 1100.f, 100.f);
	_modelInf.dir = VGet(0.f, 180.f, 0.f);
	siz = VGet(100.f, 100.f, 100.f);
	g = 3.f;
	type = -1;

	setMasterVolume(120 * (0.01 * _valData->soundMasterValume));

	return true;
}

void targetEnemy::loadEfekseer()
{

	return;
}

bool	targetEnemy::Terminate()
{
	CharBase::Terminate();
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

bool targetEnemy::Process()
{


	return true;
}

bool targetEnemy::Render(float timeSpeed)
{
	drawCube(VAdd(_modelInf.pos, VGet(0.f, siz.y / 2, 0.f)), siz, GetColorU8(255, 0, 0, 255), GetColorU8(0, 0, 0, 255));

	return true;
}

bool targetEnemy::HPmath(float Num, float Stan)
{
	_statusInf.hitPoint += Num;

	if (_statusInf.hitPoint <= 0) {

	}
	return true;
}
