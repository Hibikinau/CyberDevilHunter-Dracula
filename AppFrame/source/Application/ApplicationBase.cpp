/*****************************************************************//**
 * \file   ApplicationBase.cpp
 * \brief  アプリケーションのスーパークラスの定義
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include "ApplicationBase.h"

ApplicationBase* ApplicationBase::_lpInstance = NULL;


ApplicationBase::ApplicationBase() {
	_lpInstance = this;
}

ApplicationBase::~ApplicationBase() {
}

bool ApplicationBase::Initialize(HINSTANCE hInstance, ModeServer* ms) {

	// DXライブラリの初期化
	if (AppWindowed()) {
		ChangeWindowMode(false);							// ウィンドウモードに指定する
	}
	SetMainWindowText("CyberDevilHunter-Dracula");
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)
	{	// エラーが起きたら直ちに終了
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット
	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	// 乱数初期化
	srand((unsigned int)time(NULL));

	// モードサーバの初期化
	_serverMode = ms;

	return true;
}

bool ApplicationBase::Terminate() {
	// Effekseerを終了する。
	Effkseer_End();
	// DXライブラリ開放
	DxLib_End();

	return true;
}

int volumeGate4000(int num)
{
	if (std::abs(num) > 4000) { return num; }
	return 0;
}

bool ApplicationBase::Input() {
	//-------------Dpad入力-------------
	//_gTrgp、_gTrgpが変数の方的なアレで動かない
	//int keyold = _imputInf._gKeyp;
	//_imputInf._gTrgp = GetJoypadInputState(DX_INPUT_PAD1);
	//_imputInf._gTrgp = (_imputInf._gKeyp ^ keyold) & _imputInf._gKeyp;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
	//GetLstick(&_imputInf.lStickX, &_imputInf.lStickY);
	//GetRstick(&_imputInf.rStickX, &_imputInf.rStickY);

	//-------------キーボ－ド入力-------------
	char _gTrgbOld[256] = { 0 };
	for (int i = 0; i < 256; i++) { _gTrgbOld[i] = _imputInf._gKeyb[i]; }
	GetHitKeyStateAll(_imputInf._gKeyb);
	for (int i = 0; i < 256; i++)
	{
		_imputInf._gKeyb[i] == 1 && _gTrgbOld[i] != 1 ? _imputInf._gTrgb[i] = 1 : _imputInf._gTrgb[i] = 0;
		_imputInf._gKeyb[i] == 0 && _gTrgbOld[i] != 0 ? _imputInf._gRelb[i] = 1 : _imputInf._gRelb[i] = 0;
	}

	//-------------Xpad入力-------------
	char _gTrgbOldX[16] = { 0 };
	for (int i = 0; i < 16; i++) { _gTrgbOldX[i] = imputInfX.Buttons[i]; }
	GetJoypadXInputState(DX_INPUT_PAD1, &imputInfX);
	for (int i = 0; i < 16; i++) { _imputInf._gKeyp[i] = imputInfX.Buttons[i]; }
	for (int i = 0; i < 16; i++)
	{
		_imputInf._gKeyp[i] == 1 && _gTrgbOldX[i] != 1 ? _imputInf._gTrgp[i] = 1 : _imputInf._gTrgp[i] = 0;
		_imputInf._gKeyp[i] == 0 && _gTrgbOldX[i] != 0 ? _imputInf._gRelp[i] = 1 : _imputInf._gRelp[i] = 0;
	}
	_imputInf.lStickX = volumeGate4000(imputInfX.ThumbLX);
	_imputInf.lStickY = volumeGate4000(imputInfX.ThumbLY);
	_imputInf.rStickX = volumeGate4000(imputInfX.ThumbRX);
	_imputInf.rStickY = volumeGate4000(imputInfX.ThumbRY);
	_imputInf.rTriggerX = imputInfX.RightTrigger;
	_imputInf.lTriggerX = imputInfX.LeftTrigger;

	_serverMode->Imput(&_imputInf);
	return true;
}

bool ApplicationBase::Process() {
	if (_serverMode->_valData.hitstopF > 0) { _serverMode->_valData.hitstopF--; }
	else
	{
		_serverMode->ProcessInit();
		_serverMode->Process();
		_serverMode->ProcessFinish();
	}
	return true;
}

bool ApplicationBase::Render() {
	if (_serverMode->_valData.hitstopF > 0);
	else
	{
		ClearDrawScreen();		// 画面を初期化する
		_serverMode->RenderInit();
		_serverMode->Render();
		_serverMode->RenderFinish();
		ScreenFlip();			// 裏画面の内容を表画面に反映させる
	}
	return true;
}
