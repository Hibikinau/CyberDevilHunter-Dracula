#pragma once
#include"appframe.h"

class modeES : public ModeBase
{
public:
	 modeES(ModeServer* MS) { _modeServer = MS; };
	~modeES() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum = 0, picMenuMaxNum, backAnimNum = 0, backAnimHandle[90] = { -1 }, pic = 0, popSelectNum = 0;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
	bool isPic = false;
};
