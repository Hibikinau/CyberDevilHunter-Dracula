#pragma once
#include"appframe.h"

class modeMM : public ModeBase
{
public:
	modeMM(ModeServer* MS) { _modeServer = MS; };
	~modeMM() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();

protected:
	int _cg, DeffontSize, StrWidth, picMenuNum, picMenuMaxNum, windowAnimHandle[29] = { -1 }, windowAnimNum = 0;
	bool isBackTitle = false, isBackTitlePic = true;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::vector<std::string> menuMessage;
};
