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
{
	if (_modelInf.pos.y > 0.f) { _modelInf.vec.y -= g, isGround = false; }
	else { _modelInf.pos.y = 0.f, isGround = true;}
	return true;
}