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
	maxBloodPoint = _statusInf.bloodPoint = 100;

	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(0.0f, 0.0f, 18000.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//_modelManager.modelImport("game/res/mv1sample/rockbone.mv1", 10.0f, &_modelInf);
	_modelManager.modelImport("game/res/yukarisanMMD/yukarisan.pmd", 10.0f, &_modelInf);
	_modelManager.weponAttach("game/res/RabbitBunker/RabbitBunkerKai.pmx", &_modelInf, "右人指１", 10.f, true, "RabbitBunker");
	_modelManager.weponAttach("game/res/ゆかりんロボ用の武器/ソードブレイカー位置調整.pmx", &_modelInf, "左人指１", 10.f, true, "SwordBreaker");
	_modelManager.weponAttach("game/res/gunBlade/blade.pmx", &_modelInf, "右人指１", 10.f, false, "GunBlade");

	auto a = MV1SetShapeRate(_modelInf.wepons[2].weponHandle, 7, 1.0f);
	return true;
}

bool	PL::Terminate()
{

	return true;
}

bool	PL::Input()
{

	return true;
}

bool	PL::Process()
{
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
		isImmortal = true;
		isCharge = 0;
		dodgeDir = getMoveDir();
		dodgeDir == 0.f ? dodgeDir = 180.f + *_cameraDir : dodgeDir += *_cameraDir;
		break;
	case pushButton::X://弱攻撃
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
		StartJoypadVibration(DX_INPUT_PAD1, 100, -1);
		if (_modelInf.playTime >= _modelInf.totalTime / 2.f) { StartJoypadVibration(DX_INPUT_PAD1, 500, -1); }
		if (_modelInf.playTime >= _modelInf.totalTime) { StartJoypadVibration(DX_INPUT_PAD1, 1000, -1); }
		if (Estate != _estate::chargeATTACK)
		{
			Estate = _estate::chargeATTACK;
			isCharge = 1;
			chargeLevel = 0;
			_modelManager.animChange(motion_ZOIRUcharge, &_modelInf, false, true);
			animSpd = 0.2f;
		}
		if (isCharge == 2)
		{
			animSpd = 2.f;
			if (chargeLevel == 0)
			{
				if (_modelInf.playTime >= _modelInf.totalTime / 5.f) { chargeLevel++; }
				if (_modelInf.playTime >= _modelInf.totalTime) { chargeLevel++; }
			}
			if (_modelInf.playTime >= _modelInf.totalTime)
			{
				if (chargeLevel == 2) { _modelManager.animChange(motion_ZOIRUattack1, &_modelInf, false, true); }
				else { _modelManager.animChange(motion_ZOIRUattack2, &_modelInf, false, true); }
				isCharge = 0, animSpd = 0.5f;
			}
		}
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
		if (_imputInf->_gTrgp & PAD_INPUT_9) { isDash ^= true; }

		//移動先の角度をベクトルにして移動ベクトルに加算
		addDir = getMoveDir();
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
		if (Estate == _estate::JUMP)
		{
			//移動先の角度をベクトルにして移動ベクトルに加算
			float addDir = getMoveDir();
			if (addDir != 0) { charMove(spd / 2, *_cameraDir + addDir, true); }
			moveCheck = false;

		}
		break;
	}

	if (Estate == _estate::chargeATTACK && chargeLevel == 2 && _modelInf.playTime < 31.f && _modelInf.playTime > 9.f)
	{
		charMove(40.f, _modelInf.dir.y + 180, true);
	}
	if (isImmortal)
	{
		charMove(spd, dodgeDir, false);
	}

	waitNextAttack > 0 ? waitNextAttack-- : attackNumOld = 0;
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	if (_modelInf.pos.x > 670.f) { _modelInf.pos.x = 670.f; }
	if (_modelInf.pos.x < -670.f) { _modelInf.pos.x = -670.f; }

	if (_modelInf.pos.z > 20000.f) { _modelInf.pos.z = 20000.f; }
	if (moveCheck) { isDash = false; }
	return true;
}

bool	PL::Render()
{
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd);
	return true;
}

void PL::charMove(float Speed, float _Dir, bool animChange)
{
	if (Estate != _estate::JUMP && Estate != _estate::chargeATTACK && animChange)
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

	for()
}

pushButton PL::setAction()
{
	if (isGround && Estate == _estate::JUMP) { Estate = _estate::NORMAL; }
	bool isNext = false;
	bufferedInput = false;
	pushButton insEnum = pushButton::Neutral;
	if (isAnimEnd)
	{
		isAnimEnd = false;
		isImmortal = false;
		StopJoypadVibration(DX_INPUT_PAD1);
		if (Estate != _estate::NORMAL && Estate != _estate::JUMP && (Estate != _estate::chargeATTACK && isCharge != 0)) { Estate = _estate::NORMAL; }
	}
	else if (Estate != _estate::NORMAL) { isNext = true; }

	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1) { bufferedInput = true, insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (_imputInf->_gKeyp & PAD_INPUT_9 || _imputInf->_gKeyp & PAD_INPUT_UP || _imputInf->_gKeyp & PAD_INPUT_DOWN
		|| _imputInf->_gKeyp & PAD_INPUT_LEFT || _imputInf->_gKeyp & PAD_INPUT_RIGHT) {
		if (Estate != _estate::chargeATTACK) { insEnum = pushButton::Lstick; }
	}//Lstick
	if (isNext) { insEnum = pushButton::Irregular; }
	if (checkTrgImput(-1, PAD_INPUT_1)) { isNext ? nextKey = pushButton::X : insEnum = pushButton::X; }//X
	if (checkTrgImput(-1, PAD_INPUT_2)) { isNext ? nextKey = pushButton::Y : insEnum = pushButton::Y; }
	if (checkKeyImput(-1, PAD_INPUT_2))//Y
	{
		if (isCharge >= 1) { insEnum = pushButton::Y; }
	}
	else { if (isCharge > 0) { insEnum = pushButton::Y, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//Y離したとき

	if (checkTrgImput(-1, PAD_INPUT_4))
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

float PL::getMoveDir()
{
	float _addDir = 0.f;
	//移動先の角度指定
	//if (checkKeyImput(KEY_INPUT_W, PAD_INPUT_UP) && !checkKeyImput(KEY_INPUT_S, PAD_INPUT_DOWN)) { _addDir = 180.f; }
	//if (!checkKeyImput(KEY_INPUT_W, PAD_INPUT_UP) && checkKeyImput(KEY_INPUT_S, PAD_INPUT_DOWN)) { _addDir = 360.f; }
	//if (!checkKeyImput(KEY_INPUT_A, PAD_INPUT_LEFT) && checkKeyImput(KEY_INPUT_D, PAD_INPUT_RIGHT))
	//{
	//	if (_addDir == 0.f) { _addDir = 270.f; }
	//	else { _addDir == 360.f ? _addDir -= 45.f : _addDir += 45.f; }
	//}
	//if (checkKeyImput(KEY_INPUT_A, PAD_INPUT_LEFT) && !checkKeyImput(KEY_INPUT_D, PAD_INPUT_RIGHT))
	//{
	//	if (_addDir == 0.f) { _addDir = 90.f; }
	//	else { _addDir == 360.f ? _addDir += 45.f : _addDir -= 45.f; }
	//}
	_addDir = (std::atan2(-_imputInf->lStickX, _imputInf->lStickY) * 180.f) / DX_PI_F;
	if (_imputInf->lStickY != 0 && _addDir == 0.f) { _addDir = 360.f; }

	return _addDir;
}