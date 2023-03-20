/*****************************************************************//**
 * \file   charBase.h
 * \brief  キャラクターの基底クラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"
#include"motionDefineList.h"

 /**
	* @brief ステータス情報をまとめた構造体
	*/
struct statusInf
{
	float hitPoint//!体力値
		, stanPoint//!スタン値
		, maxHitPoint//!最大体力値
		, bloodPoint//!ブラッドポイント値
		, maxBloodPoint;//!最大ブラッドポイント値
};

/**
 * @brief 当たり判定情報をまとめた構造体
 */
struct collCapsule
{
	VECTOR underPos, overPos;//!当たり判定用カプセルの座標情報
	float r = -1;//!当たり判定用カプセルの半径情報
	int parentModelHandle//!追従させるモデルのハンドル
		, frameNum;//!追従させるモデルのフレーム番号
};

/**
 * @brief 剣の軌跡描画時に使う情報をまとめた構造体
 */
struct weaponEffect
{
	std::vector<VECTOR> upCornerPos, downCornerPos;//!剣の根元部分、剣先部分の座標情報
	float lifeTime = 10//!描画時間
		, maxLifeTime = 10;//!最大描画時間
	int efcPicHandle;//画像ハンドル
};

/**
 * @brief 攻撃判定情報をまとめた構造体
 */
struct attackColl
{
	collCapsule capColl, capCollOld;//!現在と1F前の当たり判定情報
	std::string attackChar;//!攻撃したキャラクタの名前
	MATRIX Mold;//!1F前の追従させるモデルのフレームの変換行列
	VECTOR Vec;
	float damage;//!ダメージ
	int nonActiveTimeF//!描画待機時間
		, activeTimeF;//!描画時間
	bool isUseMat//!モデルのフレームに追従させるか
		, isAlive = true;//!当たり判定が有効か
	weaponEffect rightingEfc;//!剣の軌跡情報
	float stan;//!スタン値
};

/**
 * @class CB
 * @brief キャラクタの基底クラス
 */
class CB
{
public:
	CB() {};
	virtual ~CB() {};

	/**
	 * @brief 初期化処理
	 * @return true
	 */
	virtual bool	Initialize() { return true; }

	/**
	 * @brief 削除処理
	 * @return modelDeleteが成功したか
	 */
	virtual bool	Terminate() { return model::modelDelete(&_modelInf); };

	/**
	 * @brief 更新処理
	 * @return true
	 */
	virtual bool	Process() { return true; }

	/**
	 * @brief 描画処理
	 * @param 再生速度
	 * @return true
	 */
	virtual bool	Render(float timeSpeed) { return true; }

	/**
	 * @brief 重力処理
	 * @return true
	 */
	virtual bool gravity();

	/**
	 * @brief キャラクタタイプの取得
	 * @return キャラクタのタイプ
	 */
	virtual int	getType() { return type; }

	/**
	 * @brief charBoxコンテナのポインタ取得
	 */
	void setCB(std::map<std::string, std::shared_ptr<CB> >* _CB) { charBox = _CB; }

	/**
	 * @brief リソースサーバーのポインタ取得
	 */
	void setRS(Rserver* _RS) { RS = _RS; }

	/**
	 * @brief 自機入れ替え技変更処理
	 * @param name 入れ替え技名
	 * @param XorY 2つあるうちどちらの入れ替え技を変えるか
	 * @return true
	 */
	virtual bool CA_change(std::string name, const char* XorY) { return false; };

	/**
	 * @brief 体力計算処理
	 * @param 数値
	 * @return true
	 */
	virtual bool HPmath(float Num, float Stan) { return false; };

	/**
	 * @brief ブラッドポイント計算処理
	 * @param 数値
	 * @return true
	 */
	virtual bool BPmath(float Num) { return false; };

	/**
	 * @brief メインカメラ位置情報セット処理
	 * @param 数値
	 */
	virtual void setCamDir(float Num) { camDir = Num; };

	/**
	 * @brief メインゲーム終了時ボイス処理
	 */
	virtual void battleEndVoice() { return; }

