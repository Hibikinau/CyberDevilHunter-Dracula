#include"charBase.h"

bool CB::Initialize()
{

	return true;
}

bool	CB::Terminate()
{
	_modelManager->modelDelete(&_modelInf);
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

	hitCheckGround = MV1CollCheck_Line(_GrounfInf->modelHandle, -1, VAdd(_modelInf.pos, VGet(0.f, 140.f, 0.f)), _modelInf.pos);
	if (hitCheckGround.HitFlag)
	{
		_modelInf.pos = VSub(hitCheckGround.HitPosition, VGet(0.f, .1f, 0.f));
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
	if (c > 5000.f)
	{
		_modelInf.pos = VScale(VNorm(a), 5000.f);
	}
	return true;
}

bool	CB::hitCheck(const char* name)
{

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

		if (!insCheckHit && allColl->at(i).capCollOld.r != -1)
		{
			auto insUnderPosOld = allColl->at(i).capCollOld.underPos;
			auto insOverPosOld = allColl->at(i).capCollOld.overPos;
			auto underDis = VAdd(insUnderPos, insUnderPosOld);
			auto overDis = VAdd(insOverPos, insOverPosOld);

			auto tsUnderPos = VScale(underDis, 0.5f);
			auto tsOverPos = VScale(overDis, 0.5f);
			insCheckHit = HitCheck_Capsule_Capsule
			(collCap.underPos, collCap.overPos, collCap.r
				, tsUnderPos
				, tsOverPos
				, allColl->at(i).capColl.r);
		}
		allColl->at(i).capCollOld.underPos = insUnderPos;
		allColl->at(i).capCollOld.overPos = insOverPos;
		allColl->at(i).capCollOld.r = allColl->at(i).capColl.r;

		if (insCheckHit)
		{
			allColl->at(i).activeTimeF = 0.f;
			charBox->at(allColl->at(i).attackChar)->isHit = true;
			attackChar = allColl->at(i).attackChar;
			HPmath(-allColl->at(i).damage);
		}
	}

	return true;
}

bool	CB::makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r
	, int nonActiveTimeF, int activeTimeF, bool isUseMat, float damage, int frameNum, const char* charName)
{
	attackColl acoll;
	acoll.isUseMat = isUseMat;
	acoll.capColl.parentModelHandle = _modelInf.modelHandle;
	acoll.capColl.frameNum = frameNum;
	acoll.capColl.underPos = _underPos;
	acoll.capColl.overPos = _overPos;
	acoll.capColl.r = r;
	acoll.attackChar = charName;
	acoll.activeTimeF = activeTimeF;
	acoll.nonActiveTimeF = nonActiveTimeF;
	acoll.damage = damage;
	acoll.capCollOld.r = -1;
	allColl->emplace_back(acoll);

	return true;
}