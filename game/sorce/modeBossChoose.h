#pragma once
#include"appframe.h"

class modeBC : public ModeBase
{
public:
	modeBC(ModeServer* MS) { _modeServer = MS; };
	~modeBC() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	bool isPic;
	bool pic;
};
