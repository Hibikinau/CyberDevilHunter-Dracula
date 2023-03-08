#include"player.h"
#define runSpd 40.f

#define jakuATK 50.f
#define kyouATK 70.f
#define counterATK 200.f
#define debugATK 99999.f
#define kirinukeATK 80.f
#define charge1ATK 100.f
#define charge2ATK 160.f
#define charge3ATK 160.f
#define finishATK 200.f

#define rWeponParentFrame 192
#define lWeponParentFrame 167
typedef ExclusiveState _estate;
using namespace model;

bool PL::Initialize()
{
	useAnim = 0;
	_x = 0;
	_y = 0;
	spd = runSpd;
	type = 1;
	g = 1.f;
	isCharge = 0;
	Estate = _estate::NORMAL;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 200;
	_statusInf.maxStamina = _statusInf.stamina = 100;
	_statusInf.maxBloodPoint = 1500;
	_statusInf.bloodPoint = 0;

	_modelInf.pos = VGet(210.0f, 1100.0f, 3100.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	modelImport("game/res/Player01/Player1.mv1", 1.5f, &_modelInf, RS);
	weponAttach("game/res/Weapon_Katana/Weapon_katana.mv1", &_modelInf, rWeponParentFrame, 2.f, true, "katana", RS);
	weponAttach("game/res/Weapon_Saya/Weapon_saya.mv1", &_modelInf, lWeponParentFrame, 2.f, true, "saya", RS);
	weponAttach("game/res/Weapon_noutou/Weapon_noutou.mv1", &_modelInf, lWeponParentFrame, 2.f, false, "noutou", RS);

	CA_change(_valData->plChangeAttackX, "X");
	CA_change(_valData->plChangeAttackY, "Y");

	soundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　攻撃ヒット音/SE_Damage_01.mp3"));
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃4段目/SE_Player_nATK4_Finish.mp3"));
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃三段のSE/SE_Player_ATK_01.mp3"));
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃三段のSE/SE_Player_ATK_02.mp3"));
	voiceStartNum = soundHandle.size();
	for (std::string voiceNameList : _valData->playerVoiceList)
	{
		std::string insName = "game/res/Player01/voice/" + voiceNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}

	return true;
}


bool	PL::Terminate()
{
	CB::Terminate();
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

//isIOがtrueで抜刀falseで納刀
bool KATANAIO(modelInf* MI, bool isIO)
{
	MI->wepons[0].isActive = isIO;
	MI->wepons[1].isActive = isIO;
	MI->wepons[2].isActive = !isIO;

	return true;
}

bool	PL::Process()
{
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	if (_statusInf.hitPoint <= 0)
	{
		isDead = 1;
		animSpd = 1.f;
		animChange(PL_death, &_modelInf, false, true, false);
		if (!deadVoice) { deadVoice = true; PlaySoundMem(soundHandle[voiceStartNum + 31 + rand() % 4], DX_PLAYTYPE_BACK); }
		if (_modelInf.isAnimEnd) { isDead = 2; }
		return true;
	}
	if (_statusInf.bloodPoint > 1500.f)
	{
		_statusInf.bloodPoint = 1500.f;
	}

	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 0)
	{
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);
		isAwakening = 1, atkBuff = 100.f, _modelInf.animSpdBuff = .5f;
	}
	if (_imputInf->lTriggerX < 20 && _imputInf->rTriggerX < 20)
	{
		if (isAwakening == 1) { isAwakening = 2; }
		if (isAwakening == -1) { isAwakening = 0; }
	}
	if (isAwakening > 0) { _statusInf.bloodPoint > 0 ? _statusInf.bloodPoint-- : (_statusInf.bloodPoint = 0, isAwakening = false, atkBuff = 0.f, _modelInf.animSpdBuff = 0.f); }

	if (CheckHitKey(KEY_INPUT_W)) { spd = runSpd; charMove(spd, *_cameraDir, false); }
	if (CheckHitKey(KEY_INPUT_A)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f + 90.f, false); }
	if (CheckHitKey(KEY_INPUT_S)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f, false); }
	if (CheckHitKey(KEY_INPUT_D)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f + -90.f, false); }

	float addDir = 0.f, insDir, a;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::Damage://被弾
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0;
		isCharge = 0, isFastGuard = true, isGuard = true;;
		nextKey = pushButton::Neutral;
		animChange(PL_damage, &_modelInf, false, false, false);

		if (_modelInf.playTime > 25.f)
		{
			Estate = _estate::NORMAL;
		}
		break;
	case pushButton::B://回避
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0;
		animSpd = 2.f;
		spd = 50.f;
		isCharge = 0;

		insDir = getMoveDir(false);
		if (insDir == 0) { insDir = _modelInf.dir.y; }
		dodgeDir = _modelInf.dir.y - insDir;
		if (dodgeDir > 360) { dodgeDir -= 360; }
		else if (dodgeDir < 0) { dodgeDir += 360; }

		if (dodgeDir >= 45 && 135 > dodgeDir) { animChange(PL_dodge_L, &_modelInf, false, false, false); }
		else if (dodgeDir >= 135 && 225 > dodgeDir) { animChange(PL_dodge_B, &_modelInf, false, false, false); }
		else if (dodgeDir >= 225 && 315 > dodgeDir) { animChange(PL_dodge_R, &_modelInf, false, false, false); }
		else { animChange(PL_dodge_F, &_modelInf, false, false, false); }

		dodgeTime = getAnimPlayTotalTime();
		immortalTime = dodgeTime;

		insDir != 0 ? dodgeDir = insDir : dodgeDir = _modelInf.dir.y;
		break;
	case pushButton::X://弱攻撃
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::quickATTACK;
		lastAttackState = _estate::quickATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;
		if (attackNumOld < 3) { PlaySoundMem(soundHandle[voiceStartNum + 0 + rand() % 4], DX_PLAYTYPE_BACK); }
		else { PlaySoundMem(soundHandle[voiceStartNum + 4 + rand() % 2], DX_PLAYTYPE_BACK); }
		if (attackNumOld == 0)
		{
			animChange(PL_jaku_1, &_modelInf, false, false, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 1)
		{
			animChange(PL_jaku_2, &_modelInf, false, false, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 2)
		{
			animChange(PL_jaku_3, &_modelInf, false, false, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 3)
		{
			animChange(PL_jaku_4_1, &_modelInf, false, false, true);
			setNextAnim(PL_jaku_4_2, &_modelInf, true, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 4 && isPushButtonAct)
		{
			_modelInf.animHandleNext = -1;
			animChange(PL_jaku_4_3, &_modelInf, false, false, true);
			setNextAnim(PL_jaku_4_2, &_modelInf, true, false);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld = 4;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, 12.f, true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 17.f, 8.f, true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 32.f, 10.f, true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 47.f, 13.f, true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 4 && !isPushButtonAct)
		{
			_modelInf.animHandleNext = -1;
			animChange(PL_jaku_4_4, &_modelInf, false, false, true);
			waitNextAttack = 0;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, jakuATK + atkBuff, rWeponParentFrame, Char_PL);
		}

		break;
	case pushButton::Y://強攻撃
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::slowATTACK;
		lastAttackState = _estate::slowATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;
		if (attackNumOld < 3) { PlaySoundMem(soundHandle[voiceStartNum + 6 + rand() % 5], DX_PLAYTYPE_BACK); }
		else { PlaySoundMem(soundHandle[voiceStartNum + 11 + rand() % 3], DX_PLAYTYPE_BACK); }
		if (attackNumOld == 0)
		{
			animChange(PL_kyou_1, &_modelInf, false, false, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, kyouATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 1)
		{
			animChange(PL_kyou_2, &_modelInf, false, false, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, kyouATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 2)
		{
			animChange(PL_kyou_3, &_modelInf, false, false, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, kyouATK + atkBuff, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 3)
		{
			animChange(PL_kyou_4_1, &_modelInf, false, false, true);
			setNextAnim(PL_kyou_4_2, &_modelInf, true, true);
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 4)
		{
			animChange(PL_kyou_4_3, &_modelInf, false, false, true);
			_modelInf.animHandleNext = -1;
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld = 0;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, kyouATK + atkBuff, rWeponParentFrame, Char_PL);
		}

		break;
	case pushButton::LBX://入れ替えX
		Estate = _estate::changeATTACKX;
		lastAttackState = _estate::changeATTACKX;
		attackNumOld = 0;
		changeAttackX(this);
		break;
	case pushButton::LBY://入れ替えY
		Estate = _estate::changeATTACKY;
		lastAttackState = _estate::changeATTACKY;
		attackNumOld = 0;
		changeAttackY(this);
		break;
	case pushButton::finishAttack://必殺技
		Estate = _estate::finishAttack;
		lastAttackState = _estate::finishAttack;
		attackNumOld = 0;
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);
		animChange(PL_motion_hissatsu, &_modelInf, false, false, true);
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 200.f), 20.f, 0.f, getAnimPlayTotalTime(), true, finishATK + atkBuff, rWeponParentFrame, Char_PL);
		isAwakening = -1;
		immortalTime = _modelInf.totalTime;
		_statusInf.bloodPoint = 0;
		break;
	case pushButton::Lstick://ダッシュ
		Estate = _estate::NORMAL;
		attackNumOld = 0;
		//isDash = true;//------------------------------------------------------------------------------------
		//移動先の角度をベクトルにして移動ベクトルに加算
		addDir = getMoveDir(false);
		spd = runSpd;//walkTime
		if (addDir != 0) { charMove(spd * (walkTime / 10), addDir, true); }
		moveCheck = false;
		break;
	case pushButton::R1://ガード
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0;
		if (Estate != _estate::GUARD && isFastGuard) { Estate = _estate::GUARD; }
		nextKey = pushButton::Neutral;
		if (isCounter == 1)
		{
			animSpd = 3.f;
			KATANAIO(&_modelInf, true);
			PlaySoundMem(soundHandle[voiceStartNum + 37 + rand() % 2], DX_PLAYTYPE_BACK);
			animChange(PL_counter, &_modelInf, false, false, true);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 150.f), 20.f, 0.f, getAnimPlayTotalTime(), true, counterATK + atkBuff, rWeponParentFrame, Char_PL);
			isFastGuard = true, isCounter = 2, counterTime = 0;
			immortalTime = getAnimPlayTotalTime();
		}
		else if (isFastGuard && isCounter == 0)
		{
			animSpd = 1.f;
			animSpd = _valData->counterSpd;
			animChange(PL_guard_1, &_modelInf, false, false, false);
		}
		else if (isCounter != 2) {
			animSpd = 1.f;
			animChange(PL_guard_2, &_modelInf, true, false, false);
			Estate = _estate::NORMAL;
			BPmath(-1);
		}
		break;
	case pushButton::Neutral://入力なし
		if (attackNumOld > 0) { break; }
		Estate = _estate::NORMAL;
		animChange(PL_idel, &_modelInf, true, true, false);
		spd = 0.f;
		break;
	default:
		break;
	}

	//入れ替え技や回避などの移動処理
	if (waitCAChargeTime > 0) { waitCAChargeTime -= animSpd + _modelInf.animSpdBuff; }
	else if (CAChargeTime > 0)
	{
		waitCAChargeTime = 0;
		CAChargeTime -= animSpd + _modelInf.animSpdBuff;
		charMove(CAChargeSpd, _modelInf.dir.y, false);
	}
	else { CAChargeTime = 0; }

	if (dodgeTime > 0)
	{
		if (_modelInf.playTime < _modelInf.totalTime - (_modelInf.totalTime / 6.f) && _modelInf.playTime > _modelInf.totalTime / 6.f)
		{
			animSpd = 1.5f;
			charMove(spd, dodgeDir, false);
		}
		else { animSpd = 3.f; }
		dodgeTime -= animSpd;
	}

	//ボスと重ならないように
	for (auto i = charBox->begin(); i != charBox->end() && !isGhost; ++i)
	{
		if (i->second->type == 1) { continue; }
		auto  bossDisV = VSub(i->second->getInf()->pos, _modelInf.pos);
		float bossDisF = sqrt(bossDisV.x * bossDisV.x + bossDisV.y * bossDisV.y + bossDisV.z * bossDisV.z);
		float plzDis = i->second->collCap.r + collCap.r;
		if (bossDisF < plzDis)
		{
			_modelInf.pos = VAdd(VScale(VNorm(bossDisV), -plzDis), i->second->getInf()->pos);
		}
	}

	//ベクトル計算などの舞フレーム必要な処理
	immortalTime > 0 ? immortalTime-- : immortalTime = 0;
	waitNextAttack > 0 ? waitNextAttack-- : attackNumOld = 0;
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;
	isImmortal = immortalTime > 0;
	collCap.r = 30.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 30, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 190, 0));

	//首振り-------------
	//if (CheckHitKey(KEY_INPUT_RIGHT)) { neckDir += 0.01f; }
	//if (CheckHitKey(KEY_INPUT_LEFT)) { neckDir -= 0.01f; }

	MV1ResetFrameUserLocalMatrix(_modelInf.modelHandle, 103);
	MATRIX neckDirMAT = MGetRotY(neckDir);
	MATRIX neckPosMAT = MV1GetFrameLocalMatrix(_modelInf.modelHandle, 103);
	MATRIX neckMAT = MMult(neckDirMAT, neckPosMAT);
	MV1SetFrameUserLocalMatrix(_modelInf.modelHandle, 103, neckMAT);
	//------------------

	//攻撃ヒット時のSE再生
	if (isHit)
	{
		BPmath(100);
		if (lastAttackState == _estate::quickATTACK)
		{
			int soundNum = 0;
			attackNumOld >= 5 ? soundNum = 1 : soundNum = 2 + rand() % 3;
			PlaySoundMem(soundHandle[soundNum], DX_PLAYTYPE_BACK);
		}
		isHit = false;
	}

	//modeGameでのカウンター受付時間再生用
	_valData->plCTimeN = counterTime;

	return true;
}

