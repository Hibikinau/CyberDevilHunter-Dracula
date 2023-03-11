/*****************************************************************//**
 * @file    bossKnight.h
 * @brief   ボス「騎士」のクラス
 *
 * @author  松田　裕
 * @date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

/**
 * @class BossKnight
 * @brief ボス「騎士」クラス
 */
class BossKnight : public CB
{
public:
	/**
	 * @brief コンストラクタ
	 */
	BossKnight() {};
	/**
	 * @brief デストラクタ
	 */
	~BossKnight() {};
	/**
	 * @brief 初期化
	 * @return true 
	 */
	virtual bool Initialize();
	/**
	 * @brief 
	 * @return true 
	 */
	virtual bool Terminate();
	/**
	 * @brief 更新
	 * @return true
	 */
	virtual bool Process();
	/**
	 * @brief 描画
	 * @param timeSpeed アニメーションの再生速度
	 * @return true
	 */
	virtual bool Render(float timeSpeed);
	bool HPmath(float Num,float Stan) override;
	bool UtilityJudge();
	bool RangeJ();

	//キャラクターの移動(1.移動速度,2.移動方向)
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