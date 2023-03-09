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

		if (_data[i] == "入れ替え技X") { i++; _data[i] = _val->plChangeAttackX; }
		if (_data[i] == "入れ替え技Y") { i++; _data[i] = _val->plChangeAttackY; }
		if (_data[i] == "所持ポイント") { i++; _data[i] = _val->points; }
		if (_data[i] == "撃破済みBoss") { i++; for (auto boss : _val->deadBoss) { _data[i] = boss, i++; } _data[i] = "ここまで"; }
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

		if (_data[i] == "入れ替え技X") { i++; _val->plChangeAttackX = _data[i]; }
		if (_data[i] == "入れ替え技Y") { i++; _val->plChangeAttackY = _data[i]; }
		if (_data[i] == "エフェクトフェード") { i++; _data[i] == "1" ? _val->isAtkEfcArufa = true : _val->isAtkEfcArufa = false; }
		if (_data[i] == "マスター音量") { i++; _val->soundMasterValume = std::atoi(_data[i].c_str()); }
		if (_data[i] == "ニュース") { i++; while (_data[i] != "ここまで") { _val->news.emplace_back(_data[i]); i++; } }
		if (_data[i] == "入れ替え技一覧") { i++; while (_data[i] != "ここまで") { _val->changeAttackList.emplace_back(_data[i]); i++; } }
		if (_data[i] == "プレイヤーボイスリスト") { i++; while (_data[i] != "ここまで") { _val->playerVoiceList.emplace_back(_data[i]); i++; } }
		if (_data[i] == "撃破済みBoss") { i++; while (_data[i] != "ここまで") { _val->deadBoss.emplace_back(_data[i]); i++; } }
		if (_data[i] == "所持ポイント") { i++; _val->points = std::atoi(_data[i].c_str()); }
		if (_data[i] == "開発者モード") { _val->isDebug = true; }

		if (_data[i] == "ボス1依頼情報")
		{
			_val->boss1Inf = ""; i++;
			while (_data[i] != "ここまで")
			{
				if (_data[i] == "内容") { _val->boss1Inf += "\n"; }
				if (_data[i] == "概要") { _val->boss1Inf += "\n"; }
				if (_data[i] == "報酬") { _val->boss1Inf += "\n"; }
				_val->boss1Inf += _data[i] + "\n";
				i++;
			}
		}

		if (_data[i] == "ボス2依頼情報")
		{
			_val->boss2Inf = ""; i++;
			while (_data[i] != "ここまで")
			{
				if (_data[i] == "内容") { _val->boss2Inf += "\n"; }
				if (_data[i] == "概要") { _val->boss2Inf += "\n"; }
				if (_data[i] == "報酬") { _val->boss2Inf += "\n"; }
				_val->boss2Inf += _data[i] + "\n";
				i++;
			}
		}
	}

	return true;
}

bool	modeT::Initialize()
{
	//"game/res/ZENRYOKUstage/tsStage.mv1"
	_cg = _modeServer->RS.loadGraphR("game/res/タイトル.png");
	logoHandle = _modeServer->RS.loadGraphR("game/res/AMG-LOGO.png");
	titleAnimHandle = _modeServer->RS.loadGraphR("game/res/titleMovie.mp4");
	titleLogoHandle = _modeServer->RS.loadGraphR("game/res/Titlelogo1.png");
	loadData("game/res/save.csv", &_modeServer->_valData);
	return true;
}//JNATHYN_-_Dioma_Demo_NCS_Release

bool	modeT::Process()
{
	if (!CheckMusic()) { PlayMusic("game/res/BGM/JNATHYN_-_Dioma_Demo_NCS_Release.mp3", DX_PLAYTYPE_BACK); }

	if (isPut == 1 && !CheckHitKeyAll() || isPut == 0 && !_modeServer->_valData.isLogoRender) { isPut = 2; }
	if (CheckHitKeyAll() && !_modeServer->_valData.isLogoRender && isPut == 2)
	{
		//_modeServer->Add(std::make_unique<modeG>(_modeServer), 1, MODE_GAME);
		_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
		return false;
	}
	return true;
}

bool	modeT::Render()
{

	if (_modeServer->_valData.isLogoRender)
	{//logoAlphaNum

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - std::abs(255 - logoAlphaNum));
		auto i = DrawRotaGraph(640, 360, 1, 0, logoHandle, true, false);
		if (logoAlphaNum < 510) { logoAlphaNum += 2; }
		else { _modeServer->_valData.isLogoRender = false; SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }
		if (CheckHitKeyAll()) { _modeServer->_valData.isLogoRender = false, isPut++; SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }
	}
	else
	{
		if (isFirstMovie) { PlayMovieToGraph(titleAnimHandle);		isFirstMovie = false; }
		if (GetMovieStateToGraph(titleAnimHandle) == 0) { SeekMovieToGraph(titleAnimHandle, 2870); PlayMovieToGraph(titleAnimHandle); }
		DrawExtendGraph(0, 0, 1280, 720, titleAnimHandle, FALSE);
		DrawGraph(280, 400, titleLogoHandle, true);
		//DrawString(1200, 20, "TITLEmode", GetColor(255, 255, 255));
	}
	return true;
}

bool	modeT::Terminate()
{
	return true;
}