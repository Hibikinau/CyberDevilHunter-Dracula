#include"allMode.h"

bool	modeM::Initialize()
{
	_cg = MakeGraph(1280, 720);
	GetDrawScreenGraph(0, 0, 1280, 720, _cg);
	_modeServer->disable(MODE_GAME);
	return true;
}

bool	modeM::Process()
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		_modeServer->activate(MODE_GAME);
		return false;
	}
	return true;
}

bool	modeM::Render()
{
	//DrawGraph(0, 0, _cg, false);
	DrawBox(80, 45, 1200, 675, GetColor(255, 255, 255), true);
	DrawString(1150, 60, "MENU", GetColor(0, 0, 255));
	DrawString(580, 360, "Q�L�[�Ŗ߂�", GetColor(0, 0, 255));

	return true;
}

bool	modeM::Terminate()
{
	DeleteGraph(_cg);
	return true;
}