#include"player.h"
#define walkSpd 5.f
#define runSpd 20.f

#define rWeponParentFrame 192
#define lWeponParentFrame 167
typedef ExclusiveState _estate;

bool PL::Initialize()
{
	useAnim = 0;
	_x = 0;
	_y = 0;
	spd = walkSpd;
	type = 1;
	g = 1.f;
	isCharge = 0;
	Estate = _estate::NORMAL;
	maxHitPoint = _statusInf.hitPoint = 200;
	maxStamina = _statusInf.stamina = 100;
	_statusInf.bloodPoint = 0;

	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(210.0f, 0.0f, 3100.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//_modelManager.modelImport("game/res/mv1sample/rockbone.mv1", 10.0f, &_modelInf);
	//_modelManager.modelImport("game/res/yukarisanMMD/yukarisan.pmd", 10.0f, &_modelInf);
	_modelManager.modelImport("game/res/Player01/Player1.mv1", 1.5f, &_modelInf);
	_modelManager.weponAttach("game/res/Weapon_Katana/Weapon_katana.mv1", &_modelInf, rWeponParentFrame, 2.f, true, "katana");
	_modelManager.weponAttach("game/res/Weapon_Saya/Weapon_saya.mv1", &_modelInf, lWeponParentFrame, 2.f, true, "saya");
	//_modelManager.weponAttach("game/res/ゆかりんロボ用の武器/ソードブレイカー位置調整.pmx", &_modelInf, "左人指１", 10.f, false, "SwordBreaker");
	//_modelManager.weponAttach("game/res/gunBlade/blade.pmx", &_modelInf, "右人指１", 10.f, false, "GunBlade");

	changeAttackY = &CA_charge;
	changeAttackX = &CA_debugAttack;

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
	return true;
}

bool	PL::Input()
{

	return true;
}

bool	PL::Process()
{/*
	if (_statusInf.hitPoint <= 0 || isDead != 0)
	{
		_modelManager.animChange(motion_demoDead, &_modelInf, false, false);
		if (_modelInf.isAnimEnd && isDead == 1) { isDead = 2; return false; }
		else if (isDead != 2) { isDead = 1; }
		return true;
	}*/
	//charMove(spd, *_cameraDir + addDir, true);
	if (_statusInf.hitPoint <= 0)
	{
		isDead = 2;
	}
	//_modelInf.wepons[1].isActive = true;
	//_modelInf.wepons[1].weponAttachFrameNum = 169;

	if (CheckHitKey(KEY_INPUT_D)) { _modelInf.pos.x -= 10; }
	if (CheckHitKey(KEY_INPUT_A)) { _modelInf.pos.x += 10; }
	if (CheckHitKey(KEY_INPUT_W)) { _modelInf.pos.z -= 10; }
	if (CheckHitKey(KEY_INPUT_S)) { _modelInf.pos.z += 10; }

	float addDir = 0.f, insDir;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::B://回避
		if (Estate == _estate::DODGE) { break; }
		Estate = _estate::DODGE;
		_modelManager.animChange(PL_dodge_F, &_modelInf, false, true);
		animSpd = 1.f;
		spd = 25.f;
		dodgeTime = _modelInf.totalTime;
		immortalTime = dodgeTime;
		isCharge = 0;
		//dodgeDir = getMoveDir(false);
		insDir = getMoveDir(false);
		insDir != 0 ? dodgeDir = insDir : dodgeDir = _modelInf.dir.y;
		break;
	case pushButton::X://弱攻撃
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::quickATTACK;
		lastAttackState = _estate::quickATTACK;
		waitNextAttack = 20;
		if (attackNumOld == 0)
		{
			_modelManager.animChange(PL_jaku_1, &_modelInf, false, false);
			animSpd = 1.f;
			waitNextAttack += _valData->plAtkSpd1 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 1)
		{
			_modelManager.animChange(PL_jaku_2, &_modelInf, false, false);
			animSpd = 1.f;
			waitNextAttack += _valData->plAtkSpd2 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 2)
		{
			_modelManager.animChange(PL_jaku_3, &_modelInf, false, false);
			animSpd = 1.f;
			waitNextAttack += 0;// _valData->plAtkSpd3 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 3)
		{
			_modelManager.animChange(PL_jaku_4_1, &_modelInf, false, false);
			animSpd = 1;
			waitNextAttack += _valData->plAtkSpd4 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, -20.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
		}

		break;
	case pushButton::Y://強攻撃
		_modelInf.dir.y = getMoveDir(true);
		Estate = _estate::slowATTACK;
		lastAttackState = _estate::slowATTACK;
		waitNextAttack = 20;
		if (attackNumOld == 0)
		{
			_modelManager.animChange(PL_kyou_1, &_modelInf, false, false);
			animSpd = 1.f;
			waitNextAttack += _valData->plAtkSpd1 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 1)
		{
			_modelManager.animChange(PL_kyou_2, &_modelInf, false, false);
			animSpd = 1.f;
			waitNextAttack += _valData->plAtkSpd2 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
		}
		else if (attackNumOld == 2)
		{
			_modelManager.animChange(PL_kyou_3, &_modelInf, false, false);
			animSpd = 1.f;
			waitNextAttack += 0;// _valData->plAtkSpd3 * 2;
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, _modelInf.totalTime / animSpd + 1, true, 5.f, rWeponParentFrame, Char_PL);
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
		if (addDir != 0) { charMove(spd, addDir, true); }
		moveCheck = false;

		break;
	case pushButton::R1://ガード
		//Estate = _estate::GUARD;
		immortalTime = 30.f;
		break;
	case pushButton::Neutral://入力なし
		if (attackNumOld != 0) { break; }
		Estate = _estate::NORMAL;
		_modelManager.animChange(PL_idel, &_modelInf, true, true);
		spd = 0.f;
		animSpd = 0.5f;
		break;
	default:

		break;
	}

	if (chargeLevel == 2 && _modelInf.playTime < 31.f && _modelInf.playTime > 9.f)
	{
		charMove(40.f, _modelInf.dir.y, false);
	}
	else if (chargeLevel == 2 && _modelInf.playTime > 32.f)
	{
		chargeLevel = 0;
	}

	if (dodgeTime > 0)
	{
		if(_modelInf.playTime < 52.f && _modelInf.playTime > 12.5f){ charMove(spd, dodgeDir, false); }
		dodgeTime--;
	}

	immortalTime > 0 ? immortalTime-- : immortalTime = 0;
	waitNextAttack > 0 ? waitNextAttack-- : attackNumOld = 0;
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	//ボスと重ならないように
	for (auto i = charBox->begin(); i != charBox->end(); ++i)
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

	if (moveCheck) { isDash = false; }
	isImmortal = immortalTime > 0;
	collCap.r = 30.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 30, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 190, 0));
	//首振り-------------
	if (CheckHitKey(KEY_INPUT_RIGHT)) { neckDir += 0.01f; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { neckDir -= 0.01f; }

	MV1ResetFrameUserLocalMatrix(_modelInf.modelHandle, 196);
	MATRIX neckDirMAT = MGetRotY(neckDir);
	MATRIX neckPosMAT = MV1GetFrameLocalMatrix(_modelInf.modelHandle, 196);
	MATRIX neckMAT = MMult(neckDirMAT, neckPosMAT);
	MV1SetFrameUserLocalMatrix(_modelInf.modelHandle, 196, neckMAT);
	//------------------
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

	return true;
}

