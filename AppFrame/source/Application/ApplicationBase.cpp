/*****************************************************************//**
 * \file   ApplicationBase.cpp
 * \brief  �A�v���P�[�V�����̃X�[�p�[�N���X�̒�`
 *
 * \author �R�{介v
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

	// DX���C�u�����̏�����
	if (AppWindowed()) {
		ChangeWindowMode(false);							// �E�B���h�E���[�h�Ɏw�肷��
	}
	SetMainWindowText("CyberDevilHunter-Dracula");
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)
	{	// �G���[���N�����璼���ɏI��
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// �`����ʂ𗠉�ʂɃZ�b�g
	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	// ����������
	srand((unsigned int)time(NULL));

	// ���[�h�T�[�o�̏�����
	_serverMode = ms;

	return true;
}

bool ApplicationBase::Terminate() {
	// Effekseer���I������B
	Effkseer_End();
	// DX���C�u�����J��
	DxLib_End();

	return true;
}

int volumeGate4000(int num)
{
	if (std::abs(num) > 4000) { return num; }
	return 0;
}

bool ApplicationBase::Input() {
	//-------------Dpad����-------------
	//_gTrgp�A_gTrgp���ϐ��̕��I�ȃA���œ����Ȃ�
	//int keyold = _imputInf._gKeyp;
	//_imputInf._gTrgp = GetJoypadInputState(DX_INPUT_PAD1);
	//_imputInf._gTrgp = (_imputInf._gKeyp ^ keyold) & _imputInf._gKeyp;	// �L�[�̃g���K��񐶐��i�������u�Ԃ����������Ȃ��L�[���j
	//GetLstick(&_imputInf.lStickX, &_imputInf.lStickY);
	//GetRstick(&_imputInf.rStickX, &_imputInf.rStickY);

	//-------------�L�[�{�|�h����-------------
	char _gTrgbOld[256] = { 0 };
	for (int i = 0; i < 256; i++) { _gTrgbOld[i] = _imputInf._gKeyb[i]; }
	GetHitKeyStateAll(_imputInf._gKeyb);
	for (int i = 0; i < 256; i++)
	{
		_imputInf._gKeyb[i] == 1 && _gTrgbOld[i] != 1 ? _imputInf._gTrgb[i] = 1 : _imputInf._gTrgb[i] = 0;
		_imputInf._gKeyb[i] == 0 && _gTrgbOld[i] != 0 ? _imputInf._gRelb[i] = 1 : _imputInf._gRelb[i] = 0;
	}

	//-------------Xpad����-------------
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
		ClearDrawScreen();		// ��ʂ�����������
		_serverMode->RenderInit();
		_serverMode->Render();
		_serverMode->RenderFinish();
		ScreenFlip();			// ����ʂ̓��e��\��ʂɔ��f������
	}
	return true;
}
