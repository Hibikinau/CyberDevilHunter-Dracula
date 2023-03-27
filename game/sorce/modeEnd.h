#pragma once
#include"appframe.h"

class modeE : public ModeBase
{
public:
	modeE(ModeServer* MS) { _modeServer = MS; };
	~modeE() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, logoHandle, logoAlphaNum = 0, titleAnimHandle = -1, titleLogoHandle = -1, isPut = 0, pressAtoStart, waveAlphaNum;
	bool isFirstMovie = true;
	ModeServer* _modeServer;
	imputInf _imputInf;
};