bool	PL::Render(float timeSpeed)
{
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd, timeSpeed);
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	return true;
}

void PL::charMove(float Speed, float _Dir, bool animChange)
{
	if (animChange)
	{/*
		if (isDash)*/
		{
			_modelManager.animChange(PL_run, &_modelInf, true, true);
			//_modelInf.wepons[1].isActive = false;
			//_modelInf.wepons[1].weponAttachFrameNum = 221;
			animSpd = 1.f;
		}/*
		else
		{
			_modelManager.animChange(motion_walk, &_modelInf, true, true);
			spd = walkSpd;
			animSpd = 1.f;
		}*/
	}
	_Dir -= 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	_modelInf.dir.y = _Dir + 180.f;

}

bool PL::HPmath(float math)
{
	_statusInf.hitPoint += math;
	if (math < 0)
	{
		PlaySoundMem(soundHandle[0][0], DX_PLAYTYPE_BACK);
		BPmath(std::abs(math) * 6);

		auto ACDisV = VSub(_modelInf.pos, charBox->find(attackChar)->second->_modelInf.pos);
		ACDisV = VNorm(ACDisV);
		_modelInf.vec = VScale(ACDisV, 50);
	}

	return true;
}
bool PL::BPmath(float math)
{
	_statusInf.bloodPoint += math;
	if (_statusInf.bloodPoint >= 1000.f)
	{
		_statusInf.vampireLevel++;
		_statusInf.bloodPoint -= 1000.f;
	}

	return true;
}

