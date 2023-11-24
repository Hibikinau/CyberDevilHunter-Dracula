/*****************************************************************//**
 * \file    bossBase.h
 * \brief   �{�X�̊��N���X
 *
 * \author  ���c�@�T
 * \date    November 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include <map>
#include <memory>
#include <typeindex>


 /**
�@* @class BossKnight
�@* @brief �{�X�̊��N���X
�@*/
class BossBase : public CharBase
{
public:
	/**
	 * @brief �v���C���[�Ƃ̋������
	 */
	enum class RANGE
	{
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

	/**
	 * @brief �R���X�g���N�^
	 */
	BossBase() :CharBase() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~BossBase() {};
	/**
	 * @brief ����������
	 * @return true
	 */
	virtual bool Initialize();
	/**
	 * @brief �폜����
	 * @return true
	 */
	virtual bool Terminate();
	/**
	 * @brief �X�V����
	 * @return true
	 */
	virtual bool Process();
	/**
	 * @brief �`�揈��
	 * @param �Đ����x
	 * @return true
	 */
	virtual bool Render(float timeSpeed);
	/**
	 * @brief �v���C���[�Ƃ̋�������
	 * @return PlayerDir(�v���C���[�̕���)
	 */
	float RangeJ();
	/**
�@ * @brief �L�����N�^�[�̈ړ�
 �@* @param speed �ړ����x
 �@* @param radian �ړ�����
 �@* @return true
 �@*/
	void Move(float speed, float radian);
	/**
	 * @brief �̗͌v�Z����
	 * @param ���l
	 * @return true
	 */
	bool HPmath(float Num, float Stan) override;
	/**
 �@�@* @brief 3D�T�E���h���������ɓǂݍ���
 �@�@* @param FileName �T�E���h�̃t�@�C���p�X
�@�@ * @param InitRadius �T�E���h�̕������鋗��
�@�@ * @param ReverbNo ���o�[�u�v���Z�b�g
�@�@ * @return SoundHandle
 �@�@*/
	int Load3DSoundMem(const char* FileName, float InitRadius = 256.0f, int ReverbNo = DX_REVERB_PRESET_DEFAULT);

protected:
	float animSpd,     //!�A�j���[�V�����̃X�s�[�h
		stanTime;   //!�X�^������
	int  time,        //!�ҋ@����
		attackStep;   //!�s���ԍ�
	int  swingSE,     //!�a��SE�n���h��
		newSomenHandle; //�G�t�F�N�g�n���h��
	float playerDir,       //!�v���C���[�̕���
		playerRange,       //!�v���C���[�Ƃ̋���
		playerDistance;      //!�v���C���[�Ƃ̋������̂Q
	VECTOR Pvector;               //!�v���C���[�Ƃ̋����̐�
	bool awake;       //!�o���������ǂ���
	float awakeSpd,   //!�o�����X�s�[�h
		awakeDmg,     //!�o�����ǉ��_���[�W
		awakeMove,    //!�o�����ړ���
		awakeTime,       //!�o�����ҋ@����
		awakeAddDistance;      //!�o�����U�����[�V�����ړ��ʒǉ�
	modelInf* plMI; //!�v���C���[���̃C���X�^���X
	bool  isAnimEnd;       //�A�j���[�V�����I��������ǂ���
};