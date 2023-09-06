/*****************************************************************//**
 * \file    bossLionman.h
 * \brief   ボス「ライオンマン」のクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

 /**
  * @class BossLionman
  * @brief ボス「ライオンマン」クラス
  */
class BossLion : public CharBase
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
		ATTACK,
		ATTACK2,
		ATTACK3,
		HANIATTACK,
		TACKLE,
		SLAM,
		DIVE,
	};
	STATUS status;

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
	float  animSpd,    //!アニメーションのスピード
	       stanTime;   //!スタン時間
	int    time,       //!待機時間
		   attackStep; //!行動番号
	bool   isAnimEnd;  //アニメーション終わったかどうか
	modelInf* plMI;    //!プレイヤー情報のインスタンス
	bool   ActionFlag; //!行動してるかどうか
	bool   hitFlag;
	int    swingSE,    //!斬撃SEハンドル
		   newSomenHandle;  //エフェクトハンドル
	float  Pdir,       //!プレイヤーの方向
		   Prange,     //!プレイヤーとの距離
		   PrangeA;    //!プレイヤーとの距離その２
	bool   Awake;      //!覚醒したかどうか
	float  AwakeSpd,   //!覚醒時スピード
		   AwakeDmg,   //!覚醒時追加ダメージ
		   AwakeMove,  //!覚醒時移動量
		   AwakeT,     //!覚醒時待機時間
	       AwakeTK;    //!覚醒時攻撃モーション移動量追加
	int    dodgeT,
		   hittime;
};
