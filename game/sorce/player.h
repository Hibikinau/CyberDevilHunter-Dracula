/*****************************************************************//**
 * \file   player.h
 * \brief  プレイヤークラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"

enum class ExclusiveState
{
	NORMAL,
	JUMP,
	quickATTACK,
	slowATTACK,
	changeATTACKX,
	changeATTACKY,
	DODGE,
	GUARD,
	DAMAGE,
	finishAttack,
};

enum class pushButton
{
	Neutral,
	A,
	B,
	X,
	Y,
	LBX,
	LBY,
	LBA,
	Lstick,
	Irregular,
	Damage,
	R1,
	finishAttack,
};

class PL : public CB
{
public:
	PL() {};
	~PL() {};
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Input() { return true; }
	virtual bool Process();
	virtual bool Render(float timeSpeed);
	virtual void charMove(float Speed, float _Dir, bool isAnimChange);
	pushButton setAction();
	bool checkTrgImput(int Key, int Pad) { if (_imputInf->_gTrgb[Key] || _imputInf->_gTrgp[Pad]) { return true; } else { return false; } }
	bool checkKeyImput(int Key, int Pad) { if (_imputInf->_gKeyb[Key] || _imputInf->_gKeyp[Pad]) { return true; } else { return false; } }
	bool checkRelImput(int Key, int Pad) { if (_imputInf->_gRelb[Key] || _imputInf->_gRelp[Pad]) { return true; } else { return false; } }
	float getMoveDir(bool checkUseCamDir);
	bool HPmath(float math, float Stan) override;
	bool BPmath(float math) override;
	bool CA_change(std::string name, const char* XorY) override;
	static bool CA_charge(PL* insPL);
	static bool CA_kirinuke(PL* insPL);
	static bool CA_debugAttack(PL* insPL);
	static bool CA_noData(PL* insPL);
	bool (*changeAttackX)(PL* insPL);
	bool (*changeAttackY)(PL* insPL);
	float getAnimPlayTotalTime() { return _modelInf.totalTime; };
	void battleEndVoice() override { PlaySoundMem(soundHandle[voiceStartNum + 41 + rand() % 2], DX_PLAYTYPE_BACK); }
	void battleStartVoice() override { PlaySoundMem(soundHandle[voiceStartNum + 35 + rand() % 2], DX_PLAYTYPE_BACK); }

protected:
	int _cg, useAnim, attackNumOld, waitNextAttack;
	int _x, _y, stepChargeSec, isStep = false;
	int isCharge;//0.チャージしてない 1.チャージ中 2.解放
	int chargeLevel, chargeTime, insEfcHamdle, isAwakening = 0, voiceStartNum;
	float spd, dodgeDir;
	bool attackFlag, isAnimEnd, isPushButtonAct, isGuard, isFastGuard, isAttackEnd;
	float maxHitPoint, maxBloodPoint, maxStamina, atkBuff = 0.f;
	float nowActionTime;
	bool bufferedInput, isGhost, deadVoice = false;
	int immortalTime, isCounter = 0;
	int playSoundOld[3], walkTime, counterTime, waitCAChargeTime, CAChargeTime, CAChargeSpd;
	ExclusiveState Estate = ExclusiveState::NORMAL, oldEstate = ExclusiveState::NORMAL, lastAttackState;
	pushButton nextKey;
	modelInf* Einf;

	float neckDir = 0.f;
};
