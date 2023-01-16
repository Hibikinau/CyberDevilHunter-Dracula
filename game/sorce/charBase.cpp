#include"charBase.h"

bool CB::Initialize()
{

	return true;
}

bool	CB::Terminate()
{
	_modelManager.modelDelete(&_modelInf);
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

VECTOR sortTriangle(MV1_COLL_RESULT_POLY_DIM collHitInf)
{
	VECTOR topVec = VGet(0.f, 0.f, 0.f);
	VECTOR topDir = VGet(0.5f, 100.f, 0.5f);

	for (int i = 0; i < collHitInf.HitNum; i++)
	{
		topDir.y = 0.f;
		VECTOR insDir = collHitInf.Dim->Normal;
		insDir.y = 0.f;
		VNorm(insDir);
		for (int i = 0; i < 3; i++)
		{
			VECTOR insPos;
			insPos.x = collHitInf.Dim->Position[i].x / insDir.x;
			insPos.z = collHitInf.Dim->Position[i].z / insDir.z;
			if ((insPos.x + insPos.y) > (topVec.x / topDir.x + topVec.z / topDir.z))
			{
				topVec = collHitInf.Dim->Position[i];
				topDir = insDir;
			}
		}
	}

	return topVec;
}

bool	CB::gravity()
{/*
	if (_modelInf.pos.y > 0.f) { _modelInf.vec.y -= g, isGround = false; }
	else { _modelInf.pos.y = 0.f, isGround = true;}*/

	//’n–Ê‚Æ‚Ì“–‚½‚è”»’è
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

	//•Ç‚Æ‚Ì“–‚½‚è”»’è
	hitCheckWall = MV1CollCheck_Capsule(_GrounfInf->modelHandle, -1, collCap.underPos, collCap.overPos, collCap.r);
	VECTOR insPos = sortTriangle(hitCheckWall);
	if (insPos.y == 0.f)
	{

	}

	MV1CollResultPolyDimTerminate(hitCheckWall);
	return true;
}