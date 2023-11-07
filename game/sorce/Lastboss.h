/*****************************************************************//**
 * \file    Lastboss.h
 * \brief   ���X�{�X�̃N���X
 *
 * \author  ���c�@�T
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"bossBase.h"
#include"player.h"
namespace LASTBOSS {
	constexpr float runSpd = 40.0f;
	//����Ǐ]�t���[���ԍ��ݒ�
	constexpr auto rWeponParentFrame = 190;
	constexpr auto lWeponParentFrame = 165;
}

 /**
  * @class LastBoss
  * @brief ���X�{�X�N���X
  */
class LastBoss : public BossBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	LastBoss() : BossBase() {};
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

protected:
	bool actionFlag;  //!�s�����Ă邩�ǂ���
	bool hitFlag;
	int impactEfcHandle; //!�Ռ��g�G�t�F�N�g�p�n���h��
	int hitTime;
	int voiceStartNum;   //!���Ԗڂ̃{�C�X���Đ����邩
};