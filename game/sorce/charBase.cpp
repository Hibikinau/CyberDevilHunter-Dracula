#include"charBase.h"

using namespace model;

bool CB::Initialize()
{

	return true;
}

bool	CB::Terminate()
{
	modelDelete(&_modelInf);
	return true;
}

bool	CB::Process()
{

	return true;
}

bool	CB::Render(float timeSpeed)
{

	return true;
}

bool	CB::gravity()
{
	/*if (_modelInf.pos.y > 0.f) { _modelInf.vec.y -= g, isGround = false; }
	else { _modelInf.pos.y = 0.f, isGround = true;}*/

	//hitCheckGround = MV1CollCheck_Line(_GrounfInf->modelHandle, -1, VAdd(_modelInf.pos, VGet(0.f, 140.f, 0.f)), _modelInf.pos);
	if (_modelInf.pos.y < 1100)
	{
		_modelInf.pos.y = 1090;// = VSub(hitCheckGround.HitPosition, VGet(0.f, .1f, 0.f));
		isGround = true;
		if (_modelInf.vec.y < 0) { _modelInf.vec.y = 0; }
	}
	else
	{
		_modelInf.vec.y -= g;
		isGround = false;
	}

	//マップ(円)の中から出ないように
	auto a = _modelInf.pos;
	float c = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	if (c > 4600.f)
	{
		_modelInf.pos = VScale(VNorm(a), 4600.f);
	}
	return true;
}

bool	CB::hitCheck(const char* name, VECTOR* hitPos, float* damage)
{
	bool _isHit = false;
	for (int i = 0; i < allColl->size(); i++)
	{
		if (allColl->at(i).attackChar == name || allColl->at(i).nonActiveTimeF > 0) { continue; }

		MATRIX M = MV1GetFrameLocalWorldMatrix(allColl->at(i).capColl.parentModelHandle, allColl->at(i).capColl.frameNum);
		auto insUnderPos = VTransform(allColl->at(i).capColl.underPos, M);
		auto insOverPos = VTransform(allColl->at(i).capColl.overPos, M);

		bool insCheckHit = HitCheck_Capsule_Capsule
		(collCap.underPos, collCap.overPos, collCap.r
			, insUnderPos
			, insOverPos
			, allColl->at(i).capColl.r);

		int insNum = VSize(VSub(allColl->at(i).capColl.overPos, allColl->at(i).capColl.underPos));

		for (int j = 0; j < abs(insNum) / allColl->at(i).capColl.r; j++)
		{
			if (allColl->at(i).capCollOld.r == -1) { break; }
			auto insR = allColl->at(i).capColl.r * j;
			auto insS = VSize(allColl->at(i).capColl.overPos);
			auto insN = VNorm(allColl->at(i).capColl.overPos);
			auto insCapNow = VTransform(VScale(insN, insS - insR), M);
			auto insCapOld = VTransform(VScale(insN, insS - insR), allColl->at(i).Mold);

			insCheckHit = HitCheck_Capsule_Capsule
			(collCap.underPos, collCap.overPos, collCap.r
				, insCapNow
				, insCapOld
				, allColl->at(i).capColl.r);
			//auto a = DrawCapsule3D(insCapOld, insCapNow, allColl->at(i).capColl.r, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), false);
			if (insCheckHit)
			{
				*hitPos = VScale(VAdd(insCapNow, insCapOld), 0.5f);
				break;
			}
		}

		allColl->at(i).capCollOld.underPos = insUnderPos;
		allColl->at(i).capCollOld.overPos = insOverPos;
		allColl->at(i).capCollOld.r = allColl->at(i).capColl.r;
		allColl->at(i).Mold = M;

		if (insCheckHit && allColl->at(i).isAlive)
		{
			allColl->at(i).isAlive = false;
			charBox->at(allColl->at(i).attackChar)->isHit = true;
			attackChar = allColl->at(i).attackChar;
			HPmath(-allColl->at(i).damage, allColl->at(i).stan);
			*damage = allColl->at(i).damage;
			_isHit = true;
		}
	}

	return _isHit;
}

bool	CB::makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r
	, int nonActiveTimeF, int activeTimeF, int timeSpeed, bool isUseMat, float damage, float stan, int frameNum, const char* charName)
{
	attackColl acoll;
	acoll.isUseMat = isUseMat;
	acoll.capColl.parentModelHandle = _modelInf.modelHandle;
	acoll.capColl.frameNum = frameNum;
	acoll.capColl.underPos = _underPos;
	acoll.capColl.overPos = _overPos;
	acoll.capColl.r = r;
	acoll.attackChar = charName;
	activeTimeF == 0 || timeSpeed == 0 ? acoll.activeTimeF = activeTimeF : acoll.activeTimeF = activeTimeF / timeSpeed;
	nonActiveTimeF == 0 || timeSpeed == 0 ? acoll.nonActiveTimeF = nonActiveTimeF : acoll.nonActiveTimeF = nonActiveTimeF / timeSpeed;
	acoll.damage = damage;
	acoll.stan = stan;
	acoll.capCollOld.r = -1;
	allColl->emplace_back(acoll);

	return true;
}