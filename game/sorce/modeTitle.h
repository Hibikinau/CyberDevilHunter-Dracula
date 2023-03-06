#pragma once
#include"appframe.h"

class modeT : public ModeBase
{
public:
	modeT(ModeServer* MS) { _modeServer = MS; };
	~modeT() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();
	static bool loadData(const char* dir, valData* _val);
	static bool save(const char* dir, valData* _val);
protected:
	int _cg;
	ModeServer* _modeServer;
	imputInf _imputInf;
};