bool	PL::Render(float timeSpeed)
{
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);
	return true;
}

void PL::charMove(float Speed, float _Dir, bool isAnimChange)
{
	_Dir -= 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	if (isAnimChange)
	{
		animChange(PL_run, &_modelInf, true, true, false);
		animSpd = 1.f;
		_modelInf.dir.y = _Dir + 180.f;
	}
}

bool PL::HPmath(float math,float Stan)
{
	if (math < 0)
	{
		if (counterTime > 0) { isCounter = 1; }
		else if (immortalTime <= 0)
		{
			if (!isGuard || isFastGuard)
			{
				if (isAwakening == 0)
				{
					if (!deadVoice) { PlaySoundMem(soundHandle[voiceStartNum + 27 + rand() % 4], DX_PLAYTYPE_BACK); }
					_statusInf.hitPoint += math; BPmath(std::abs(math) * 6);
				}
				PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
				Estate = _estate::DAMAGE;/*
				auto a = PlayEffekseer3DEffect(_valData->efcHandle);
				SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y, _modelInf.pos.z);*/
			}

			auto ACDisV = VSub(_modelInf.pos, charBox->find(attackChar)->second->_modelInf.pos);
			ACDisV = VNorm(ACDisV);
			_modelInf.vec = VScale(ACDisV, 50);
		}
	}
	else
	{
		_statusInf.hitPoint += math;
		if (_statusInf.hitPoint > _statusInf.maxHitPoint) { _statusInf.hitPoint = _statusInf.maxHitPoint; }
	}
	if (dodgeTime > 0) { PlaySoundMem(soundHandle[voiceStartNum + 24 + rand() % 3], DX_PLAYTYPE_BACK); }

	return true;
}
bool PL::BPmath(float math)
{
	if (isAwakening > 0 && math > 0) { return false; }
	_statusInf.bloodPoint += math;
	if (_statusInf.bloodPoint > _statusInf.maxBloodPoint) { _statusInf.bloodPoint = _statusInf.maxBloodPoint; }
	if (_statusInf.bloodPoint < 0) { _statusInf.bloodPoint = 0; }

	return true;
}