pushButton PL::setAction()
{
	bool isNext = false;
	bufferedInput = false;
	pushButton insEnum = pushButton::Neutral;
	if (isAnimEnd)
	{
		isAnimEnd = false;
		StopJoypadVibration(DX_INPUT_PAD1);
		if (Estate != _estate::NORMAL && isCharge == 0) { Estate = _estate::NORMAL; }
	}
	else if (Estate != _estate::NORMAL) { isNext = true; }

	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1) { bufferedInput = true, insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (checkKeyImput(KEY_INPUT_LSHIFT, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0) {
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
	}//Lstick
	if (isNext) { insEnum = pushButton::Irregular; }

	if (checkKeyImput(KEY_INPUT_C, XINPUT_BUTTON_LEFT_SHOULDER))
	{//入れ替え技
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X)) { isNext ? nextKey = pushButton::LBX : insEnum = pushButton::LBX; }//LBX
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::LBY : insEnum = pushButton::LBY; }//LBY
	}
	else
	{//通常技
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X)) { isNext ? nextKey = pushButton::X : insEnum = pushButton::X; }//X
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

	if (checkTrgImput(-1, XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		isNext ? nextKey = pushButton::R1 : insEnum = pushButton::R1;
	}

	if (checkTrgImput(KEY_INPUT_SPACE, XINPUT_BUTTON_B))
	{
		if (isNext)
		{
			if (_modelInf.playTime < 6.0f) { insEnum = pushButton::B, nextKey = pushButton::Neutral; }
			else { nextKey = pushButton::B; }
		}
		else { insEnum = pushButton::B; }
	}//B

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
		//if ("senpuu" == name) { changeAttackX = &CA_senpuu; }
	}
	else if ("Y" == XorY)
	{
		if ("charge" == name) { changeAttackY = &CA_charge; }
		//if ("senpuu" == name) { changeAttackY = &CA_senpuu; }
	}

	return true;
}

bool PL::CA_debugAttack(PL* insPL)
{
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	insPL->_modelManager.animChange(PL_kyou_1, &insPL->_modelInf, false, false);
	insPL->animSpd = 1;
	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, insPL->_modelInf.totalTime / insPL->animSpd + 1, true, 99999.f, rWeponParentFrame, Char_PL);

	return true;
}

bool PL::CA_charge(PL* insPL)
{
	insPL->animSpd = 1.f;
	//StartJoypadVibration(DX_INPUT_PAD1, 100, -1);
	//if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 2.f) { StartJoypadVibration(DX_INPUT_PAD1, 500, -1); }
	//if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime) { StartJoypadVibration(DX_INPUT_PAD1, 1000, -1); }
	if (insPL->isCharge == 0)
	{
		insPL->isCharge = 1;
		insPL->chargeLevel = 0;
		insPL->_modelManager.animChange(PL_arts_tsuki_1, &insPL->_modelInf, false, false);
		insPL->_modelManager.setNextAnim(PL_arts_tsuki_2, &insPL->_modelInf, true, false);
	}
	if (insPL->isCharge == 2)
	{
		//StopJoypadVibration(DX_INPUT_PAD1);
		if (insPL->chargeLevel == 0)
		{
			if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 5.f) { insPL->chargeLevel++; }
			if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime) { insPL->chargeLevel++; }
		}
		if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime)
		{
			auto insDir = insPL->getMoveDir(true);
			if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
			insPL->isCharge = 0;
			if (insPL->chargeLevel == 2)
			{
				insPL->_modelManager.animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false);
				insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, -20.f), 20.f, 0.f, insPL->_modelInf.totalTime / insPL->animSpd + 1, true, 5.f, 116, Char_PL);
			}
			else
			{
				insPL->_modelManager.animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false);
				insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, -20.f), 20.f, 0.f, insPL->_modelInf.totalTime / insPL->animSpd + 1, true, 5.f, 116, Char_PL);
			}
		}
	}

	return true;
}