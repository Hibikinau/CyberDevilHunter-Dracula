/*****************************************************************//**
 * \file    bossKnight.h
 * \brief   ボス「騎士」のクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"player.h"

 /**
	* @class BossKnight
	* @brief ボス「騎士」クラス
	*/
class BossKnight : public CharBase
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
	 * @brief エフェクト読み込み用関数
	 */
	void loadEfekseer() override;

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
		SRASH,
		SLAM,
		STAB,
		ROBES,
		JAMPACT,
		ONESLASH,
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
	float animSpd,         //!アニメーションのスピード
		  stanTime;        //!スタン時間
	int   time,            //!待機時間
		  attackStep,      //!行動番号
		  slamEfcHandle;
	bool  isAnimEnd;       //アニメーション終わったかどうか
	modelInf* plMI;        //!プレイヤー情報のインスタンス
	bool  ActionFlag;      //!行動してるかどうか
	bool  jumpActFlag;     //!ジャンプ行動してるか
	bool  PosFlag;
	bool  STABFlag;        //!突進攻撃してるかどうか
	int   swingSE,         //!斬撃SEハンドル
		  newSomenHandle;  //エフェクトハンドル
	float Pdir,            //!プレイヤーの方向
		  Prange,          //!プレイヤーとの距離
		  PrangeA;         //!プレイヤーとの距離その２
	bool  Awake;           //!覚醒したかどうか
	float AwakeSpd,        //!覚醒時スピード
		  AwakeDmg,        //!覚醒時追加ダメージ
		  AwakeMove,       //!覚醒時移動量
		  AwakeT;          //!覚醒時待機時間
	int   impactEfcHandle; //!衝撃波エフェクト用ハンドル
	VECTOR Efa, Efb, Efaz, Efbz;
};