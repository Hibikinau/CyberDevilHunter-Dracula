/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  �v���O�����̃G���g���[�|�C���g
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
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
	ApplicationBase* appBase = ApplicationBase::GetInstance();
	ModeServer _modeServer;
	SetOutApplicationLogValidFlag(false);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (!appBase) { return 0; }

	if (!appBase->Initialize(hInstance, &_modeServer)) {
		return 0;
	}

	int FPS = 60;
	int setTime = GetNowCount();//1000=1�b
	bool two = false;
	// 1�t���[�����[�v��g�� ----------------------------------------------------------
	while (true)		// �v���O�������I������܂Ń��[�v
	{
		if (two) { if (ProcessMessage() != 0) { break; } }
		two = !two;
		while (setTime >= GetNowCount()) {};
		setTime = static_cast<int>(1000 / FPS) + GetNowCount();

		appBase->Input();
		if (!appBase->Process()) { break; }
		appBase->Render();
	}
	appBase->Terminate();

	return 0;
}
