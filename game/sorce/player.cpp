#include"player.h"
#define runSpd 20.f

#define rWeponParentFrame 102
#define lWeponParentFrame 77
typedef ExclusiveState _estate;

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
	_statusInf.bloodPoint = 0;
	_statusInf.vampireLevel = 0;

	_modelInf.pos = VGet(210.0f, 0.0f, 3100.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//_modelManager.modelImport("game/res/mv1sample/rockbone.mv1", 10.0f, &_modelInf);
	//_modelManager.modelImport("game/res/yukarisanMMD/yukarisan.pmd", 10.0f, &_modelInf);
	_modelManager.modelImport("game/res/Player01/Player1.mv1", 1.5f, &_modelInf);
	_modelManager.weponAttach("game/res/Weapon_Katana/Weapon_katana.mv1", &_modelInf, rWeponParentFrame, 2.f, true, "katana");
	_modelManager.weponAttach("game/res/Weapon_Saya/Weapon_saya.mv1", &_modelInf, lWeponParentFrame, 2.f, true, "saya");
	_modelManager.weponAttach("game/res/Weapon_noutou/Weapon_noutou.mv1", &_modelInf, lWeponParentFrame, 2.f, false, "noutou");
	//_modelManager.weponAttach("game/res/ゆかりんロボ用の武器/ソードブレイカー位置調整.pmx", &_modelInf, "左人指１", 10.f, false, "SwordBreaker");
	//_modelManager.weponAttach("game/res/gunBlade/blade.pmx", &_modelInf, "右人指１", 10.f, false, "GunBlade");

	changeAttackY = &CA_kirinuke;
	changeAttackX = &CA_charge;

	std::vector<int> insSoundHandle;
	insSoundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　攻撃ヒット音/SE_Damage_01.mp3"));
	soundHandle.emplace_back(insSoundHandle);
	insSoundHandle.clear();
	insSoundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃4段目/SE_Player_nATK4_Finish.mp3"));
	//insSoundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃4段目/SE_Player_nATK4_loop.mp3"));
	soundHandle.emplace_back(insSoundHandle);
	insSoundHandle.clear();
	insSoundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃三段のSE/SE_Player_ATK_01.mp3"));
	insSoundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃三段のSE/SE_Player_ATK_02.mp3"));
	insSoundHandle.emplace_back(LoadSoundMem("game/res/SE/プレイヤー　弱攻撃三段のSE/SE_Player_ATK_03.mp3"));
	soundHandle.emplace_back(insSoundHandle);

	return true;
}

bool	PL::Terminate()
{
	CB::Terminate();
	for (int i = 0; i < soundHandle.size(); i++)
	{
		for (int j = 0; j < soundHandle[i].size(); j++)
		{
			DeleteSoundMem(soundHandle[i][j]);
		}
	}
	return true;
}

