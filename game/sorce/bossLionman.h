/*****************************************************************//**
 * \file    bossLionman.h
 * \brief   ボス「ライオンマン」のクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"bossBase.h"
#include"player.h"

 /**
  * @class BossLionman
  * @brief ボス「ライオンマン」クラス
  */
class BossLion : public BossBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	BossLion() {};
	/**
	 * @brief デストラクタ
	 */
	~BossLion() {};
	/**
	 * @brief 初期化処理
	 * @return true
	 */
	bool Initialize() override;
	/**
	 * @brief 削除処理
	 * @return true
	 */
	bool Terminate() override;
	/**
	 * @brief 更新処理
	 * @return true
	 */
	bool Process() override;
	/**
	 * @brief 描画処理
	 * @param 再生速度
	 * @return true
	 */
	bool Render(float timeSpeed) override;
	/**
	 * @brief 体力計算処理
	 * @param 数値
	 * @return true
	 */
	bool HPmath(float Num, float Stan) override;
	/**
	* @brief 行動判断AI
	* @return true
	*/
	bool UtilityJudge();
	/**
	 * @brief キャラクターの移動
	 * @param speed 移動速度
	 * @param radian 移動方向
	 * @return true
	 */
	void Move(float speed, float radian);

	/**
	 * @brief キャラクターの現在の状態
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
	bool actionFlag; //!行動してるかどうか
	bool hitFlag;
	int  dodgeTime,
		hitTime;
};
