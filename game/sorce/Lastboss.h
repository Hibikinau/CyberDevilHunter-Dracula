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
	virtual bool Initialize();
	/**
	 * @brief 削除処理
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
	 * @brief エフェクト読み込み用関数
	 */
	void loadEfekseer() override;

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
	float animSpd//!アニメーションのスピード
		, stanTime;//!スタン時間
	int  time,//!待機時間
		attackStep;//!行動番号
	bool isAnimEnd;//アニメーション終わったかどうか
	modelInf* plMI;//!プレイヤー情報のインスタンス
	bool ActionFlag;//!行動してるかどうか
	bool hitFlag;
	int  swingSE, //!斬撃SEハンドル
		newSomenHandle;//エフェクトハンドル
	float Pdir,//!プレイヤーの方向 
		Prange,//!プレイヤーとの距離
		PrangeA;//!プレイヤーとの距離その２
	bool Awake;//!覚醒したかどうか
	float AwakeSpd,//!覚醒時スピード
		AwakeDmg,//!覚醒時追加ダメージ
		AwakeMove,//!覚醒時移動量
		AwakeT,//!覚醒時待機時間
		AwakeTK;
	int impactEfcHandle;
	int hittime;
	int voiceStartNum;
};