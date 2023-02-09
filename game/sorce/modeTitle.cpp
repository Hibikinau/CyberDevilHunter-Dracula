#include"allMode.h"

bool	modeT::Initialize()
{
	//"game/res/ZENRYOKUstage/tsStage.mv1"
	_cg = LoadGraph("game/res/ƒ^ƒCƒgƒ‹.png");
	return true;
}

bool	modeT::Process()
{
	if (CheckHitKeyAll())
	{
		_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
		return false;
	}
	return true;
}

bool	modeT::Render()
{
	DrawGraph(0, 0, _cg, true);
	DrawString(1200, 20, "TITLEmode", GetColor(255, 255, 255));
	return true;
}

bool	modeT::Terminate()
{
	InitGraph();
	return true;
}