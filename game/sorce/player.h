/*****************************************************************//**
 * \file   player.h
 * \brief  プレイヤークラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"

 /**
	* @enum ExclusiveState
	* キャラの行動状態
	*/
enum class ExclusiveState
{
	NORMAL,//!初期状態
	JUMP,//!ジャンプ
	quickATTACK,//!弱攻撃
	slowATTACK,//!強攻撃
	changeATTACKX,//Xボタン側入れ替え技
	changeATTACKY,//Yボタン側入れ替え技
	DODGE,//!回避
	GUARD,//!ガード
	DAMAGE,//!被弾時やられ
	finishAttack,//!覚醒時フィニッシュ技
};

enum class pushButton
{
	Neutral,//!非入力
	A,//!Aボタン
	B,//!Bボタン
	X,//!Xボタン
	Y,//!Yボタン
	R1,//!R1ボタン
	LBX,//!L1 + Xボタン
	LBY,//!L1 + Yボタン
	LBA,//!L1 + Aボタン
	Lstick,//!Lsrick入力
	Irregular,//!例外
	Damage,//!被弾時例外
	finishAttack,//!覚醒時フィニッシュ技例外
};

/**
 * @class player
 * @brief プレイヤクラス
 */
class player : public CharBase
{
public:
	player() {};
	~player() {};

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
	 * @brief キャラ移動処理
	 * @param Speed 移動スピード
	 * @param _Dir 移動方向
	 * @param isAnimChange runアニメーションを再生するか
	 */
	void charMove(float Speed, float _Dir, bool isAnimChange);

	/**
	 * @brief 入力を基準にプレイヤの行動を返す処理
	 * \return 次キャラクタの行動
	 */
	pushButton setAction();

