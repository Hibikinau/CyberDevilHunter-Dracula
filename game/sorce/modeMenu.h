#pragma once
#include"../../AppFrame/source/appframe.h"

class modeM : public ModeBase
{
public:
	modeM(ModeServer* MS) { _modeServer = MS; };
	~modeM() {};

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
};
