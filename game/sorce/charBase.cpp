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

bool	CB::gravity()
{/*
	if (_modelInf.pos.y > 0.f) { _modelInf.vec.y -= g, isGround = false; }
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