	/**
	 * @brief メインゲーム開始時ボイス処理
	 */
	virtual void battleStartVoice() { return; }

	/**
	 * @brief 名前情報セット処理
	 * @param 名前
	 * @return true
	 */
	void setName(const char* _name) { name = _name; }

	/**
	 * @brief 当たり判定処理
	 * @param name 攻撃キャラ情報
	 * @param hitPos 攻撃ヒット座標取得用ポインタ
	 * @param damage ダメージ取得用ポインタ
	 * @return true
	 */
	bool hitCheck(const char* name, VECTOR* hitPos, VECTOR* hitDir, float* damage);

	/**
	 * @brief キャラモデル情報取得
	 * @return キャラモデル情報
	 */
	modelInf* getInf() { return &_modelInf; }

	/**
	 * @brief ステージモデル情報取得
	 * @return ステージモデル情報
	 */
	void setGroundInf(modelInf* GE) { _GrounfInf = GE; }

	/**
	 * @brief ステータス情報取得
	 * @return ステータス情報
	 */
	statusInf	getStatus() { return _statusInf; }

	/**
	 * @brief 操作入力情報のセット
	 * @param iInf 操作入力情報セット用ポインタ
	 * @param cameraDir カメラ位置情報セット用ポインタ
	 */
	void getInputKey(imputInf* iInf, float* cameraDir) { _imputInf = iInf, _cameraDir = cameraDir; };

	/**
	 * @brief 当たり判定情報作成処理
	 * @param _underPos カプセル下部座標
	 * @param _overPos カプセル上部座標
	 * @param r カプセル半径
	 * @param nonActiveTimeF 待機時間
	 * @param activeTimeF 活動時間
	 * @param isUseMat モデルフレームに追従させるか
	 * @param damage 攻撃力
	 * @param frameNum 追従させるモデルフレームの番号
	 * @param charName キャラクタ名
	 * @param cameraDir カメラ位置情報セット用ポインタ
	 */
	bool makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r, int nonActiveTimeF, int activeTimeF, int timeSpeed, bool isUseMat, float damage, float stan, int frameNum, VECTOR _dir);

	/**
	 * @brief マスター音量セット処理
	 * @param masterValume マスター音量
	 */
	void setMasterVolume(int masterValume) { for (int handle : soundHandle) { ChangeVolumeSoundMem(255 * (0.01 * _valData->soundMasterValume), handle); } }

	int type = 0//!キャラクタタイプ(player = 1, boss = 2)
		, caRecastX = 0, caRecastY = 0, setRecastTime = 0;
	bool isGround//!設置しているか
		, isHit//!攻撃がヒットしたか
		, isSetSoundValume;//!マスター音量適用をするか
	float g//!重力値
		, camDir//!ロックオンカメラ位置情報
		, * _cameraDir//!カメラ位置情報
		, animSpd;//!アニメーション
	int isDead;//!キャラが志望常態か(0.生きてる 1.死亡モーション中 2.インスタンス解放)
	int isImmortal = false//!無敵状態か
		, dodgeTime;//!回避時間
	std::string name//!自キャラの名前
		, attackChar;//!攻撃してきたキャラの名前
	modelInf _modelInf//!モデル情報インスタンス
		, * _GrounfInf;//!ステージ情報インスタンス
	statusInf _statusInf;//ステータス情報インスタンス
	std::map<std::string, std::shared_ptr<CB> >* charBox;//!全キャラコンテナ用ポインタ
	valData* _valData;//!共有データ用ポインタ
	MV1_COLL_RESULT_POLY hitCheckGround;//ステージの当たり判定情報
	collCapsule collCap;//!当たり判定用カプセルのインスタンス
	std::vector<int> soundHandle;//!音声データ用コンテナ
	std::vector<attackColl>* allColl;//!当たり判定カプセル用コンテナ用ポインタ
	imputInf* _imputInf;//!入力データ用ポインタ
	Rserver* RS;//!リソースサーバークラス用ポインタ
};