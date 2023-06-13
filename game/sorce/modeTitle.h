/*****************************************************************//**
 * \file   modeTitle.h
 * \brief  タイトル画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
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
	int _cg, logoHandle, logoAlphaNum = 0, titleAnimHandle = -1, titleLogoHandle = -1, isPut = 0, pressAtoStart, waveAlphaNum;
	bool isFirstMovie = true;
	ModeServer* _modeServer;
	imputInf _imputInf;
	std::string titleCallList[7] = { {"player/player_voice_15_A.mp3"},{"player/player_voice_15_B.mp3"}, {"player/player_voice_15_C.mp3"}
		,{"boss_voice/boss_voice_09_A.mp3"}, {"boss_voice/boss_voice_09_B.mp3"}, {"boss_voice/boss_voice_09_C.mp3"}, {"boss_voice/boss_voice_09_D.mp3"} };
	int titleCallHaldle[7];
};