#include"allMode.h"

bool modeT::loadData(const char* dir, valData* _val)
{
	std::vector<std::string> _data;
	fileIO::loadCSV(&_data, dir, false);

	for (int i = 0; i < _data.size(); i++)
	{
		if (_data[i].find("//") != std::string::npos)
		{
			continue;
		}

		if (_data[i] == "入れ替え技X") { i++; _val->plChangeAttackX = _data[i]; }
		if (_data[i] == "入れ替え技Y") { i++; _val->plChangeAttackY = _data[i]; }
		if (_data[i] == "エフェクトフェード") { i++; _data[i] == "1" ? _val->isAtkEfcArufa = true : _val->isAtkEfcArufa = false; }

	}

	return true;
}

bool	modeT::Initialize()
{
	//"game/res/ZENRYOKUstage/tsStage.mv1"
	_cg = LoadGraph("game/res/タイトル.png");
	loadData("game/res/save.csv", &_modeServer->_valData);
	return true;
}

bool	modeT::Process()
{
	if (CheckHitKeyAll())
	{
		//_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
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