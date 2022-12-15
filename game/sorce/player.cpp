#include"player.h"
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 7
#define motion_walk 0
#define motion_run 1
#define motion_jump 2
#define motion_DR1 3
#define motion_DR2 4
#define motion_DR3 5
#define motion_DR4 6
typedef ExclusiveState _estate;

bool PL::Initialize()
{
	_cg = LoadGraph("game/res/player00.png");
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

	_modelManager.modelImport("game/res/yukarisanMMD/yukarisan.pmd", 10.0f, &_modelInf);
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
		//Estate = _estate::DODGE;

		break;
	case pushButton::X://弱攻撃
		//Estate = _estate::ATTACK;

		break;
	case pushButton::Y://強攻撃
		//Estate = _estate::ATTACK;

		break;
	case pushButton::A://ジャンプ
		if (isGround)
		{
			Estate = _estate::JUMP;
			_modelInf.vec.y = 30.f;
			_modelInf.playTime = 0.f;
			_modelManager.animChange(motion_jump, &_modelInf);
			animSpd = 0.5f;
			moveCheck = false;

		}
		break;
	case pushButton::Lstick://ダッシュ
		Estate = _estate::NORMAL;
		if (*_gTrgp & PAD_INPUT_9) { isDash ^= true; }

		//移動先の角度をベクトルにして移動ベクトルに加算
		addDir = getMoveDir();
		if (addDir != 0) { charMove(spd, *_cameraDir + addDir); }
		moveCheck = false;

		break;
	case pushButton::Neutral://入力なし
		Estate = _estate::NORMAL;
		_modelManager.animChange(motion_idel, &_modelInf);
		spd = 0.f;
		animSpd = 0.5f;
		break;
	default:
		if (Estate == _estate::JUMP)
		{
			//移動先の角度をベクトルにして移動ベクトルに加算
			float addDir = getMoveDir();
			if (addDir != 0) { charMove(spd / 2, *_cameraDir + addDir); }
			moveCheck = false;

		}
		break;
	}

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
	_modelManager.modelRender(&_modelInf);
	DrawGraph(0, 0, _cg, true);
	return true;
}

void PL::charMove(float Speed, float _Dir)
{
	if (Estate != _estate::JUMP)
	{
		if (isDash)
		{
			_modelManager.animChange(motion_run, &_modelInf);
			spd = runSpd;
			animSpd = 0.5f;
		}
		else
		{
			_modelManager.animChange(motion_walk, &_modelInf);
			spd = walkSpd;
			animSpd = 0.9f;
		}
	}

	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	_modelInf.dir.y = _Dir + 180.f;
}

pushButton PL::setAction()
{
	if (isGround && Estate == _estate::JUMP) { Estate = _estate::NORMAL; }
	_modelInf.playTime += animSpd;
	if (_modelInf.playTime > _modelInf.totalTime)
	{
		_modelInf.playTime = 0.f;
		if (Estate != _estate::NORMAL && Estate != _estate::JUMP) { Estate = _estate::NORMAL; }
	}
	else if(Estate != _estate::NORMAL){ return pushButton::Irregular; }

	if (checkTrgImput(-1, PAD_INPUT_4)) { return pushButton::B; }//B
	if (checkTrgImput(-1, PAD_INPUT_3)) { return pushButton::A; }//A
	if (checkTrgImput(-1, PAD_INPUT_1)) { return pushButton::X; }//X
	if (checkTrgImput(-1, PAD_INPUT_2)) { return pushButton::Y; }//Y
	if (*_gKeyp & PAD_INPUT_9 || *_gKeyp & PAD_INPUT_UP || *_gKeyp & PAD_INPUT_DOWN 
		|| *_gKeyp & PAD_INPUT_LEFT || *_gKeyp & PAD_INPUT_RIGHT) { return pushButton::Lstick; }//Lstick
		return pushButton::Neutral;
}

float PL::getMoveDir()
{
	float _addDir = 0.f;
	//移動先の角度指定
	if (checkKeyImput(KEY_INPUT_W, PAD_INPUT_UP) && !checkKeyImput(KEY_INPUT_S, PAD_INPUT_DOWN)) { _addDir = 180.f; }
	if (!checkKeyImput(KEY_INPUT_W, PAD_INPUT_UP) && checkKeyImput(KEY_INPUT_S, PAD_INPUT_DOWN)) { _addDir = 360.f; }
	if (!checkKeyImput(KEY_INPUT_A, PAD_INPUT_LEFT) && checkKeyImput(KEY_INPUT_D, PAD_INPUT_RIGHT))
	{
		if (_addDir == 0.f) { _addDir = 270.f; }
		else { _addDir == 360.f ? _addDir -= 45.f : _addDir += 45.f; }
	}
	if (checkKeyImput(KEY_INPUT_A, PAD_INPUT_LEFT) && !checkKeyImput(KEY_INPUT_D, PAD_INPUT_RIGHT))
	{
		if (_addDir == 0.f) { _addDir = 90.f; }
		else { _addDir == 360.f ? _addDir += 45.f : _addDir -= 45.f; }
	}

	return _addDir;
}