bool	PL::Input()
{

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
	if (_statusInf.hitPoint <= 0)
	{
		isDead = 1;
		animSpd = 1.f;
		_modelManager.animChange(PL_death, &_modelInf, false, false, false);
		if (_modelInf.isAnimEnd) { isDead = 2; }
		return true;
	}
	if (_statusInf.bloodPoint >= 1000.f)
	{
		_statusInf.vampireLevel++;
		_statusInf.bloodPoint -= 1000.f;
	}
	//_addDir += *_cameraDir + 180.f;

	if (CheckHitKey(KEY_INPUT_W)) { spd = runSpd; charMove(spd, *_cameraDir, false); }
	if (CheckHitKey(KEY_INPUT_A)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f + 90.f, false); }
	if (CheckHitKey(KEY_INPUT_S)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f, false); }
	if (CheckHitKey(KEY_INPUT_D)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f + -90.f, false); }

	float addDir = 0.f, insDir;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::Damage://被弾
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false;
		_modelManager.animChange(PL_damage, &_modelInf, false, false, false);
		if (_modelInf.playTime > 25.f)
		{
			Estate = _estate::NORMAL;
		}
		break;
	case pushButton::B://回避
		_modelInf.animHandleNext = -1;
		animSpd = 1.f;
		spd = 25.f;
		isCharge = 0;

		insDir = getMoveDir(false);
		if (insDir == 0) { insDir = _modelInf.dir.y; }
		dodgeDir = _modelInf.dir.y - insDir;
		if (dodgeDir > 360) { dodgeDir -= 360; }
		else if (dodgeDir < 0) { dodgeDir += 360; }

		if (dodgeDir >= 45 && 135 > dodgeDir) { _modelManager.animChange(PL_dodge_L, &_modelInf, false, true, false); }
		else if (dodgeDir >= 135 && 225 > dodgeDir) { _modelManager.animChange(PL_dodge_B, &_modelInf, false, true, false); }
		else if (dodgeDir >= 225 && 315 > dodgeDir) { _modelManager.animChange(PL_dodge_R, &_modelInf, false, true, false); }
		else { _modelManager.animChange(PL_dodge_F, &_modelInf, false, true, false); }

		dodgeTime = getAnimPlayTotalTime();
		immortalTime = dodgeTime;

		insDir != 0 ? dodgeDir = insDir : dodgeDir = _modelInf.dir.y;
		break;
	case pushButton::X://弱攻撃
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::quickATTACK;
		lastAttackState = _estate::quickATTACK;
		waitNextAttack = 20;
		animSpd = 1.f;
		if (attackNumOld == 0)
		{
			_modelManager.animChange(PL_jaku_1, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 50.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 1)
		{
			_modelManager.animChange(PL_jaku_2, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 50.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 2)
		{
			_modelManager.animChange(PL_jaku_3, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 50.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 3)
		{
			_modelManager.animChange(PL_jaku_4_1, &_modelInf, false, false, true);
			_modelManager.setNextAnim(PL_jaku_4_2, &_modelInf, true, true);
			waitNextAttack += 60;
			attackNumOld++;
		}
		else if (attackNumOld == 4 && isPushButtonAct)
		{
			_modelInf.animHandleNext = -1;
			_modelManager.animChange(PL_jaku_4_3, &_modelInf, false, false, true);
			_modelManager.setNextAnim(PL_jaku_4_2, &_modelInf, true, false);
			waitNextAttack += 60;
			attackNumOld = 4;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 50.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 4 && !isPushButtonAct)
		{
			_modelInf.animHandleNext = -1;
			_modelManager.animChange(PL_jaku_4_4, &_modelInf, false, false, true);
			waitNextAttack = 0;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 50.f, rWeponParentFrame, Char_PL);
		}

		break;
	case pushButton::Y://強攻撃
		_modelInf.dir.y = getMoveDir(true);
		Estate = _estate::slowATTACK;
		lastAttackState = _estate::slowATTACK;
		waitNextAttack = 20;
		animSpd = 1.f;
		if (attackNumOld == 0)
		{
			_modelManager.animChange(PL_kyou_1, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 70.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 1)
		{
			_modelManager.animChange(PL_kyou_2, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 70.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 2)
		{
			_modelManager.animChange(PL_kyou_3, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 70.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 3)
		{
			_modelManager.animChange(PL_kyou_4_1, &_modelInf, false, false, true);
			_modelManager.setNextAnim(PL_kyou_4_2, &_modelInf, true, true);
			waitNextAttack += 60;
			attackNumOld++;
		}
		else if (attackNumOld == 4)
		{
			_modelManager.animChange(PL_kyou_4_3, &_modelInf, false, false, true);
			waitNextAttack += 60;
			attackNumOld = 0;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 70.f, rWeponParentFrame, Char_PL);
		}

		break;
	case pushButton::LBX://入れ替えX
		Estate = _estate::changeATTACKX;
		lastAttackState = _estate::changeATTACKX;
		changeAttackX(this);
		break;
	case pushButton::LBY://入れ替えY
		Estate = _estate::changeATTACKY;
		lastAttackState = _estate::changeATTACKY;
		changeAttackY(this);
		break;
	case pushButton::Lstick://ダッシュ
		Estate = _estate::NORMAL;
		if (_imputInf->_gTrgp[XINPUT_BUTTON_LEFT_THUMB]) { isDash ^= true; }
		//isDash = true;//------------------------------------------------------------------------------------
		//移動先の角度をベクトルにして移動ベクトルに加算
		addDir = getMoveDir(false);
		spd = runSpd;
		if (addDir != 0 && walkTime > 5) { charMove(spd, addDir, true); }
		moveCheck = false;

		break;
	case pushButton::R1://ガード
		Estate = _estate::GUARD;
		animSpd = 1.f;
		if (isCounter)
		{
			KATANAIO(&_modelInf, true);
			_modelManager.animChange(PL_counter, &_modelInf, false, false, true);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 150.f), 20.f, 0.f, getAnimPlayTotalTime(), true, 200.f, rWeponParentFrame, Char_PL);
			isFastGuard = false, isGuard = false, isCounter = false, counterTime = 0;
			immortalTime = getAnimPlayTotalTime();
		}
		else if (isFastGuard)
		{
			animSpd = _valData->counterSpd;
			_modelManager.animChange(PL_guard_1, &_modelInf, false, false, false);
		}
		else {
			_modelManager.animChange(PL_guard_2, &_modelInf, true, false, false);
			Estate = _estate::NORMAL;
		}
		break;
	case pushButton::Neutral://入力なし
		if (attackNumOld != 0) { break; }
		Estate = _estate::NORMAL;
		_modelManager.animChange(PL_idel, &_modelInf, true, true, false);
		spd = 0.f;
		break;
	default:

		break;
	}

	//入れ替え技や回避などの移動処理
	if (waitCAChargeTime > 0) { waitCAChargeTime -= animSpd; }
	else if (CAChargeTime > 0)
	{
		waitCAChargeTime = 0;
		CAChargeTime -= animSpd;
		charMove(CAChargeSpd, _modelInf.dir.y, false);
	}
	else { CAChargeTime = 0; }

	if (dodgeTime > 0)
	{
		if (_modelInf.playTime < _modelInf.totalTime - (_modelInf.totalTime / 6.f) && _modelInf.playTime > _modelInf.totalTime / 6.f)
		{
			charMove(spd, dodgeDir, false);
		}
		dodgeTime--;
	}

	//ボスと重ならないように
	for (auto i = charBox->begin(); i != charBox->end() && !isGhost; ++i)
	{
		if (i->second->type == 1) { continue; }
		auto bossDisV = VSub(i->second->getInf()->pos, _modelInf.pos);
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
	if (moveCheck) { isDash = false; }
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
		if (lastAttackState == _estate::quickATTACK)
		{
			int soundNum = 0, attackType;
			attackNumOld >= 5 ? attackType = 1 : attackType = 2;
			do
			{
				soundNum = rand() % soundHandle[attackType].size();
			} while (playSoundOld[0] == soundNum && soundHandle[attackType].size() > 1);
			playSoundOld[0] = soundNum;
			PlaySoundMem(soundHandle[attackType][soundNum], DX_PLAYTYPE_BACK);

		}
		isHit = false;
	}

	//攻撃用カプセルコリジョンの作成
	//for (int i = 0; i < _modelInf.wepons.size() && allColl->size() > 0; i++)
	//{
	//	if (_modelInf.wepons[i].isActive)
	//	{
	//		attackColl acoll;
	//		acoll.isUseMat = true;
	//		acoll.capColl.parentModelHandle = _modelInf.modelHandle;
	//		acoll.capColl.frameNum = _modelInf.wepons[i].weponAttachFrameNum;
	//		acoll.capColl.underPos = VGet(0.f, 0.f, 0.f);
	//		acoll.capColl.overPos = VGet(0.f, 0.f, -13.f);
	//		acoll.capColl.r = 5.f;
	//		acoll.attackChar = Char_PL;
	//		acoll.activeTimeF = 1.f;
	//		acoll.nonActiveTimeF = 0.f;
	//		acoll.damage = 0.f;

	//		allColl->emplace_back(acoll);
	//	}
	//}
	//modeGameでのカウンター受付時間再生用
	_valData->plCTimeN = counterTime;

	return true;
}

