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

bool	CB::Render()
{

	return true;
}

//bool sortTriangle(MV1_COLL_RESULT_POLY_DIM collHitInf, VECTOR *_topVec, VECTOR* _topDir)
//{
//	VECTOR topVec = VGet(0.f, 0.f, 0.f);
//	VECTOR topDir = VGet(0.5f, 100.f, 0.5f);
//	bool ret = false;
//	for (int i = 0; i < collHitInf.HitNum; i++)
//	{
//		ret = true;
//		topDir.y = 0.f;
//		VECTOR insDir = collHitInf.Dim->Normal;
//		insDir.y = 0.f;
//		insDir = VNorm(insDir);
//		for (int i = 0; i < 3; i++)
//		{
//			VECTOR insPos ;
//			insPos.x = collHitInf.Dim->Position[i].x / insDir.x;
//			insPos.z = collHitInf.Dim->Position[i].z / insDir.z;
//			if ((insPos.x + insPos.z) > (topVec.x / topDir.x + topVec.z / topDir.z))
//			{
//				*_topVec = collHitInf.Dim->Position[i];
//				*_topDir = insDir;
//			}
//		}
//	}
//
//	return ret;
//}

bool	CB::gravity()
{
	return true;
	/*if (_modelInf.pos.y > 0.f) { _modelInf.vec.y -= g, isGround = false; }
	else { _modelInf.pos.y = 0.f, isGround = true;}*/

	hitCheckGround = MV1CollCheck_Line(_GrounfInf->modelHandle, -1, _modelInf.pos, VAdd(_modelInf.pos, VGet(0.f, 40.f, 0.f)));
	if (hitCheckGround.HitFlag)
	{
		_modelInf.pos = VSub(hitCheckGround.HitPosition, VGet(0.f, 1.f, 0.f));
		isGround = true;
	}
	else
	{
		_modelInf.vec.y -= g;
		isGround = false;
	}

	return true;
}

bool	CB::hitCheck(const char* name)
{

	for (int i = 0; i < allColl->size(); i++)
	{
		if (allColl->at(i).attackChar == name || allColl->at(i).nonActiveTimeF > 0) { continue; }

		MATRIX M = MV1GetFrameLocalWorldMatrix(allColl->at(i).capColl.parentModelHandle, allColl->at(i).capColl.frameNum);

		bool insCheckHit = HitCheck_Capsule_Capsule
		(collCap.underPos, collCap.overPos, collCap.r
			, VTransform(allColl->at(i).capColl.underPos, M)
			, VTransform(allColl->at(i).capColl.overPos, M)
			, allColl->at(i).capColl.r);

		if (insCheckHit)
		{
			allColl->at(i).activeTimeF = 0.f;
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

	allColl->emplace_back(acoll);

	return true;
}