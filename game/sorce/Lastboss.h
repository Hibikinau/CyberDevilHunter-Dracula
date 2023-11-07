/*****************************************************************//**
 * \file    Lastboss.h
 * \brief   ラスボスのクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"bossBase.h"
#include"player.h"
namespace LASTBOSS {
	constexpr float runSpd = 40.0f;
	//武器追従フレーム番号設定
	constexpr auto rWeponParentFrame = 190;
	constexpr auto lWeponParentFrame = 165;
}

 /**
  * @class LastBoss
  * @brief ラスボスクラス
  */
class LastBoss : public BossBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	LastBoss() : BossBase() {};
	/**
	 * @brief デストラクタ
	 */
	~LastBoss() {};
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
	enum class STATUS {
		NORMAL,//!初期状態
		quick,//!弱攻撃
		slowATTACK,//!強攻撃
		STAB,//Xボタン側入れ替え技
		cY,//Yボタン側入れ替え技
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,//!回避
		DAMAGE,//!被弾時やられ
		finishAttack,//!覚醒時フィニッシュ技
		NONE,
		WAIT,
		RUN,
		STAN,
		DEAD,
		idol,//!仰ぎ
		idolT,//!爪
		kick,//!ハイキック
		kaiten,//!回転切り
		jumpattack,//!ジャンプ攻撃
		magicK,//!魔法　片手
		magicR,//!魔法　両手
	};
	STATUS status; //!キャラの行動状態インスタンス
	STATUS	oldEstate = STATUS::NORMAL;//!1つ前のキャラの行動状態インスタンス
	STATUS	lastAttackState;//!キャラの最終攻撃インスタンス

protected:
	bool actionFlag;  //!行動してるかどうか
	bool hitFlag;
	int impactEfcHandle; //!衝撃波エフェクト用ハンドル
	int hitTime;
	int voiceStartNum;   //!何番目のボイスを再生するか
};