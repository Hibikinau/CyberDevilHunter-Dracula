/*
** WinMain
*/

//
// include 部
//

#include "../appframe.h"



//
// WinMain(). プログラム起動関数
//
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	ApplicationBase *appBase = ApplicationBase::GetInstance();
	ModeServer _modeServer;
	SetOutApplicationLogValidFlag(true);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (!appBase) { return 0; }

	if (!appBase->Initialize(hInstance, &_modeServer)) {
		return 0;
	}

	// 1フレームループを組む ----------------------------------------------------------
	while (ProcessMessage() == 0)		// プログラムが終了するまでループ
	{
		appBase->Input();
		if (!appBase->Process()) { break; }

		ClearDrawScreen();		// 画面を初期化する
		appBase->Render();
		ScreenFlip();			// 裏画面の内容を表画面に反映させる
	}
	appBase->Terminate();

	return 0;
}
