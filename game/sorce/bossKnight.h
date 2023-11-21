/*****************************************************************//**
 * \file    bossKnight.h
 * \brief   �{�X�u�R�m�v�̃N���X
 *
 * \author  ���c�@�T
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"bossBase.h"
#include"player.h"

 /**
	* @class BossKnight
	* @brief �{�X�u�R�m�v�N���X
	*/
class BossKnight : public BossBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	BossKnight() : BossBase(){};
	/**
	 * @brief �f�X�g���N�^
	 */
	~BossKnight() {};
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
	enum class K_STATUS
	{
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
		SRASH,
		SLAM,
		STAB,
		ROBES,
		JUMPACT,
		ONESLASH,
	};
	K_STATUS status;

	
protected:
	bool  actionFlag;      //!�s�����Ă邩�ǂ���
	bool  jumpActFlag;     //!�W�����v�s�����Ă邩
	bool  posFlag;
	bool  STABFlag;        //!�ːi�U�����Ă邩�ǂ���
    float awakeWaitTime;     //!�o�����ҋ@����
};