#include"player.h"
#define walkSpd 6.f
#define runSpd 20.f
#define attackMotionTotalTime1 14.f
#define attackMotionTotalTime2 12.f
#define attackMotionTotalTime3 10.f
#define attackMotionTotalTime4 14.f
#define attackMotionTotalTimeZoiru 58.f
#define attackMotionTotalTimeSenpu 43.f
#define attackMotionTotalTimeSenpuL 34.f
#define motion_idel 7
#define motion_walk 0
#define motion_run 1
#define motion_jump 2
#define motion_DR1 3
#define motion_DR2 4
#define motion_DR3 5
#define motion_DR4 6
#define motion_ZOIRUcharge 9
#define motion_ZOIRUattack1 10
#define motion_ZOIRUattack2 11
#define motion_SENPUU 12
#define motion_SENPUUL 13
#define motion_rollingF 14
#define motion_demoDead 15
typedef ExclusiveState _estate;

bool PL::Initialize()
{
	useAnim = 0;
	_x = 0;
	_y = 0;
	spd = walkSpd;
	type = 1;
	g = 1.f;
	Estate = _estate::NORMAL;
	maxHitPoint = _statusInf.hitPoint = 200;
	maxStamina = _statusInf.stamina = 100;
	_statusInf.bloodPoint = 0;

	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(0.0f, 0.0f, 0.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//_modelManager.modelImport("game/res/mv1sample/rockbone.mv1", 10.0f, &_modelInf);
	_modelManager.modelImport("game/res/yukarisanMMD/yukarisan.pmd", 10.0f, &_modelInf);
	_modelManager.weponAttach("game/res/RabbitBunker/RabbitBunkerKai.pmx", &_modelInf, "右人指１", 10.f, true, "RabbitBunker");
	_modelManager.weponAttach("game/res/ゆかりんロボ用の武器/ソードブレイカー位置調整.pmx", &_modelInf, "左人指１", 10.f, true, "SwordBreaker");
	_modelManager.weponAttach("game/res/gunBlade/blade.pmx", &_modelInf, "右人指１", 10.f, false, "GunBlade");

	changeAttackY = &CA_charge;
	changeAttackX = &CA_senpuu;

	MV1SetShapeRate(_modelInf.wepons[2].weponHandle, 7, 1.0f);

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

bool	PL::attackHitCheck()
{
	for (int i = 0; i < allColl.size(); i++)
	{

	}
	

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

	for (int i = 0; i < allColl.size(); i++)
	{
		if (allColl[i].nonActiveTimeF > 0)
		{
			allColl[i].nonActiveTimeF--;
		}
		else if (allColl[i].activeTimeF > 0)
		{
			allColl[i].activeTimeF--;


		}
		else
		{
			allColl.erase(allColl.begin() + i);
		}
	}

	if (CheckHitKey(KEY_INPUT_D)) { _modelInf.pos.x -= 10; }
	if (CheckHitKey(KEY_INPUT_A)) { _modelInf.pos.x += 10; }
	if (CheckHitKey(KEY_INPUT_W)) { _modelInf.pos.z -= 10; }
	if (CheckHitKey(KEY_INPUT_S)) { _modelInf.pos.z += 10; }

		//HitCheck_Capsule_Capsule();

	float addDir = 0.f;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::B://回避
		if (Estate == _estate::DODGE) { break; }
		Estate = _estate::DODGE;
		_modelManager.animChange(motion_rollingF, &_modelInf, false, true);
		animSpd = 0.5f;
		spd = 25.f;
		dodgeTime = 52;
		immortalTime = dodgeTime;
		isCharge = 0;
		dodgeDir = getMoveDir(true);
		dodgeDir == 0.f ? dodgeDir = 180.f + *_cameraDir : dodgeDir += *_cameraDir;
		break;
	case pushButton::X://弱攻撃
		_modelInf.dir.y = getMoveDir(true);
		Estate = _estate::quickATTACK;
		waitNextAttack = 20;
		if (attackNumOld == 0)
		{
			_modelManager.animChange(motion_DR1, &_modelInf, false, true);
			animSpd = attackMotionTotalTime1 / _valData->plAtkSpd1;
			waitNextAttack += _valData->plAtkSpd1;
			attackNumOld++;
		}
		else if (attackNumOld == 1)
		{
			_modelManager.animChange(motion_DR2, &_modelInf, false, false);
			animSpd = attackMotionTotalTime2 / _valData->plAtkSpd2;
			waitNextAttack += _valData->plAtkSpd2;
			attackNumOld++;
		}
		else if (attackNumOld == 2)
		{
			_modelManager.animChange(motion_DR3, &_modelInf, false, false);
			animSpd = attackMotionTotalTime3 / _valData->plAtkSpd3;
			waitNextAttack += _valData->plAtkSpd3;
			attackNumOld++;
		}
		else if (attackNumOld == 3)
		{
			_modelManager.animChange(motion_DR4, &_modelInf, false, false);
			animSpd = attackMotionTotalTime4 / _valData->plAtkSpd4;
			waitNextAttack += _valData->plAtkSpd4;
			attackNumOld++;
		}
		else if (attackNumOld == 4)
		{
			_modelManager.animChange(motion_SENPUU, &_modelInf, false, false);
			animSpd = attackMotionTotalTimeSenpu / 60.f;
			waitNextAttack += 60.f;
			attackNumOld++;
		}
		else if (attackNumOld == 5)
		{
			if (!_modelManager.animChange(motion_SENPUUL, &_modelInf, false, false)) { _modelInf.playTime = 0.f; }
			animSpd = attackMotionTotalTimeSenpu / 60.f;
			waitNextAttack += 30.f;
			attackNumOld = 5;
		}

		break;
	case pushButton::Y://強攻撃
		_modelInf.dir.y = getMoveDir(true);
		Estate = _estate::slowATTACK;
		waitNextAttack = 20;
		if (attackNumOld == 0)
		{
			_modelManager.animChange(motion_DR1, &_modelInf, false, true);
			animSpd = attackMotionTotalTime1 / _valData->plAtkSpd1;
			waitNextAttack += _valData->plAtkSpd1;
			attackNumOld++;
		}
		else if (attackNumOld == 1)
		{
			_modelManager.animChange(motion_DR2, &_modelInf, false, false);
			animSpd = attackMotionTotalTime2 / _valData->plAtkSpd2;
			waitNextAttack += _valData->plAtkSpd2;
			attackNumOld++;
		}
		else if (attackNumOld == 2)
		{
			_modelManager.animChange(motion_DR3, &_modelInf, false, false);
			animSpd = attackMotionTotalTime3 / _valData->plAtkSpd3;
			waitNextAttack += _valData->plAtkSpd3;
			attackNumOld++;
		}
		else if (attackNumOld == 3)
		{
			_modelManager.animChange(motion_DR4, &_modelInf, false, false);
			animSpd = attackMotionTotalTime4 / _valData->plAtkSpd4;
			waitNextAttack += _valData->plAtkSpd4;
			attackNumOld++;
		}
		else if (attackNumOld == 4)
		{
			_modelManager.animChange(motion_SENPUU, &_modelInf, false, false);
			animSpd = attackMotionTotalTimeSenpu / 60.f;
			waitNextAttack += 60.f;
			attackNumOld++;
		}
		else if (attackNumOld == 5)
		{
			if (!_modelManager.animChange(motion_SENPUUL, &_modelInf, false, false)) { _modelInf.playTime = 0.f; }
			animSpd = attackMotionTotalTimeSenpu / 60.f;
			waitNextAttack += 30.f;
			attackNumOld = 5;
		}

		break;
	case pushButton::LBX://入れ替えX
		Estate = _estate::changeATTACKX;
		changeAttackX(this);
		break;
	case pushButton::LBY://入れ替えY
		Estate = _estate::changeATTACKY;
		changeAttackY(this);
		break;
	case pushButton::A://ジャンプ
		if (isGround)
		{
			Estate = _estate::JUMP;
			_modelInf.vec.y = 30.f;
			_modelInf.playTime = 0.f;
			_modelManager.animChange(motion_jump, &_modelInf, false, false);
			animSpd = 0.5f;
			moveCheck = false;

		}
		break;
	case pushButton::Lstick://ダッシュ
		Estate = _estate::NORMAL;
		if (_imputInf->_gTrgp[XINPUT_BUTTON_LEFT_THUMB]) { isDash ^= true; }

		//移動先の角度をベクトルにして移動ベクトルに加算
		addDir = getMoveDir(false);
		if (addDir != 0) { charMove(spd, *_cameraDir + addDir, true); }
		moveCheck = false;

		break;
	case pushButton::Neutral://入力なし
		if (attackNumOld != 0) { break; }
		Estate = _estate::NORMAL;
		_modelManager.animChange(motion_idel, &_modelInf, true, true);
		spd = 0.f;
		animSpd = 0.5f;
		break;
	default:

		break;
	}

	if (chargeLevel == 2 && _modelInf.playTime < 31.f && _modelInf.playTime > 9.f)
	{
		charMove(40.f, _modelInf.dir.y + 180, false);
	}
	else if (chargeLevel == 2 && _modelInf.playTime > 32.f)
	{
		chargeLevel = 0;
	}

	if (dodgeTime > 0)
	{
		charMove(spd, dodgeDir, false);
		dodgeTime--;
	}

	immortalTime > 0 ? immortalTime-- : immortalTime = 0;
	waitNextAttack > 0 ? waitNextAttack-- : attackNumOld = 0;
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	//if (_modelInf.pos.x > 670.f) { _modelInf.pos.x = 670.f; }
	//if (_modelInf.pos.x < -670.f) { _modelInf.pos.x = -670.f; }
	//if (_modelInf.pos.z > 20000.f) { _modelInf.pos.z = 20000.f; }

	if (moveCheck) { isDash = false; }

	collCap.r = 30.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 30, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 170, 0));

	Einf = charBox->find(Char_BOSS1)->second->getInf();

	//bossと距離一定以内でHP減少
	auto a = VSub(Einf->pos, _modelInf.pos);
	if (sqrt(a.x * a.x + a.y * a.y + a.z * a.z) < 140.f && immortalTime <= 0)
	{
		HPmath(-2.f);
	}
	HPmath(-2.f);
	return true;
}

