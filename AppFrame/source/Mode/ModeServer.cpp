/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  モードのサーバークラスの定義
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include <cstddef>
#include "../../../DxLib/DxLib.h"
#include "ModeServer.h"


 /// インスタンス
ModeServer* ModeServer::_lpInstance = NULL;

// --------------------------------------------------------------------------
/// @brief コンストラクタ
// --------------------------------------------------------------------------
ModeServer::ModeServer()
{
	_lpInstance = this;
	_uid_count = 1;
	_nowMode.clear();
}

// --------------------------------------------------------------------------
/// @brief
// --------------------------------------------------------------------------
ModeServer::~ModeServer()
{
	Clear();
	_lpInstance = NULL;
}


// 登録はするが、一度メインを回さないといけない
int ModeServer::Add(std::shared_ptr<ModeBase> mode, int layer, const char* name)
{
	mode->_uid = _uid_count;
	_uid_count++;
	mode->_layer = layer;
	mode->_szName = name;
	_vModeAdd.emplace(name, std::move(mode));		// 登録予約

	return _uid_count - 1;
}

// 削除予約
int ModeServer::Del(const char* name)
{

	_vModeDel.emplace_back(name);
	return 0;
}

// 削除＆delete
int ModeServer::Release(const char* name, bool isDel)
{
	if (isDel)
	{
		for (int i = 0; i < _vModeDel.size(); i++)
		{
			if (_vModeDel[i] == name)
			{
				_vModeDel.erase(_vModeDel.begin() + i);
				return 1;
			}
		}
		return 1;
	}
	else
	{
		_vMode.at(name)->Terminate();
		_vMode.at(name).reset();
		_vMode.erase(name);
		disable(name);
		return 1;
	}
	return 0;
}

// 全部削除
void ModeServer::Clear()
{
	OutputDebugString("error!");
	try
	{
		for (auto itr = _vMode.begin(); itr != _vMode.end(); ++itr) {
			if (itr->second != NULL) { itr->second->Terminate(); itr->second.reset(); }
		}
		for (auto iteAdd = _vModeAdd.begin(); iteAdd != _vModeAdd.end(); ++iteAdd) {
			if (iteAdd->second != NULL) { iteAdd->second->Terminate(); iteAdd->second.reset(); }
		}
	}
	catch (std::exception)
	{
		OutputDebugString("error!");
	}
	_vModeAdd.clear();
	_vMode.clear();
	_vModeDel.clear();
	_nowMode.clear();
}

// 削除予約されているか？
bool ModeServer::IsDelRegist(const char* name)
{
	for (int i = 0; i < _vModeDel.size(); i++)
	{
		if (_vModeDel[i] == name) { return true; }
	}
	return false;
}

int ModeServer::activate(const char* name)
{
	if (_vMode.find(name) != _vMode.end() || _vModeAdd.find(name) != _vModeAdd.end())
	{
		_nowMode.emplace_back(name);
	}
	return 0;
}

int ModeServer::disable(const char* name)
{
	for (int i = 0; i < _nowMode.size(); i++)
	{
		if (_nowMode[i] == name) { _nowMode.erase(_nowMode.begin() + i); return 1; }
	}
	return 0;
}

// プロセスを回すための初期化
int ModeServer::ProcessInit()
{
	// 削除予約されていたものを削除
	for (int i = 0; i < _vModeDel.size(); i++)
	{
		Release(_vModeDel[i].c_str(), false);
	}
	_vModeDel.clear();

	// Add()されたものを有効にし、必要であればソートを行う
	if (_vModeAdd.size() > 0) {
		auto ite = _vModeAdd.begin();
		for (; ite != _vModeAdd.end(); ++ite) {
			ite->second->Initialize();
			activate(ite->first.c_str());
			_vMode.emplace(ite->first.c_str(), std::move(ite->second));
		}
		_vModeAdd.clear();

	}

	// スキップ、ポーズの解除
	return 0;
}

int ModeServer::Imput(imputInf* iInf)
{
	for (int i = 0; i < _nowMode.size(); i++)
	{
		if (_vMode.size() == 0 || _nowMode[i].c_str() == "") { continue; }
		if (!IsDelRegist(_nowMode[i].c_str())) {
			_vMode.at(_nowMode[i].c_str())->Imput(iInf);
		}
	}

	return 0;
}

int ModeServer::Process()
{

	// 現在の時間を取得
	unsigned int t = GetNowCount();
	bool pause = false;

	// 指定のクラスのみ処理
	for (int i = 0; i < _nowMode.size(); i++)
	{
		if (_vMode.find(_nowMode[i].c_str()) != _vMode.end()) {
			if (!pause) {
				// 時間経過処理
				_vMode.at(_nowMode[i].c_str())->StepTime(t);
			}

			// Processコール
			if (!_vMode.at(_nowMode[i].c_str())->Process())
			{
				Del(_nowMode[i].c_str());
				//_vMode.erase(nowM.c_str());
			}

			// Modeが無い場合 (Reboot時)
			if (_vMode.empty()) {
				_nowMode.clear();
				return 0;
			}

			if (!pause) {
				// カウンタ処理
				_vMode.at(_nowMode[i].c_str())->StepCount();
			}
		}
	}

	// Effekseerにより再生中のエフェクトを更新する。
	//UpdateEffekseer3D();
	return 0;
}

// プロセスを回した後の後始末
int ModeServer::ProcessFinish() { return 0; }

// 描画を回すための初期化
int ModeServer::RenderInit()
{
	return 0;
}

int ModeServer::Render() {
	// 指定のクラスのみ処理
	for (int i = 0; i < _nowMode.size(); i++)
	{
		if (_vMode.size() == 0 || _nowMode[i].c_str() == "") { continue; }
		if (!IsDelRegist(_nowMode[i].c_str())) {
			_vMode.at(_nowMode[i].c_str())->Render();
		}
	}
	// Effekseerにより再生中のエフェクトを描画する。
	//DrawEffekseer3D();
	return 0;
}

// 描画を回した後の後始末
int ModeServer::RenderFinish() { return 0; }