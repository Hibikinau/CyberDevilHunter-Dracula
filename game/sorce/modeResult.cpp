#include"allMode.h"

bool	modeR::Initialize()
{
	//"game/res/ZENRYOKUstage/tsStage.mv1"
	_cg = _modeServer->RS.loadGraphR("game/res/result.png");
	return true;
}

bool	modeR::Process()
{
	if (CheckHitKeyAll())
	{
		_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
	while(CheckHitKeyAll()){
	}
		return false;
	}
	return true;
}

bool	modeR::Render()
{
	DrawGraph(0, 0, _cg, true);
	DrawString(1200, 20, "RESULTmode", GetColor(255, 255, 255));
	return true;
}

bool	modeR::Terminate()
{
	return true;
}