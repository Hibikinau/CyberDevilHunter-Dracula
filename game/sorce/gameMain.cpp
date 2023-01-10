#include<vector>
#include<memory>
#include"gameMain.h"
#include"allMode.h"
using namespace DxLib;

// ŽÀ‘Ì
gameMain				appFrame;

bool gameMain::Initialize(HINSTANCE hInstance, ModeServer* ms) {
	if (!base::Initialize(hInstance, ms)) { return false; }
	_modeServer = ms;
	_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);

	return true;
}

bool gameMain::Terminate() {
	_modeServer->Clear();
	base::Terminate();
	return true;
}


bool gameMain::Input() {
	base::Input();
	return true;
}

bool gameMain::Process() {
	base::Process();

	if (_imputInf._gKeyb[KEY_INPUT_ESCAPE] || _imputInf._gKeyp[XINPUT_BUTTON_BACK] == 1) { return false; }
	return true;
}

bool gameMain::Render() {
	base::Render();

	return true;
}
