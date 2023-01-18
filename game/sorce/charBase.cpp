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

	////•Ç‚Æ‚Ì“–‚½‚è”»’è
	//hitCheckWall = MV1CollCheck_Capsule(_GrounfInf->modelHandle, -1, VAdd(collCap.underPos, VGet(0.f, 40.f, 0.f)), collCap.overPos, collCap.r);
	//VECTOR insPos, insDir;
	//if (hitCheckWall.Dim > 0)
	//{
	//	hitCheckWall.Dim->Position
	//	insPos.y = 0.f, insDir.y = 0.f;
	//	_modelInf.pos = VAdd(_modelInf.pos, (VScale(insDir, collCap.r)));
	//}

	//MV1CollResultPolyDimTerminate(hitCheckWall);
	return true;
}