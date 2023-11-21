/*****************************************************************//**
 * \file    bossKnight.h
 * \brief   ボス「騎士」のクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"bossBase.h"
#include"player.h"

 /**
	* @class BossKnight
	* @brief ボス「騎士」クラス
	*/
class BossKnight : public BossBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	BossKnight() : BossBase(){};
	/**
	 * @brief デストラクタ
	 */
	~BossKnight() {};
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
	 * @brief キャラクターの現在の状態
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
	bool  actionFlag;      //!行動してるかどうか
	bool  jumpActFlag;     //!ジャンプ行動してるか
	bool  posFlag;
	bool  STABFlag;        //!突進攻撃してるかどうか
    float awakeWaitTime;     //!覚醒時待機時間
};