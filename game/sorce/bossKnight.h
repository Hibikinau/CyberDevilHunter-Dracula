/*****************************************************************//**
 * \file    bossKnight.h
 * \brief   �{�X�u�R�m�v�̃N���X
 *
 * \author  ���c�@�T
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

/**
 * @class BossKnight
 * @brief �{�X�u�R�m�v�N���X
 */
class BossKnight : public CB
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	BossKnight() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~BossKnight() {};
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
	 * @brief �X�V
	 * @return true
	 */
	virtual bool Process();
	/**
	 * @brief �`��
	 * @param timeSpeed �A�j���[�V�����̍Đ����x
	 * @return true
	 */
	virtual bool Render(float timeSpeed);
	/**
	 * @brief �̗͌v�Z����
	 * @param ���l
	 * @return true
	 */
	bool HPmath(float Num,float Stan) override;
	/**
	 * @brief �s�����fAI
	 * @return true 
	 */
	bool UtilityJudge();
	/**
	 * @brief �v���C���[�Ƃ̋�������
	 * @return true 
	 */
	bool RangeJ();
	/**
	 * @brief �L�����N�^�[�̈ړ�
	 * @param speed �ړ����x
	 * @param radian �ړ�����
	 * @return true
	 */
	void Move(float speed, float radian);

	/**
	 * @brief �L�����N�^�[�̌��݂̏��
	 */
	enum class STATUS {
		NONE,
		WAIT,
		DAMEGE,
		STAN,
		DEAD,
		RUN,
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,
		SRASH,
		SLAM,
		STAB,
		ROBES,
		JAMPACT,
		ONESLASH,
	};
	STATUS status;

	/**
	 * @brief �v���C���[�Ƃ̋������
	 */
	enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

protected:
	float animSpd//!�A�j���[�V�����̃X�s�[�h
		 ,stanTime;//!�X�^������
	int  time,//!�ҋ@����
		attackStep//!�s���ԍ�
		, slamEfcHandle;
	bool isAnimEnd;//�A�j���[�V�����I��������ǂ���
	modelInf* plMI;//!�v���C���[���̃C���X�^���X
	bool ActionFlag;//!�s�����Ă邩�ǂ���
	bool jumpActFlag;//!�W�����v�s�����Ă邩
	int  swingSE, //!�a��SE�n���h��
		newSomenHandle;//�G�t�F�N�g�n���h��
	float Pdir,//!�v���C���[�̕��� 
		Prange,//!�v���C���[�Ƃ̋���
		PrangeA;//!�v���C���[�Ƃ̋������̂Q
	bool Awake;//!�o���������ǂ���
	float AwakeSpd,//!�o�����X�s�[�h
		AwakeDmg,//!�o�����ǉ��_���[�W
		AwakeMove,//!�o�����ړ���
		AwakeT;//!�o�����ҋ@����
};