#include"bossTentative.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 1
#define motion_run 2


bool Boss::Initialize()
{
	_modelManager.modelImport("game/res/reimu/nigareimu/nigareimu.pmx", 10.0f, &_modelInf);
	useAnim = 0;
	//�֊s���̑傫�����C������
	int MaterialNum = MV1GetMaterialNum(_handle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// �}�e���A���̗֊s���̑������擾
		float dotwidth = MV1GetMaterialOutLineDotWidth(_handle, i);
		// �}�e���A���̗֊s���̑������g�債��������������
		MV1SetMaterialOutLineDotWidth(_handle, i, dotwidth / 10);
	}
	status = STATUS::NONE;
	// �Đ����Ԃ̏�����
    _total_time = 0.f;
	_play_time = 0.0f;

	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(0.0f, 0.0f, 15000.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	// ���ʒu�̐ݒ�
	_colSubY = 40.f;
	return true;
}

bool	Boss::Terminate()
{
	CB::Terminate();
	return true;
}

bool	Boss::Input()
{

	return true;
}

bool	Boss::Process()
{
	float addDir = 0.f;

	for (auto i = charBox->begin(); i != charBox->end(); i++) {
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
		}
	}


	//�ύX�_
	//auto x = plMI->pos.x - _modelInf.pos.x;
	//auto y = plMI->pos.y - _modelInf.pos.y;
	//dir=atan2(x,y);

	auto xz = plMI->pos;

	//boss�Ƌ������ȓ��s���ύX
	auto a = VSub(plMI->pos, _modelInf.pos);
	auto b = (std::atan2(-a.x, -a.z) * 180.f) / DX_PI_F;
	float c = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	if (c < 200)
	{
		CRange();
		
	}
	else if(200<=c && c<=500){
		MRange();
		status = STATUS::WALK;
		Walk(xz);
		_modelInf.dir.y = b;
	}
	else  {
		LRange();
		status = STATUS::WALK;
		_modelInf.dir.y = b;
		Walk(xz);
	}


	// �X�e�[�^�X�ɍ��킹�ăA�j���[�V�����̃A�^�b�`
	switch (status) {
	case STATUS::WAIT:
		_modelManager.animChange(motion_idel, &_modelInf, true, true);
		spd = 0.f;
		animSpd = 0.5f;
		break;
	case STATUS::WALK:
		//_modelInf.playTime = 0.f;
		_modelManager.animChange(1, &_modelInf, true, true);
		animSpd = 0.5f;
		break;
	case STATUS::KICK:
		//_modelInf.playTime = 0.f;
		_modelManager.animChange(2, &_modelInf, false, false);
		animSpd = 1.0f;
        break;
	}


	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	return true;
}

bool	Boss::Render()
{
	//DrawFormatString(600, 20, GetColor(0, 0, 0), "%f",dir);
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd);
	return true;
}

void Boss::charMove(float Speed, float _Dir)
{

}

bool Boss::step()
{
	return true;
}

void Boss::Walk(VECTOR x) {
	float xz = 7.0;
	auto c=VSub(x, _modelInf.pos);
	sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
	float radian = _modelInf.dir.y * DX_PI_F / 180.0f;
	//for (auto i = ;) {
		_modelInf.pos.x -= sin(radian) * xz;
		_modelInf.pos.z -= cos(radian) * xz;
	//}
	/*if (x.x<_modelInf.pos.x) {
		_modelInf.pos.x-=xz;
	}
	else if (x.x > _modelInf.pos.x) {
		_modelInf.pos.x+=xz;
	}

	if (x.z < _modelInf.pos.z) {
		_modelInf.pos.z-=xz;
	}
	else if (x.z > _modelInf.pos.z) {
		_modelInf.pos.z+=xz;
	}*/
}

void Boss::CRange(){

	int AttackRand=GetRand(100);
	if (AttackRand<=70) {
		status = STATUS::KICK;
		DrawBox(500, 500 , 510, 520 , GetColor(0, 0, 0), true);
		DrawString(500, 500, "A", GetColor(255, 255, 255));
	}
	else if (AttackRand > 70) {
		status = STATUS::KICK;
		DrawBox(500, 500, 510, 520, GetColor(0, 0, 0), true);
		DrawString(500, 500, "B", GetColor(255, 255, 255));
	}
	return;
}

void Boss::MRange() {
	int AttackRand = GetRand(100);
	if (AttackRand <= 70) {

	}
	else if (AttackRand > 70) {

	}
	return;
}

void Boss:: LRange(){
	float e = plMI->pos.x;
	return;
}