	/**
	 * @brief キーボード/PAD入力のトリガ入力確認処理
	 * @param Key キー
	 * @param Pad ボタン
	 * @return 入力されているか
	 */
	bool checkTrgImput(int Key, int Pad) { if (_imputInf->_gTrgb[Key] || _imputInf->_gTrgp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief キーボード/PAD入力の入力確認処理
	 * @param Key キー
	 * @param Pad ボタン
	 * @return 入力されているか
	 */
	bool checkKeyImput(int Key, int Pad) { if (_imputInf->_gKeyb[Key] || _imputInf->_gKeyp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief キーボード/PAD入力のリリース入力確認処理
	 * @param Key キー
	 * @param Pad ボタン
	 * @return 入力されているか
	 */
	bool checkRelImput(int Key, int Pad) { if (_imputInf->_gRelb[Key] || _imputInf->_gRelp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief Lstick入力からキャラの移動方向を返す処理
	 * @param checkUseCamDir ロックオンカメラ基準で返すかどうか
	 * @return 移動方向
	 */
	float getMoveDir(bool checkUseCamDir);

	/**
	 * @brief 体力計算処理
	 * @param 数値
	 * @return true
	 */
	bool HPmath(float math, float Stan) override;

	/**
	 * @brief ブラッドポイント計算処理
	 * @param 数値
	 * @return true
	 */
	bool BPmath(float math) override;

	/**
	 * @brief エフェクト読み込み用関数
	 */
	void loadEfekseer() override;

	/**
	 * @brief 自機入れ替え技変更処理
	 * @param name 入れ替え技名
	 * @param XorY 2つあるうちどちらの入れ替え技を変えるか
	 * @return true
	 */
	bool CA_change(std::string name, const char* XorY) override;

	/**
	 * @brief 入れ替え技(突き)
	 * @param insPL プレイヤクラスポインタ
	 * @return true
	 */
	static bool CA_charge(player* insPL);

	/**
	 * @brief 入れ替え技(切り抜け)
	 * @param insPL プレイヤクラスポインタ
	 * @return true
	 */
	static bool CA_kirinuke(player* insPL);

	/**
	 * @brief 入れ替え技(デバック用攻撃)
	 * @param insPL プレイヤクラスポインタ
	 * @return true
	 */
	static bool CA_debugAttack(player* insPL);

	/**
	 * @brief 入れ替え技(非設定)
	 * @param insPL プレイヤクラスポインタ
	 * @return false
	 */
	static bool CA_noData(player* insPL);

	/**
	 * @brief 入れ替え技用関数ポインタ
	 * @param insPL プレイヤクラスポインタ
	 */
	bool (*changeAttackX)(player* insPL);

	/**
	 * @brief 入れ替え技用関数ポインタ
	 * @param insPL プレイヤクラスポインタ
	 */
	bool (*changeAttackY)(player* insPL);

	/**
	 * @brief アニメーション総再生時間取得処理
	 * @return アニメーション総再生時間
	 */
	float getAnimPlayTotalTime() { return _modelInf.totalTime; };

	/**
	 * @brief メインゲーム終了時ボイス処理
	 */
	void battleEndVoice() override { PlaySoundMem(soundHandle[voiceStartNum + 41 + rand() % 2], DX_PLAYTYPE_BACK); }

	/**
	 * @brief メインゲーム開始時ボイス処理
	 */
	void battleStartVoice() override { PlaySoundMem(soundHandle[voiceStartNum + 35 + rand() % 2], DX_PLAYTYPE_BACK); }

	/**
	 * @brief 入れ替え技のリキャスト時間設定
	 */
	bool recastSet()
	{
		if (lastAttackState == ExclusiveState::changeATTACKX && caRecastX <= 0) { caRecastX = setRecastTime; }
		else if (lastAttackState == ExclusiveState::changeATTACKY && caRecastY <= 0) { caRecastY = setRecastTime; }
		else { return false; }
		setRecastTime = 0;
		return true;
	};

	/**
	 * @brief 飛ぶ斬撃用vectorの数値計算
	 * @param _underPos　当たり判定カプセルの根元位置
	 * @param _overPos　当たり判定カプセルの先端位置
	 * @param _moveDir 当たり判定カプセルの移動方向
	 * @param insPL　プレイヤクラスポインタ
	 * @return true
	 */
	void mathFlyingSlashPos(VECTOR* _underPos, VECTOR* _overPos, VECTOR* _moveDir, player* insPL)
	{
		*_underPos = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y - 90, 300));
		*_overPos = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y + 90, 300));
		*_moveDir = getDirVecP(insPL->_modelInf.dir.y, 30);
		_underPos->y = _overPos->y = insPL->_modelInf.pos.y + 200;
	}

protected:
	int attackNumOld//!弱/強攻撃が何段目か
		, waitNextAttack//!弱/強攻撃受付時間
		, isCharge//!攻撃のチャージ中か(0.チャージしてない 1.チャージ中 2.解放)
		, chargeLevel//!攻撃のチャージ段階
		, chargeTime//!チャージ時間
		, isAwakening = 0//覚醒中か
		, voiceStartNum//!音声データコンテナ内の声データの初期値
		//!各種エフェクト用ハンドル
		, guardEfcHandle, chargeEfcHandle, healEfcHandle, impactEfcHandle, insGuardEfcHandle, insHealEfcHandle
		, waitBlowTime//!吹き飛ばされモーション用の受付時間
		, immortalTime//!無敵時間
		, isCounter = 0//!カウンター状態か
		, walkTime//!キャラ移動開始からの経過時間
		, counterTime//!カウンター再生時間
		, waitCAChargeTime//!入れ替え技での移動までの待機時間
		, CAChargeTime//!入れ替え技での移動時間
		, CAChargeSpd//!入れ替え技での移動スピード
		, CAChargeAttackNum = 0//!入れ替え技のチャージ段階
		, awakeSeCoolTime = 0;//!覚醒可能時SEを連続再生させないための変数
	float spd//!キャラの移動速度
		, dodgeDir//!キャラの回避方向
		, atkBuff = 0.f;//!加算攻撃バフ値
	bool isAnimEnd//!アニメーション再生が終わっているか
		, isPushButtonAct//!弱攻撃連打から攻撃受付時間終わりまでに別アクションに切り替えたか
		, isGuard//!ガード中か
		, isFastGuard//!ガード準備中か
		, isBlow//!吹き飛ばされモーション中か
		, isGhost//!別キャラとのすり抜けを許可するか
		, deadVoice = false;//!死亡音声の再生はしたか
	ExclusiveState Estate = ExclusiveState::NORMAL//!キャラの行動状態インスタンス
		, oldEstate = ExclusiveState::NORMAL//!1つ前のキャラの行動状態インスタンス
		, lastAttackState;//!キャラの最終攻撃インスタンス
	pushButton nextKey;//!先行入力用インスタンス
};
