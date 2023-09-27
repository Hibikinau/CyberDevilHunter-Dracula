/*****************************************************************//**
 * \file    Lastboss.h
 * \brief   ラスボスのクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

 /**
  * @class LastBoss
  * @brief ラスボスクラス
  */
class LastBoss : public CharBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	LastBoss() {};
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
	 * @brief プレイヤーとの距離判定
	 * @return true
	 */
	bool RangeJ();
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

	/**
	 * @brief プレイヤーとの距離状態
	 */
	enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

protected:
	float animSpd,     //!アニメーションのスピード
		stanTime;   //!スタン時間
	int  time,        //!待機時間
		attackStep;   //!行動番号
	bool isAnimEnd;   //アニメーション終わったかどうか
	modelInf* plMI;   //!プレイヤー情報のインスタンス
	bool actionFlag;  //!行動してるかどうか
	bool hitFlag;
	int  swingSE,     //!斬撃SEハンドル
		newSomenHandle; //エフェクトハンドル
	float playerDir,       //!プレイヤーの方向
		playerRange,       //!プレイヤーとの距離
		playerDistance;      //!プレイヤーとの距離その２
	bool awake;       //!覚醒したかどうか
	float awakeSpd,   //!覚醒時スピード
		awakeDmg,     //!覚醒時追加ダメージ
		awakeMove,    //!覚醒時移動量
		awakeTime,       //!覚醒時待機時間
		awakeAddDistance;      //!覚醒時攻撃モーション移動量追加
	int impactEfcHandle; //!衝撃波エフェクト用ハンドル
	int hitTime;
	int voiceStartNum;   //!何番目のボイスを再生するか
};