/*****************************************************************//**
 * @file    bossKnight.h
 * @brief   �{�X�u�R�m�v�̃N���X
 *
 * @author  ���c�@�T
 * @date    March 2023
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
	 * @brief ������
	 * @return true 
	 */
	virtual bool Initialize();
	/**
	 * @brief 
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
	bool HPmath(float Num,float Stan) override;
	bool UtilityJudge();
	bool RangeJ();

	//�L�����N�^�[�̈ړ�(1.�ړ����x,2.�ړ�����)
	void Move(float speed, float radian);


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

	enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

protected:
	float animSpd, stanTime;
	int  time, attackStep;
	bool AttackFlag, isAnimEnd;
	modelInf* plMI, stage;
	double dir;
	bool MotionFlag;
	bool ActionFlag;
	bool jumpActFlag;
	int  swingSE, newSomenHandle;
	float PrangeA, PrangeB, Pdir, Prange, atkDir;
	bool Awake;
	float AwakeSpd, AwakeDmg,AwakeMove,AwakeT;
};