bool	PL::Render(float timeSpeed)
{
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd, timeSpeed);
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	_statusInf.bloodPoint++;
	return true;
}

void PL::charMove(float Speed, float _Dir, bool animChange)
{
	_Dir -= 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	if (animChange)
	{
		_modelManager.animChange(PL_run, &_modelInf, true, true, false);
		animSpd = 1.f;
		_modelInf.dir.y = _Dir + 180.f;
	}
}

bool PL::HPmath(float math)
{
	if (math < 0)
	{
		if (counterTime > 0)
		{
			isCounter = true;
		}
		else if (immortalTime <= 0)
		{
			if (!isGuard || isFastGuard)
			{
				_statusInf.hitPoint += math;
				PlaySoundMem(soundHandle[0][0], DX_PLAYTYPE_BACK);
				BPmath(std::abs(math) * 6);
				Estate = _estate::DAMAGE;
			}

			auto ACDisV = VSub(_modelInf.pos, charBox->find(attackChar)->second->_modelInf.pos);
			ACDisV = VNorm(ACDisV);
			_modelInf.vec = VScale(ACDisV, 50);
		}
	}
	else
	{
		_statusInf.hitPoint += math;

	}

	return true;
}
bool PL::BPmath(float math)
{
	_statusInf.bloodPoint += math;

	return true;
}

