/*****************************************************************//**
 * \file    bossLionman.h
 * \brief   �{�X�u���C�I���}���v�̃N���X
 *
 * \author  ���c�@�T
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

 /**
  * @class BossLionman
  * @brief �{�X�u���C�I���}���v�N���X
  */
class BossLion : public CharBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	BossLion() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~BossLion() {};
	/**
	 * @brief ����������
	 * @return true
	 */
	bool Initialize() override;
	/**
	 * @brief �폜����
	 * @return true
	 */
	bool Terminate() override;
	/**
	 * @brief �X�V����
	 * @return true
	 */
	bool Process() override;
	/**
	 * @brief �`�揈��
	 * @param �Đ����x
	 * @return true
	 */
	bool Render(float timeSpeed) override;
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
		ATTACK,
		ATTACK2,
		ATTACK3,
		HANIATTACK,
		TACKLE,
		SLAM,
		DIVE,
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
	float  animSpd,    //!�A�j���[�V�����̃X�s�[�h
	       stanTime;   //!�X�^������
	int    time,       //!�ҋ@����
		   attackStep; //!�s���ԍ�
	bool   isAnimEnd;  //�A�j���[�V�����I��������ǂ���
	modelInf* plMI;    //!�v���C���[���̃C���X�^���X
	bool   ActionFlag; //!�s�����Ă邩�ǂ���
	bool   hitFlag;
	int    swingSE,    //!�a��SE�n���h��
		   newSomenHandle;  //�G�t�F�N�g�n���h��
	float  Pdir,       //!�v���C���[�̕���
		   Prange,     //!�v���C���[�Ƃ̋���
		   PrangeA;    //!�v���C���[�Ƃ̋������̂Q
	bool   Awake;      //!�o���������ǂ���
	float  AwakeSpd,   //!�o�����X�s�[�h
		   AwakeDmg,   //!�o�����ǉ��_���[�W
		   AwakeMove,  //!�o�����ړ���
		   AwakeT,     //!�o�����ҋ@����
	       AwakeTK;    //!�o�����U�����[�V�����ړ��ʒǉ�
	int    dodgeT,
		   hittime;
};
