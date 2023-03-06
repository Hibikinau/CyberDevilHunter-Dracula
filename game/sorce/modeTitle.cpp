#include"allMode.h"
#include <fstream>
#include <sstream>

bool modeT::save(const char* dir, valData* _val)
{
	std::vector<std::string> _data;
	fileIO::loadCSV(&_data, dir, false);

	for (int i = 0; i < _data.size(); i++)
	{
		if (_data[i].find("//") != std::string::npos)
		{
			continue;
		}

		if (_data[i] == "����ւ��ZX") { i++; _data[i] = _val->plChangeAttackX; }
		if (_data[i] == "����ւ��ZY") { i++; _data[i] = _val->plChangeAttackY; }
	}
	std::string insStr = "";
	for (auto insData : _data) { insStr += insData + "\n"; }
	std::ofstream ofs(dir);
	ofs << insStr;
	ofs.close();

	return true;
}

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

		if (_data[i] == "����ւ��ZX") { i++; _val->plChangeAttackX = _data[i]; }
		if (_data[i] == "����ւ��ZY") { i++; _val->plChangeAttackY = _data[i]; }
		if (_data[i] == "�G�t�F�N�g�t�F�[�h") { i++; _data[i] == "1" ? _val->isAtkEfcArufa = true : _val->isAtkEfcArufa = false; }
		if (_data[i] == "�{�X1�˗����")
		{
			_val->boss1Inf = ""; i++;
			while (_data[i] != "�����܂�")
			{
				if (_data[i] == "���e") { _val->boss1Inf += "\n"; }
				if (_data[i] == "�T�v") { _val->boss1Inf += "\n"; }
				if (_data[i] == "��V") { _val->boss1Inf += "\n"; }
				_val->boss1Inf += _data[i] + "\n";
				i++;
			}
		}
		if (_data[i] == "�{�X2�˗����")
		{
			_val->boss2Inf = ""; i++;
			while (_data[i] != "�����܂�")
			{
				if (_data[i] == "���e") { _val->boss2Inf += "\n"; }
				if (_data[i] == "�T�v") { _val->boss2Inf += "\n"; }
				if (_data[i] == "��V") { _val->boss2Inf += "\n"; }
				_val->boss2Inf += _data[i] + "\n";
				i++;
			}
		}
		if (_data[i] == "�j���[�X")
		{
			i++;
			while (_data[i] != "�����܂�") { _val->news.emplace_back(_data[i]); i++; }
		}
		if (_data[i] == "����ւ��Z�ꗗ")
		{
			i++;
			while (_data[i] != "�����܂�") { _val->changeAttackList.emplace_back(_data[i]); i++; }
		}

	}

	return true;
}

bool	modeT::Initialize()
{
	//"game/res/ZENRYOKUstage/tsStage.mv1"
	_cg = _modeServer->RS.loadGraphR("game/res/�^�C�g��.png");
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
	return true;
}