pushButton PL::setAction()
{
	bool isNext = false;
	bufferedInput = false;
	isPushButtonAct = false;
	pushButton insEnum = pushButton::Neutral;
	if (Estate == _estate::DAMAGE) { return pushButton::Damage; }
	if (isCounter) { return pushButton::R1; }
	if (isAnimEnd)
	{
		isAnimEnd = false, isGhost = false;
		StopJoypadVibration(DX_INPUT_PAD1);
		if (Estate != _estate::NORMAL && isCharge == 0 && !isGuard) { Estate = _estate::NORMAL; }
		if (isFastGuard)
		{
			isFastGuard = false, counterTime = _valData->_counterTime;
		}
	}
	else if (Estate != _estate::NORMAL) { isNext = true; }

	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1 && !isGuard && Estate != _estate::DODGE) { bufferedInput = true, insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (checkKeyImput(KEY_INPUT_LSHIFT, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0) {
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
		walkTime++;
	}//Lstick
	else { walkTime = 0; }
	if (isNext) { insEnum = pushButton::Irregular; }

	if (checkKeyImput(KEY_INPUT_C, XINPUT_BUTTON_LEFT_SHOULDER))
	{//入れ替え技
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X)) { isNext ? nextKey = pushButton::LBX : insEnum = pushButton::LBX; }//LBX
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::LBY : insEnum = pushButton::LBY; }//LBY
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

	if (checkKeyImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		if (isFastGuard && _modelInf.playTime > 17.5f) { KATANAIO(&_modelInf, false); }
		if (checkTrgImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER) && !isNext) { isFastGuard = true, isGuard = true; }
		if (isGuard && !isNext) { insEnum = pushButton::R1; }
		counterTime > 0 ? counterTime-- : counterTime = 0;
	}
	else
	{
		KATANAIO(&_modelInf, true);
		isGuard = false, counterTime = 0;
	}

	if (waitNextAttack <= 0 && lastAttackState == _estate::quickATTACK && attackNumOld == 4)
	{
		insEnum = pushButton::X;
	}

	if (checkRelImput(KEY_INPUT_SPACE, XINPUT_BUTTON_B))//B
	{
		attackNumOld = 0;
		Estate = _estate::DODGE;
		if (isNext)
		{
			if (_modelInf.playTime < 10.0f)
			{
				insEnum = pushButton::B, nextKey = pushButton::Neutral;
			}
			else { nextKey = pushButton::B; }
		}
		else { insEnum = pushButton::B; }
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
		//if ("senpuu" == name) { changeAttackX = &CA_senpuu; }
	}
	else if ("Y" == XorY)
	{
		if ("charge" == name) { changeAttackY = &CA_charge; }
		if ("kirinuke" == name) { changeAttackX = &CA_kirinuke; }
		//if ("senpuu" == name) { changeAttackY = &CA_senpuu; }
	}

	return true;
}

bool PL::CA_debugAttack(PL* insPL)
{
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	insPL->_modelManager.animChange(PL_kyou_1, &insPL->_modelInf, false, false, true);
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
		insPL->_modelManager.animChange(PL_arts_tsuki_1, &insPL->_modelInf, false, false, true);
		insPL->_modelManager.setNextAnim(PL_arts_tsuki_2, &insPL->_modelInf, true, true);
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
			insPL->_modelManager.animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);
			insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 16.f, 41.f, true, 160.f, rWeponParentFrame, Char_PL);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 40.f;
		}
		else
		{
			insPL->_modelManager.animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);
			float insDamage;
			insPL->chargeLevel == 1 ? insDamage = 100.f : insDamage = 160.f;
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
	//insPL->isCharge = 1;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	insPL->_modelManager.animChange(PL_arts_kirinuke, &insPL->_modelInf, false, false, true);
	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, insPL->getAnimPlayTotalTime(), true, 80.f, rWeponParentFrame, Char_PL);
	insPL->waitCAChargeTime = 12.f;
	insPL->CAChargeTime = 53.f - insPL->waitCAChargeTime;
	insPL->CAChargeSpd = 40.f;
	insPL->isGhost = true;

	return true;
}