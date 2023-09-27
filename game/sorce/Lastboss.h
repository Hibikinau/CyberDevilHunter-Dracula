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
	float animSpd,     //!�A�j���[�V�����̃X�s�[�h
		stanTime;   //!�X�^������
	int  time,        //!�ҋ@����
		attackStep;   //!�s���ԍ�
	bool isAnimEnd;   //�A�j���[�V�����I��������ǂ���
	modelInf* plMI;   //!�v���C���[���̃C���X�^���X
	bool actionFlag;  //!�s�����Ă邩�ǂ���
	bool hitFlag;
	int  swingSE,     //!�a��SE�n���h��
		newSomenHandle; //�G�t�F�N�g�n���h��
	float playerDir,       //!�v���C���[�̕���
		playerRange,       //!�v���C���[�Ƃ̋���
		playerDistance;      //!�v���C���[�Ƃ̋������̂Q
	bool awake;       //!�o���������ǂ���
	float awakeSpd,   //!�o�����X�s�[�h
		awakeDmg,     //!�o�����ǉ��_���[�W
		awakeMove,    //!�o�����ړ���
		awakeTime,       //!�o�����ҋ@����
		awakeAddDistance;      //!�o�����U�����[�V�����ړ��ʒǉ�
	int impactEfcHandle; //!�Ռ��g�G�t�F�N�g�p�n���h��
	int hitTime;
	int voiceStartNum;   //!���Ԗڂ̃{�C�X���Đ����邩
};