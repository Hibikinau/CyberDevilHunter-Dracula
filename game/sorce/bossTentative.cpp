#include"bossTentative.h"
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 1
#define motion_run 2


bool Boss::Initialize()
{
	_handle = MV1LoadModel("game/res/reimu/reimu.mv1");
	Anim_handle = MV1LoadModel("game/res/reimu/taiki.mv1");
	Anim_handle2 = MV1LoadModel("game/res/reimu/aruku.mv1");
	Anim_handle2 = MV1LoadModel("game/res/reimu/kick.mv1");
	_attach_index = -1;		// �A�j���[�V�����A�^�b�`�͂���Ă��Ȃ�

	MV1SetScale(_handle, VGet(10, 10, 10));
	status = STATUS::NONE;
	// �Đ����Ԃ̏�����
	_total_time = 0.f;
	_play_time = 0.0f;
	// �ʒu,�����̏�����
	_vPos = VGet(0.0f, 0.0f, 15000.f);
	_vDir = VGet(0, 0, 1);		// �L�������f���̓f�t�H���g��-Z�����������Ă���
	// ���ʒu�̐ݒ�
	_colSubY = 40.f;
	plMI=getInf();
	return true;
}

bool	Boss::Terminate()
{

	return true;
}

bool	Boss::Input()
{

	return true;
}

bool	Boss::Process()
{
	float addDir = 0.f;
	STATUS oldStatus = status;
	status = STATUS::WAIT;

	if (_vPos.x<plMI.pos.x) {
		status = STATUS::KICK;
	}
	else {
		status = STATUS::WAIT;
	}

	// �X�e�[�^�X���ς���Ă��Ȃ����H
	if (oldStatus == status) {
		// �Đ����Ԃ�i�߂�
		_play_time += 0.5f;
	}
	else {
		// �A�j���[�V�������A�^�b�`����Ă�����A�f�^�b�`����
		if (_attach_index != -1) {
			MV1DetachAnim(_handle, _attach_index);
			_attach_index = -1;
		}
		// �X�e�[�^�X�ɍ��킹�ăA�j���[�V�����̃A�^�b�`
		switch (status) {
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_handle, 0, Anim_handle, FALSE);
			//(*iteChara)->_attach_index = MV1AttachAnim((*iteChara)->_handle, MV1GetAnimIndex((*iteChara)->_handle, "Anim000"), -1, FALSE);
			break;
		case STATUS::WALK:
			_attach_index = MV1AttachAnim(_handle, 0, Anim_handle2, FALSE);
			//(*iteChara)->_attach_index = MV1AttachAnim((*iteChara)->_handle, MV1GetAnimIndex((*iteChara)->_handle, "Anim001"), -1, FALSE);
			break;
		case STATUS::KICK:
			_attach_index = MV1AttachAnim(_handle, 0, Anim_handle3, FALSE);
			break;
		}
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		// �Đ����Ԃ�������
		_play_time = 0.0f;
	}

	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (_play_time >= _total_time) {
		_play_time = 0.0f;
	}


	return true;
}

bool	Boss::Render()
{
	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(_handle, _attach_index, _play_time);

	// �ʒu
	MV1SetPosition(_handle, _vPos);
	// ��������Y����]���Z�o
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);		// ���f�����W���łǂ���������Ă��邩�Ŏ����ς��(�����-z�������Ă���ꍇ)
	MV1SetRotationXYZ(_handle, vRot);
	// �`��
	MV1DrawModel(_handle);
	return true;
}

void Boss::charMove(float Speed, float _Dir)
{
	
}

bool Boss::step()
{
	return true;
}
