/*****************************************************************//**
 * \file    bossBase.cpp
 * \brief   �{�X�̊��N���X�̒�`
 *
 * \author  ���c�@�T
 * \date    November 2023
 *********************************************************************/
#include"bossBase.h"

bool BossBase::Initialize()
{
	type = 2;
	time = 200;
	stanTime = 200;
	_statusInf.stanPoint = 0;
	_statusInf.redHitPoint = 0;
	g = 3.f;
	awake = false;
	awakeDmg = 1;
	awakeMove = 1;
	awakeSpd = 1;
	awakeTime = 0;
	awakeAddDistance = 0;
	return true;
}

bool BossBase::Terminate()
{
	return false;
}

bool BossBase::Process()
{
	for (auto i = charBox->begin(); i != charBox->end(); i++)
	{
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
			dodgeTime = i->second->dodgeTime;
		}
	}
	return true;
}

bool BossBase::Render(float timeSpeed)
{
	return false;
}

 /**
   * @brief �v���C���[�Ƃ̋�������
   * @return PlayerDir(�v���C���[�̕���)
   */
float BossBase::RangeJ(){
	for (auto i = charBox->begin(); i != charBox->end(); i++)
	{
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
		}
	}
	Pvector = VSub(plMI->pos, this->_modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerRange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	if (playerRange < 300)
	{
		this->range = RANGE::CrossRange;
	}
	if (300 <= playerRange && playerRange <= 2000)
	{
		this->range = RANGE::MidRange;
	}
	if (playerRange > 2000)
	{
		this->range = RANGE::LongRange;
	}
	return playerDir;
};

/**
 * @brief �L�����N�^�[�̈ړ�
 * @param speed �ړ����x
 * @param radian �ړ�����
 * @return true
 */
void BossBase::Move(float speed, float radian) 
{
	float Speed = speed;
	float Radian = (this->_modelInf.dir.y + radian) * DX_PI_F / 180.0f;
	this->_modelInf.pos.x -= sin(Radian) * Speed;
	this->_modelInf.pos.z -= cos(Radian) * Speed;
};

/**
 * @brief �L�����N�^�[�̈ړ�
 * @param speed �ړ����x
 * @param radian �ړ�����
 * @return true
 */
bool BossBase::HPmath(float Num, float Stan)
{
	this->_statusInf.hitPoint += Num;
	if (this->_statusInf.redHitPoint <= 0) {this-> _statusInf.redHitPointDelayTime = 100; }
	this->_statusInf.redHitPoint += std::abs(Num);
	this->_statusInf.stanPoint += Stan;
	
	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, this->_modelInf.pos.x, this->_modelInf.pos.y, this->_modelInf.pos.z);

	return true;
}

/**
 * @brief 3D�T�E���h���������ɓǂݍ���
 * @param FileName �T�E���h�̃t�@�C���p�X
 * @param InitRadius �T�E���h�̕������鋗��
 * @param ReverbNo ���o�[�u�v���Z�b�g
 * @return SoundHandle
 */
int BossBase::Load3DSoundMem(const char* FileName, float InitRadius, int ReverbNo)
{
	int tmpSoundHandle=0;

	SetCreate3DSoundFlag(TRUE);
	tmpSoundHandle = LoadSoundMem(FileName);
	SetCreate3DSoundFlag(FALSE);
	Set3DPositionSoundMem(VGet(0, 0, 0), tmpSoundHandle);
	Set3DRadiusSoundMem(InitRadius, tmpSoundHandle);
    //Set3DPresetReverbParamSoundMem(ReverbNo, tmpSoundHandle);
	
	return tmpSoundHandle;
}