pushButton PL::setAction()
{
	bool isNext = false;
	bufferedInput = false;
	isPushButtonAct = false;
	pushButton insEnum = pushButton::Neutral;

	if (Estate == _estate::DAMAGE) { return pushButton::Damage; }
	if (isCounter == 1) { return pushButton::R1; }
	if (isAnimEnd)
	{
		isAnimEnd = false, isGhost = false;
		StopJoypadVibration(DX_INPUT_PAD1);
		if (Estate != _estate::NORMAL && isCharge == 0 && !isGuard) { Estate = _estate::NORMAL; }
		if (isFastGuard) { isFastGuard = false, counterTime = _valData->_counterTime; }
		if (isCounter == 2) { isCounter = 0; }
	}
	else if (Estate != _estate::NORMAL) { isNext = true; }

	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1 && !isGuard && Estate != _estate::DODGE) { bufferedInput = true, insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (checkTrgImput(KEY_INPUT_SPACE, XINPUT_BUTTON_B))//B
	{
		attackNumOld = 0;
		Estate = _estate::DODGE;
		return insEnum = pushButton::B;
	}
	else if (checkKeyImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER) && dodgeTime <= 0)
	{
		if (isFastGuard && _modelInf.playTime > 17.5f) { KATANAIO(&_modelInf, false); }
		if (checkTrgImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER)) { isFastGuard = true, isGuard = true; }
		counterTime > 0 ? counterTime-- : counterTime = 0;
		return insEnum = pushButton::R1;
	}
	else
	{
		KATANAIO(&_modelInf, true);
		isGuard = false, counterTime = 0;
	}

	if (checkKeyImput(KEY_INPUT_LSHIFT, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0) {
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
		walkTime < 10 ? walkTime++ : walkTime = 10;
	}//Lstick
	else { walkTime = 0; }
	if (isNext) { insEnum = pushButton::Irregular; }

	if (checkKeyImput(KEY_INPUT_C, XINPUT_BUTTON_LEFT_SHOULDER))
	{//入れ替え技
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X)) { isNext ? nextKey = pushButton::LBX : insEnum = pushButton::LBX; }//LBX
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::LBY : insEnum = pushButton::LBY; }//LBY
		if (checkTrgImput(KEY_INPUT_V, XINPUT_BUTTON_A) && _statusInf.bloodPoint > 500.f) { HPmath(100,10); BPmath(-500); }//LBA
	}
	else
	{//通常技
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X))
		{
			if (isNext) { if (attackNumOld != 4) { nextKey = pushButton::X; } }
			else { insEnum = pushButton::X; }
			isPushButtonAct = true;
		}//X
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::Y : insEnum = pushButton::Y; }//Y
	}

	if (checkKeyImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && Estate == _estate::changeATTACKX)//LBXチャージ
	{
		if (isCharge >= 1) { insEnum = pushButton::LBX; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKX) { insEnum = pushButton::LBX, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//LB,X離したとき
	if (checkKeyImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && Estate == _estate::changeATTACKY)//LBYチャージ
	{
		if (isCharge >= 1) { insEnum = pushButton::LBY; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKY) { insEnum = pushButton::LBY, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//LB,Y離したとき

	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 2)
	{
		isNext ? nextKey = pushButton::finishAttack : insEnum = pushButton::finishAttack;
	}

	if (Estate == _estate::finishAttack) { return pushButton::Irregular; }

	if (waitNextAttack <= 0 && lastAttackState == _estate::quickATTACK && attackNumOld == 4)
	{
		insEnum = pushButton::X;
	}


	return insEnum;
}

float PL::getMoveDir(bool checkUseCamDir)
{
	float _addDir = 0.f;
	//移動先の角度指定
	_addDir = (std::atan2(-_imputInf->lStickX, -_imputInf->lStickY) * 180.f) / DX_PI_F;
	if (_imputInf->lStickY != 0 && _addDir == 0.f) { _addDir = 360.f; }
	if (_addDir != 0) { _addDir += *_cameraDir + 180.f; }
	if (camDir != -1.f && checkUseCamDir)
	{
		_addDir = camDir;
	}
	return _addDir;
}

bool PL::CA_change(std::string name, const char* XorY)
{//changeAttackY = &CA_charge;
	if ("X" == XorY)
	{
		if ("charge" == name) { changeAttackX = &CA_charge; }
		if ("kirinuke" == name) { changeAttackX = &CA_kirinuke; }
		if ("debug" == name) { changeAttackX = &CA_debugAttack; }
	}
	else if ("Y" == XorY)
	{
		if ("charge" == name) { changeAttackY = &CA_charge; }
		if ("kirinuke" == name) { changeAttackY = &CA_kirinuke; }
		if ("debug" == name) { changeAttackY = &CA_debugAttack; }
	}

	return true;
}

bool PL::CA_debugAttack(PL* insPL)
{
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	animChange(PL_motion_hissatsu, &insPL->_modelInf, false, false, true);
	insPL->animSpd = 1.f;
	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, insPL->getAnimPlayTotalTime(), true, 99999.f, rWeponParentFrame, Char_PL);

	return true;
}

bool PL::CA_charge(PL* insPL)
{
	insPL->animSpd = 1.f;
	insPL->chargeTime < 1000 ? insPL->chargeTime += 12 : insPL->chargeTime = 100;

	StartJoypadVibration(DX_INPUT_PAD1, insPL->chargeTime, -1);
	if (insPL->isCharge == 0)
	{
		insPL->isCharge = 1;
		insPL->chargeLevel = 0;
		insPL->chargeTime = 0;
		animChange(PL_arts_tsuki_1, &insPL->_modelInf, false, false, true);
		setNextAnim(PL_arts_tsuki_2, &insPL->_modelInf, true, true);
	}
	if (insPL->isCharge == 2)
	{
		StopJoypadVibration(DX_INPUT_PAD1);
		if (insPL->_modelInf.animHandleNext == -1)
		{
			insPL->chargeLevel = 2;
		}
		else if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 5.f)
		{
			insPL->chargeLevel = 1;
		}
		insPL->_modelInf.animHandleNext = -1;

		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
		insPL->isCharge = 0;
		if (insPL->chargeLevel == 2)
		{
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);
			insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 16.f, 41.f, true, charge3ATK + insPL->atkBuff, rWeponParentFrame, Char_PL);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 40.f;
		}
		else
		{
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);
			float insDamage = insPL->atkBuff;
			insPL->chargeLevel == 1 ? insDamage += charge2ATK : insDamage += charge1ATK;
			insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 16.f, 41.f, true, insDamage, rWeponParentFrame, Char_PL);

		}
		insPL->isCharge = 0;
		insPL->chargeLevel = 0;
		insPL->isGhost = true;
	}

	return true;
}

bool PL::CA_kirinuke(PL* insPL)
{
	insPL->animSpd = 1.f;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 22 + rand() % 2], DX_PLAYTYPE_BACK);
	animChange(PL_arts_kirinuke, &insPL->_modelInf, false, false, true);
	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, insPL->getAnimPlayTotalTime(), true, kirinukeATK + insPL->atkBuff, rWeponParentFrame, Char_PL);
	insPL->waitCAChargeTime = 12.f;
	insPL->CAChargeTime = 53.f - insPL->waitCAChargeTime;
	insPL->CAChargeSpd = 40.f;
	insPL->isGhost = true;

	return true;
}