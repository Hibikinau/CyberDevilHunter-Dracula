/*
** WinMain
*/

//
// include ��
//

#include "../appframe.h"



//
// WinMain(). �v���O�����N���֐�
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

	// 1�t���[�����[�v��g�� ----------------------------------------------------------
	while (ProcessMessage() == 0)		// �v���O�������I������܂Ń��[�v
	{
		appBase->Input();
		if (!appBase->Process()) { break; }

		ClearDrawScreen();		// ��ʂ�����������
		appBase->Render();
		ScreenFlip();			// ����ʂ̓��e��\��ʂɔ��f������
	}
	appBase->Terminate();

	return 0;
}
