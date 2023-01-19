#include"allMode.h"

bool	modeM::Initialize()
{
	_cg = MakeGraph(1280, 720);
	GetDrawScreenGraph(0, 0, 1280, 720, _cg);
	_modeServer->disable(MODE_GAME);
	SetDragFileValidFlag(TRUE);
	return true;
}

bool	modeM::Process()
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		_modeServer->activate(MODE_GAME);
		return false;
	}

	size_t size = 0;
	TCHAR* path = NULL;

	
	
	while ((size = GetDragFilePath(NULL)) != -1) {
		if (!(path = (TCHAR*)malloc(size))) { return -1; }
		GetDragFilePath(path, size);
		printf("%s\n", path);
		free(path);
	}
	return true;
}

bool	modeM::Render()
{
	//DrawGraph(0, 0, _cg, false);
	DrawBox(80, 45, 1200, 675, GetColor(255, 255, 255), true);
	DrawString(1150, 60, "MENU", GetColor(0, 0, 255));
	DrawString(580, 360, "QÉLÅ[Ç≈ñﬂÇÈ", GetColor(0, 0, 255));

	return true;
}

bool	modeM::Terminate()
{
	DeleteGraph(_cg);
	return true;
}