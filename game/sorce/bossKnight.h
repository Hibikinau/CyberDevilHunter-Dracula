/*****************************************************************//**
 * \file    bossKnight.h
 * \brief   ボス「騎士」のクラス
 *
 * \author  松田　裕
 * \date    March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
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
	//bool RangeJ();
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
	
	K_STATUS status;

	/**
	 * @brief プレイヤーとの距離状態
	 */
	/*enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;*/

protected:
	float animSpd,       //!アニメーションのスピード
		stanTime;        //!スタン時間
	int   time,          //!待機時間
		attackStep;      //!行動番号
	bool  isAnimEnd;       //アニメーション終わったかどうか
	modelInf* plMI;        //!プレイヤー情報のインスタンス
	bool  actionFlag;      //!行動してるかどうか
	bool  jumpActFlag;     //!ジャンプ行動してるか
	bool  posFlag;
	bool  STABFlag;        //!突進攻撃してるかどうか
	int   swingSE,         //!斬撃SEハンドル
		newSomenHandle;  //エフェクトハンドル
	float playerDir,       //!プレイヤーの方向
		playerRange,       //!プレイヤーとの距離
		playerDistance;    //!プレイヤーとの距離その２
	bool  awake;           //!覚醒したかどうか
	float awakeSpd,        //!覚醒時スピード
		awakeDmg,        //!覚醒時追加ダメージ
		awakeMove,       //!覚醒時移動量
		awakeWaitTime;     //!覚醒時待機時間
};