bool	PL::Render()
{
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd);
	DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 0), GetColor(0, 0, 0), false);
	return true;
}

void PL::charMove(float Speed, float _Dir, bool animChange)
{
	if (animChange)
	{
		if (isDash)
		{
			_modelManager.animChange(motion_run, &_modelInf, true, true);
			spd = runSpd;
			animSpd = 0.5f;
		}
		else
		{
			_modelManager.animChange(motion_walk, &_modelInf, true, true);
			spd = walkSpd;
			animSpd = 0.9f;
		}
	}

	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	_modelInf.dir.y = _Dir + 180.f;

}

bool PL::HPmath(float math)
{
	_statusInf.hitPoint += math;
	if (math < 0) { BPmath(std::abs(math) * 6); }

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

	if (checkKeyImput(-1, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0) {
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
	}//Lstick
	if (isNext) { insEnum = pushButton::Irregular; }

	if (checkKeyImput(-1, XINPUT_BUTTON_LEFT_SHOULDER))
	{//入れ替え技
		if (checkTrgImput(-1, XINPUT_BUTTON_X)) { isNext ? nextKey = pushButton::LBX : insEnum = pushButton::LBX; }//LBX
		if (checkTrgImput(-1, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::LBY : insEnum = pushButton::LBY; }//LBY
	}
	else
	{//通常技
		if (checkTrgImput(-1, XINPUT_BUTTON_X)) { isNext ? nextKey = pushButton::X : insEnum = pushButton::X; }//X
		if (checkTrgImput(-1, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::Y : insEnum = pushButton::Y; }//Y
	}

	if (checkKeyImput(-1, XINPUT_BUTTON_X) && Estate == _estate::changeATTACKX)//LBXチャージ
	{
		if (isCharge >= 1) { insEnum = pushButton::LBX; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKX) { insEnum = pushButton::LBX, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//LB,X離したとき
	if (checkKeyImput(-1, XINPUT_BUTTON_Y) && Estate == _estate::changeATTACKY)//LBYチャージ
	{
		if (isCharge >= 1) { insEnum = pushButton::LBY; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKY) { insEnum = pushButton::LBY, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//LB,Y離したとき

	if (checkTrgImput(-1, XINPUT_BUTTON_B))
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
		if ("senpuu" == name) { changeAttackX = &CA_senpuu; }
	}
	else if ("Y" == XorY)
	{
		if ("charge" == name) { changeAttackY = &CA_charge; }
		if ("senpuu" == name) { changeAttackY = &CA_senpuu; }
	}

	return true;
}

bool PL::CA_senpuu(PL* insPL)
{
	insPL->_modelInf.dir.y = insPL->getMoveDir(true);
	insPL->_modelManager.animChange(motion_SENPUUL, &insPL->_modelInf, false, true);
	insPL->animSpd = attackMotionTotalTimeSenpu / 60.f;

	return true;
}
bool PL::CA_charge(PL* insPL)
{
	StartJoypadVibration(DX_INPUT_PAD1, 100, -1);
	if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 2.f) { StartJoypadVibration(DX_INPUT_PAD1, 500, -1); }
	if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime) { StartJoypadVibration(DX_INPUT_PAD1, 1000, -1); }
	if (insPL->isCharge == 0)
	{
		insPL->isCharge = 1;
		insPL->chargeLevel = 0;
		insPL->_modelManager.animChange(motion_ZOIRUcharge, &insPL->_modelInf, false, true);
		insPL->animSpd = 0.2f;
	}
	if (insPL->isCharge == 2)
	{
		StopJoypadVibration(DX_INPUT_PAD1);
		insPL->animSpd = 2.f;
		if (insPL->chargeLevel == 0)
		{
			if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 5.f) { insPL->chargeLevel++; }
			if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime) { insPL->chargeLevel++; }
		}
		if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime)
		{
			insPL->_modelInf.dir.y = insPL->getMoveDir(true);
			if (insPL->chargeLevel == 2) { insPL->_modelManager.animChange(motion_ZOIRUattack1, &insPL->_modelInf, false, true); }
			else { insPL->_modelManager.animChange(motion_ZOIRUattack2, &insPL->_modelInf, false, true); }
			insPL->isCharge = 0, insPL->animSpd = 0.5f;
		}
	}

	return true;
}