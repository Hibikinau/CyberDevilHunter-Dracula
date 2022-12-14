#include"allMode.h"

bool	modeT::Initialize()
{

	return true;
}

bool	modeT::Process()
{
	if (CheckHitKeyAll())
	{
		_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
		return false;
	}
	return true;
}

bool	modeT::Render()
{
	DrawString(1200, 20, "TITLEmode", GetColor(255, 255, 255));
	return true;
}

bool	modeT::Terminate()
{
	return true;
}