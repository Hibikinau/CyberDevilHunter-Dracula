/*****************************************************************//**
 * \file    Lastboss.h
 * \brief   ���X�{�X�̃N���X
 *
 * \author  ���c�@�T
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

 /**
  * @class LastBoss
  * @brief ���X�{�X�N���X
  */
class LastBoss : public CharBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	LastBoss() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~LastBoss() {};
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
	bool HPmath(float Num, float Stan) override;
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
	 * @brief �G�t�F�N�g�ǂݍ��ݗp�֐�
	 */
	void loadEfekseer() override;

	/**
	 * @brief �L�����N�^�[�̌��݂̏��
	 */
	enum class STATUS {
		NORMAL,//!�������
		quick,//!��U��
		slowATTACK,//!���U��
		STAB,//X�{�^��������ւ��Z
		cY,//Y�{�^��������ւ��Z
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,//!���
		DAMAGE,//!��e������
		finishAttack,//!�o�����t�B�j�b�V���Z
		NONE,
		WAIT,
		RUN,
		STAN,
		DEAD,
		idol,//!��
		idolT,//!��
		kick,//!�n�C�L�b�N
		kaiten,//!��]�؂�
		jumpattack,//!�W�����v�U��
		magicK,//!���@�@�Ў�
		magicR,//!���@�@����
	};
	STATUS status; //!�L�����̍s����ԃC���X�^���X
	STATUS	oldEstate = STATUS::NORMAL;//!1�O�̃L�����̍s����ԃC���X�^���X
	STATUS	lastAttackState;//!�L�����̍ŏI�U���C���X�^���X

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
		, stanTime;//!�X�^������
	int  time,//!�ҋ@����
		attackStep;//!�s���ԍ�
	bool isAnimEnd;//�A�j���[�V�����I��������ǂ���
	modelInf* plMI;//!�v���C���[���̃C���X�^���X
	bool ActionFlag;//!�s�����Ă邩�ǂ���
	bool hitFlag;
	int  swingSE, //!�a��SE�n���h��
		newSomenHandle;//�G�t�F�N�g�n���h��
	float Pdir,//!�v���C���[�̕��� 
		Prange,//!�v���C���[�Ƃ̋���
		PrangeA;//!�v���C���[�Ƃ̋������̂Q
	bool Awake;//!�o���������ǂ���
	float AwakeSpd,//!�o�����X�s�[�h
		AwakeDmg,//!�o�����ǉ��_���[�W
		AwakeMove,//!�o�����ړ���
		AwakeT,//!�o�����ҋ@����
		AwakeTK;
	int impactEfcHandle;
	int hittime;
	int voiceStartNum;
};