/*****************************************************************//**
 * \file    bossBase.h
 * \brief   ボスの基底クラス
 *
 * \author  松田　裕
 * \date    November 2023
 *********************************************************************/
#pragma once
#include"charBase.h"
#include"modeGame.h"
//#include"ComponentBase.h"
#include <map>
#include <memory>
#include <typeindex>


class modeGame;
 /**
	* @class BossKnight
	* @brief ボスの基底クラス
	*/
class BossBase : public CharBase 
{
public:
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
	 * @brief コンストラクタ
	 */
	BossBase() :CharBase() {};
	/**
	 * @brief デストラクタ
	 */
	~BossBase() {};
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
	 * @brief 更新処理
	 * @return true
	 */
	virtual bool Process();
	/**
	 * @brief 描画処理
	 * @param 再生速度
	 * @return true
	 */
	virtual bool Render(float timeSpeed);
	/**
	 * @brief プレイヤーとの距離判定
	 * @return PlayerDir(プレイヤーの方向)
	 */
	float RangeJ();
	/**
　 * @brief キャラクターの移動
 　* @param speed 移動速度
 　* @param radian 移動方向
 　* @return true
 　*/
	void Move(float speed, float radian);
	/**
	 * @brief 体力計算処理
	 * @param 数値
	 * @return true
	 */
	bool HPmath(float Num, float Stan) override;

	int Load3DSoundMem(const char* FileName, int ReverbNo = 0, float InitRadius = 256.0f);

	
	//同型コンポーネントを追加する際は異なるIDを設定する
	//virtual void AddComponent(std::unique_ptr<ComponentBase> component, int id = 0);

	/**
	 * \brief 指定のコンポーネントを取得する　例：GetComponent<MV1Component>()
	 * \return コンポーネントのポインタを返す。所持していなければnullptrを返す。
	 */
	/*
	template <typename C>
	C* GetComponent(int id = 0) {
		C* result = nullptr;

		if (_components.count({ typeid(C), id }) != 0) {
			result = static_cast<C*>(_components[{typeid(C), id}].get());
		}
		return result;
	}
	*/
	/**
	 * \brief 指定のクラスか確認する　例：CheckType<Player>()、CheckType<Enemy>()
	 * \return 指定のクラスであればtrueを返す
	 */
	/*
	template <typename T>
	bool CheckType()const {
		if (typeid(T&) == typeid(*this)) {
			return true;
		}
		return false;
	}
	*/
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
	modeGame* _modeG;
	bool  isAnimEnd;       //アニメーション終わったかどうか
	//std::map<std::pair<std::type_index, int>, std::unique_ptr<ComponentBase>> _components;
};