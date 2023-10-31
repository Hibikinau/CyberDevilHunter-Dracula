#pragma once
#include"charBase.h"
#include "picojson/picojson.h"

namespace {
	constexpr auto BossUtility = "res/ResourcePathData.json";
}

class BossBase : public CharBase 
{
public:
	enum class TYPE
	{
		KNIGHT,
		LION,
		LAST
	};
	TYPE type;

	/**
	 * @brief プレイヤーとの距離状態
	 */
	enum class RANGE
	{
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

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
	
	BossBase() :CharBase() {};
	~BossBase() {};

	bool loadJson(TYPE _type);
	/**
	 * @brief プレイヤーとの距離判定
	 * @return true
	 */
	float RangeJ();
	/**
 * @brief キャラクターの移動
 * @param speed 移動速度
 * @param radian 移動方向
 * @return true
 */
	void Move(float speed, float radian);

	//STATUS UtilityJudge() {}
protected:
	float animSpd,     //!アニメーションのスピード
		stanTime;   //!スタン時間
	int  time,        //!待機時間
		attackStep;   //!行動番号
	int  swingSE,     //!斬撃SEハンドル
		newSomenHandle; //エフェクトハンドル
	float playerDir,       //!プレイヤーの方向
		playerRange,       //!プレイヤーとの距離
		playerDistance;      //!プレイヤーとの距離その２
	VECTOR Pvector;               //!プレイヤーとの距離の数
	bool awake;       //!覚醒したかどうか
	float awakeSpd,   //!覚醒時スピード
		awakeDmg,     //!覚醒時追加ダメージ
		awakeMove,    //!覚醒時移動量
		awakeTime,       //!覚醒時待機時間
		awakeAddDistance;      //!覚醒時攻撃モーション移動量追加
	modelInf* plMI; //!プレイヤー情報のインスタンス
	bool  isAnimEnd;       //アニメーション終わったかどうか
};