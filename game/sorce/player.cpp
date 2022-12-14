#include"player.h"
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 1
#define motion_run 2

bool PL::Initialize()
{
	_cg = LoadGraph("game/res/player00.png");
	useAnim = 0;
	_x = 0;
	_y = 0;
	spd = walkSpd;
	type = 1;
	g = 1.f;
	Estate = NOMAL;

	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(0.0f, 0.0f, 18000.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	_modelManager.modelImport("game/res/SDChar/motion_SDChar_idle_walk_run.mv1", 1.0f, &_modelInf);
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

	if (step())
	{
		if (checkKeyImput(KEY_INPUT_W, PAD_INPUT_UP) && !checkKeyImput(KEY_INPUT_S, PAD_INPUT_DOWN)) { addDir = 180.f; }
		if (!checkKeyImput(KEY_INPUT_W, PAD_INPUT_UP) && checkKeyImput(KEY_INPUT_S, PAD_INPUT_DOWN)) { addDir = 360.f; }
		if (!checkKeyImput(KEY_INPUT_A, PAD_INPUT_LEFT) && checkKeyImput(KEY_INPUT_D, PAD_INPUT_RIGHT))
		{
			if (addDir == 0.f) { addDir = 270.f; }
			else { addDir == 360.f ? addDir -= 45.f : addDir += 45.f; }
		}
		if (checkKeyImput(KEY_INPUT_A, PAD_INPUT_LEFT) && !checkKeyImput(KEY_INPUT_D, PAD_INPUT_RIGHT))
		{
			if (addDir == 0.f) { addDir = 90.f; }
			else { addDir == 360.f ? addDir += 45.f : addDir -= 45.f; }
		}
		if (addDir != 0) { charMove(spd, *_cameraDir + addDir); }
		else
		{
			if(Estate == NOMAL){ _modelManager.animChange(motion_idel, &_modelInf); }
			spd = 0.f;
			animSpd = 0.5f;
		}
		if (checkTrgImput(KEY_INPUT_SPACE, PAD_INPUT_3) && Estate != JUMP)
		{
			Estate = JUMP;
			_modelInf.vec.y = 20.f;
			_modelManager.animChange(motion_run, &_modelInf);
			animSpd = 0.5f;
		}
		_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
		_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	}
	if (_modelInf.pos.y > 0.f) { _modelInf.vec.y -= g; }
	else { _modelInf.pos.y = 0.f, Estate = NOMAL; }

	if (_modelInf.pos.x > 670.f) { _modelInf.pos.x = 670.f; }
	if (_modelInf.pos.x < -670.f) { _modelInf.pos.x = -670.f; }

	if (_modelInf.pos.z > 20000.f) { _modelInf.pos.z = 20000.f; }
	return true;
}

bool	PL::Render()
{
	_modelInf.playTime += animSpd;
	if (_modelInf.playTime >= _modelInf.totalTime) { _modelInf.playTime = 0.0f; }
	_modelManager.modelRender(&_modelInf);
	DrawGraph(0, 0, _cg, true);
	return true;
}

void PL::charMove(float Speed, float _Dir)
{
	if (Estate != JUMP)
	{
		if (checkKeyImput(KEY_INPUT_LSHIFT, PAD_INPUT_4))
		{
			_modelManager.animChange(motion_run, &_modelInf);
			spd = runSpd;
			animSpd = 0.5f;
		}
		else if (!checkKeyImput(KEY_INPUT_LSHIFT, PAD_INPUT_4))
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

bool PL::step()
{
	return true;
}
