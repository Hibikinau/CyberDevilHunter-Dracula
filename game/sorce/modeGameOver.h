#pragma once
#include"appframe.h"

class modeGO : public ModeBase
{
public:
	modeGO(ModeServer* MS) { _modeServer = MS; };
	~modeGO() {};

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
	int gameOverAnimHandle[89] = { -1 };
	float GOAnimNum = 0;
};
