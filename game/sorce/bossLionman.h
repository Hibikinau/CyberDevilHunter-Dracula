/*****************************************************************//**
 * \file    bossLionman.h
 * \brief   �{�X�u���C�I���}���v�̃N���X
 *
 * \author  ���c�@�T
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"bossBase.h"
#include"player.h"

 /**
  * @class BossLionman
  * @brief �{�X�u���C�I���}���v�N���X
  */
class BossLion : public BossBase
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
		DAMAGE,
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

protected:
	bool actionFlag; //!�s�����Ă邩�ǂ���
	bool hitFlag;
	int  dodgeTime,
		hitTime;
};
