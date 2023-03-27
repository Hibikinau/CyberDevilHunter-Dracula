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
	int _cg, creditsY, DeffontSize;
	bool isFirstMovie = true;
	ModeServer* _modeServer;
	imputInf _imputInf;
};
