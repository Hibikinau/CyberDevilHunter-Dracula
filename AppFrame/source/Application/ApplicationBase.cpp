
#include "ApplicationBase.h"

ApplicationBase* ApplicationBase::_lpInstance = NULL;


ApplicationBase::ApplicationBase() {
	_lpInstance = this;
}

ApplicationBase::~ApplicationBase() {
}

bool ApplicationBase::Initialize(HINSTANCE hInstance, ModeServer *ms) {

	// DXライブラリの初期化
	if (AppWindowed()) {
		ChangeWindowMode(true);							// ウィンドウモードに指定する
	}
	SetMainWindowText("kojinGAME");
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	if (DxLib_Init() == -1)
	{	// エラーが起きたら直ちに終了
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット

	// 乱数初期化
	srand((unsigned int)time(NULL));

	// モードサーバの初期化
	_serverMode = ms;

	return true;
}

bool ApplicationBase::Terminate() {
	// DXライブラリ開放
	DxLib_End();

	return true;
}


bool ApplicationBase::Input() {
	// キーの入力、トリガ入力を得る
	int keyold = _imputInf._gKeyp;
	_imputInf._gKeyp = GetJoypadInputState(DX_INPUT_PAD1);
	_imputInf._gTrgp = (_imputInf._gKeyp ^ keyold) & _imputInf._gKeyp;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）

	char _gTrgbOld[256] = { 0 };
	for (int i = 0; i < 256; i++) { _gTrgbOld[i] = _imputInf._gKeyb[i]; }
	GetHitKeyStateAll(_imputInf._gKeyb);
	for (int i = 0; i < 256; i++)
	{
		_imputInf._gKeyb[i] == 1 && _gTrgbOld[i] != 1 ? _imputInf._gTrgb[i] = 1 : _imputInf._gTrgb[i] = 0;
	}
	_serverMode->Imput(&_imputInf);

	GetLstick(&_imputInf.lStickX, &_imputInf.lStickY);
	GetRstick(&_imputInf.rStickX, &_imputInf.rStickY);
	return true;
}

bool ApplicationBase::Process() {
	_serverMode->ProcessInit();
	_serverMode->Process();
	_serverMode->ProcessFinish();
	return true;
}

bool ApplicationBase::Render() {
	_serverMode->RenderInit();
	_serverMode->Render();
	_serverMode->RenderFinish();
	return true;
}
