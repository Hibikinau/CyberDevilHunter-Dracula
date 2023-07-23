#pragma once
#include"charBase.h"
#include"player.h"

class targetEnemy : public CharBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	targetEnemy() {};
	/**
	 * @brief デストラクタ
	 */
	~targetEnemy() {};
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

protected:
	VECTOR siz;
};