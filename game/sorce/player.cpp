/*****************************************************************//**
 * \file   player.cpp
 * \brief  プレイヤクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"player.h"

 //プレイヤのrun状態での移動速度
#define runSpd 40.f

//各攻撃のダメージ参照用番号設定
#define jakuNum 0
#define kyouNum 1
#define counterNum 2
#define debugNum 3
#define kirinukeNum 4
#define charge1Num 5
#define charge2Num 6
#define charge3Num 7
#define finishNum 8

//各攻撃の攻撃判定の指定
#define defaultAttackUnderPos VGet(0.f, 0.f, 0.f)
#define defaultAttackOverPos VGet(0.f, 0.f, 300.f)
#define skillAttackOverPos VGet(0.f, 0.f, 200.f)

//武器追従フレーム番号設定
#define rWeponParentFrame 192
#define lWeponParentFrame 167

typedef ExclusiveState _estate;
using namespace model;

bool player::Initialize()
{
	//各変数の初期化
	spd = runSpd;
	type = 1;
	g = 1.f;
	isCharge = 0;
	Estate = _estate::NORMAL;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 200;
	_statusInf.maxBloodPoint = 1500;
	_statusInf.bloodPoint = 0;

	//開始時点の自機の位置と方向設定
	_modelInf.pos = VGet(210.0f, 1100.0f, 3100.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//モデルの読み込み
	modelImport("game/res/Player01/Player1.mv1", 1.5f, &_modelInf, RS);
	weponAttach("game/res/Weapon_Katana/Weapon_katana.mv1", &_modelInf, rWeponParentFrame, 2.f, true, "katana", RS);
	weponAttach("game/res/Weapon_Saya/Weapon_saya.mv1", &_modelInf, lWeponParentFrame, 2.f, true, "saya", RS);
	weponAttach("game/res/Weapon_noutou/Weapon_noutou.mv1", &_modelInf, lWeponParentFrame, 2.f, false, "noutou", RS);

	//入れ替え技設定
	CA_change(_valData->plChangeAttackX, "X");
	CA_change(_valData->plChangeAttackY, "Y");

	//音声データの読み込み
	for (std::string seNameList : _valData->playerSeList)
	{
		std::string insName = "game/res/SE/player/" + seNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}
	voiceStartNum = soundHandle.size();
	for (std::string voiceNameList : _valData->playerVoiceList)
	{
		std::string insName = "game/res/voice/player/" + voiceNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}

	return true;
}

void player::loadEfekseer()
{
	//エフェクト読み込み
	guardEfcHandle = LoadEffekseerEffect("game/res/effect/ガード/guard_effect_01.efkefc", 20.f);
	chargeEfcHandle = LoadEffekseerEffect("game/res/effect/チャージエフェクト/charge.efkefc", 20.f);
	healEfcHandle = LoadEffekseerEffect("game/res/effect/回復1/heal_01.efkefc", 60.f);
	impactEfcHandle = LoadEffekseerEffect("game/res/effect/衝撃波1/slash_shot.efkefc", 80.f);

	return;
}

bool	player::Terminate()
{
	CharBase::Terminate();
	//音声データのメモリ解放
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

/**
 * @brief 武器の納刀、抜刀増タイの切り替え
 * \param MI モデルデータ
 * \param isIO trueで抜刀falseで納刀
 * \return true
 */
bool KATANAIO(modelInf* MI, bool isIO)
{
	MI->wepons[0]->isActive = isIO;
	MI->wepons[1]->isActive = isIO;
	MI->wepons[2]->isActive = !isIO;

	return true;
}

bool	player::Process()
{
	//マスター音量の適応
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	//死亡時処理
	if (_statusInf.hitPoint <= 0)
	{
		isDead = 1;//死亡状態変数を死亡モーション再生状態に
		animSpd = .7f;//アニメーション再生速度設定
		_modelInf.animSpdBuff = .0f;//アニメーション再生速度バフ設定

		animChange(PL_DEATH, &_modelInf, false, true, false);//アニメーションを死亡モーションに変更
		//死亡音声の再生
		if (!deadVoice) { deadVoice = true; PlaySoundMem(soundHandle[voiceStartNum + 31 + rand() % 4], DX_PLAYTYPE_BACK); }
		//アニメーションの再生が終わったら死亡状態をキャラ削除状態に
		if (_modelInf.isAnimEnd) { isDead = 2; }
		return true;
	}

	//ブラッドポイントが1500以上にならないように
	if (_statusInf.bloodPoint > 1500.f)
	{
		_statusInf.bloodPoint = 1500.f;
	}

	//非覚醒時にL2/R2ボタンを押すと覚醒状態に移行
	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 0 && _statusInf.bloodPoint >= _statusInf.maxBloodPoint)
	{
		//覚醒用音声データの再生
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);
		isAwakening = 1, atkBuff = 100.f, _modelInf.animSpdBuff = .5f;
	}

	//L2/R2ボタンの入力値から連続で処理がされないように調整
	if (_imputInf->lTriggerX < 20 && _imputInf->rTriggerX < 20 && isAwakening == 1) { isAwakening = 2; }

	//覚醒中にブラッドポイントの減少、尽きた際の覚醒解除処理
	if (isAwakening > 0)
	{
		if (_statusInf.bloodPoint > 0) { _statusInf.bloodPoint--; }
		else
		{
			_statusInf.bloodPoint = 0;
			isAwakening = 0;
			atkBuff = 0.f;
			_modelInf.animSpdBuff = 0.f;
			PlaySoundMem(soundHandle[19], DX_PLAYTYPE_BACK);
		}
	}

	//キーボードでの自機移動処理(デバッグ用)
	if (CheckHitKey(KEY_INPUT_W)) { spd = runSpd; charMove(spd, *_cameraDir, false); }
	if (CheckHitKey(KEY_INPUT_A)) { spd = runSpd; charMove(spd, *_cameraDir + 270.f, false); }
	if (CheckHitKey(KEY_INPUT_S)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f, false); }
	if (CheckHitKey(KEY_INPUT_D)) { spd = runSpd; charMove(spd, *_cameraDir + 90.f, false); }

	//次の行動判定処理
	float addDir = 0.f, insDir, a;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::Damage://被弾
		//被弾モーション再生後一回だけする処理
		if (_modelInf.playTime <= animSpd)
		{
			dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
			isCharge = 0, isFastGuard = true, isGuard = true;
			nextKey = pushButton::Neutral;
			recastSet();
		}

		//吹き飛ばされ中の処理
		//吹き飛び判定が出ているか
		if (isBlow)
		{
			animSpd = 2.f;
			animChange(PL_huttobi1, &_modelInf, false, false, false);
			setNextAnim(PL_huttobi2, &_modelInf, false, false);
			if (_modelInf.playTime > 180.f) { Estate = _estate::NORMAL; }
			//吹き飛びモーション再生中か
			auto animName = _modelInf.animHandleOld;
			if (animName == PL_huttobi1)
			{
				//吹き飛びモーション再生後一回だけする処理
				if (_modelInf.playTime <= animSpd)
				{
					_modelInf.dir.y = getMoveDir(true);
					waitCAChargeTime = 20.f;
					CAChargeTime = 100.f;
					CAChargeSpd = -30.f;
				}
				//吹き飛びモーション中無敵時間の設定
				immortalTime = getAnimPlayTotalTime() - _modelInf.playTime;
				if (_modelInf.playTime > 140.f) { Estate = _estate::NORMAL; }
			}
		}
		else
		{
			//被弾判定は出ているが吹き飛び判定が出ていないので被弾モーションを再生
			animChange(PL_hirumi, &_modelInf, false, false, false);
			if (_modelInf.playTime > 25.f) { Estate = _estate::NORMAL; }
		}

		break;
	case pushButton::B://回避
		//設定処理
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
		animSpd = 4.f;
		spd = 70.f;
		isCharge = 0;
		recastSet();

		//回避方向の設定
		insDir = getMoveDir(false);
		if (insDir == 0) { insDir = _modelInf.dir.y; }
		dodgeDir = _modelInf.dir.y - insDir;
		if (dodgeDir > 360) { dodgeDir -= 360; }
		else if (dodgeDir < 0) { dodgeDir += 360; }

		//回避方向を基準にしたモーションの選択と再生
		if (dodgeDir >= 45 && 135 > dodgeDir) { animChange(PL_DODGE_L, &_modelInf, false, false, false); }//アニメーションを左回避モーションに変更
		else if (dodgeDir >= 135 && 225 > dodgeDir) { animChange(PL_DODGE_B, &_modelInf, false, false, false); }//アニメーションを後ろ回避モーションに変更
		else if (dodgeDir >= 225 && 315 > dodgeDir) { animChange(PL_DODGE_R, &_modelInf, false, false, false); }//アニメーションを右回避モーションに変更
		else { animChange(PL_DODGE_F, &_modelInf, false, false, false); }//アニメーションを前回避モーションに変更

		//無敵判定の設定
		dodgeTime = getAnimPlayTotalTime();
		immortalTime = dodgeTime;

		insDir != 0 ? dodgeDir = insDir : dodgeDir = _modelInf.dir.y;
		break;
	case pushButton::X://弱攻撃
		//設定処理
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::quickATTACK;
		lastAttackState = _estate::quickATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;

		//ボイス再生
		if (attackNumOld < 3) { PlaySoundMem(soundHandle[voiceStartNum + 0 + rand() % 4], DX_PLAYTYPE_BACK); }
		else { PlaySoundMem(soundHandle[voiceStartNum + 4 + rand() % 2], DX_PLAYTYPE_BACK); }

		//攻撃が何段目かの判定と攻撃処理
		if (attackNumOld == 0)
		{
			PlaySoundMem(soundHandle[12], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_1, &_modelInf, false, false, true);//アニメーションを弱攻撃１段目モーションに変更
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 1)
		{
			PlaySoundMem(soundHandle[13], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_2, &_modelInf, false, false, true);//アニメーションを弱攻撃２段目モーションに変更
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 2)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_3, &_modelInf, false, false, true);//アニメーションを弱攻撃３段目モーションに変更
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 3)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_4_1, &_modelInf, false, false, true);//アニメーションを弱攻撃４段目モーション１に変更
			setNextAnim(PL_JAB_4_2, &_modelInf, true, true);//次アニメーションに納刀をセット
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 4 && isPushButtonAct)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			_modelInf.animHandleNext = -1;
			animChange(PL_JAB_4_3, &_modelInf, false, false, true);//アニメーションを弱攻撃４段目モーション２に変更
			setNextAnim(PL_JAB_4_2, &_modelInf, true, false);//次アニメーションに納刀をセット
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld = 4;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, 12.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 17.f, 8.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 32.f, 10.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 47.f, 13.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 4 && !isPushButtonAct)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			_modelInf.animHandleNext = -1;
			animChange(PL_JAB_4_4, &_modelInf, false, false, true);//アニメーションを弱攻撃４段目モーション３に変更
			waitNextAttack = 0;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}

		break;
	case pushButton::Y://強攻撃
		//設定処理
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::slowATTACK;
		lastAttackState = _estate::slowATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;

		//攻撃が何段目かの判定と攻撃処理
		if (attackNumOld == 0)
		{
			animChange(PL_kyou_4_1, &_modelInf, false, false, true);//アニメーションを強攻撃１段目モーションに変更
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 1)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			animChange(PL_kyou_4_3, &_modelInf, false, false, true);//アニメーションを強攻撃２段目モーションに変更
			waitNextAttack = 0;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[kyouNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}

		break;
	case pushButton::LBX://入れ替えX
		//設定処理
		Estate = _estate::changeATTACKX;
		lastAttackState = _estate::changeATTACKX;
		attackNumOld = 0;
		changeAttackX(this);//入れ替え技Xの関数ポインタ呼び出し
		break;
	case pushButton::LBY://入れ替えY
		//設定処理
		Estate = _estate::changeATTACKY;
		lastAttackState = _estate::changeATTACKY;
		attackNumOld = 0;
		changeAttackY(this);//入れ替え技Xの関数ポインタ呼び出し
		break;
	case pushButton::finishAttack://必殺技
		//設定処理
		Estate = _estate::finishAttack;
		lastAttackState = _estate::finishAttack;
		attackNumOld = 0;
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);//ボイス再生
		animChange(PL_motion_hissatsu, &_modelInf, false, false, true);//アニメーションを覚醒時必殺技モーションに変更
		makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[finishNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		//覚醒時のステータス処理
		_statusInf.bloodPoint = 0, immortalTime = _modelInf.totalTime, _modelInf.animSpdBuff = 0.f;
		break;
	case pushButton::Lstick://ダッシュ
		//設定処理
		Estate = _estate::NORMAL;
		attackNumOld = 0;

		//移動先の角度をベクトルにして移動ベクトルに加算
		addDir = getMoveDir(false);
		spd = runSpd;//walkTime
		if (addDir != 0) { charMove(spd * (walkTime / 10), addDir, true); }
		moveCheck = false;
		break;
	case pushButton::R1://ガード
		//設定処理
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
		recastSet();

		//ガード使用時一回だけ呼ぶ処理
		if (Estate != _estate::GUARD && isFastGuard)
		{
			Estate = _estate::GUARD;
			insGuardEfcHandle = PlayEffekseer3DEffect(guardEfcHandle);
			SetPosPlayingEffekseer3DEffect(insGuardEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 100.f, _modelInf.pos.z);
			PlaySoundMem(soundHandle[17], DX_PLAYTYPE_BACK);

			counterTime = _valData->_counterTime;
		}
		//ガードをキャンセルして動けるように処理
		nextKey = pushButton::Neutral;

		//カウンター判定が出ているか判定
		if (isCounter == 1)
		{
			//設定処理
			insDir = getMoveDir(true);
			if (insDir != 0) { _modelInf.dir.y = insDir; }
			animSpd = 2.f;
			isFastGuard = true, isCounter = 2, counterTime = 0;
			immortalTime = getAnimPlayTotalTime();
			_valData->hitstopF = 5;//ヒットストップ
			//攻撃時処理
			KATANAIO(&_modelInf, true);//カタナの鞘をカタナにつけるか腰につけるかの設定変更
			animChange(PL_COUNTER, &_modelInf, false, false, true);//アニメーションをカウンターモーションに変更
			makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[counterNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			//飛ぶ斬撃への設定
			VECTOR underPos, overPos, modeDir;
			mathFlyingSlashPos(&underPos, &overPos, &modeDir, this);
			makeAttackCap(underPos, overPos, 40.f, 10.f, getAnimPlayTotalTime() - 10.f, animSpd, false, 75.f, 20, -1, modeDir, 0);

			//音声の再生
			PlaySoundMem(soundHandle[15], DX_PLAYTYPE_BACK);
			PlaySoundMem(soundHandle[voiceStartNum + 37 + rand() % 2], DX_PLAYTYPE_BACK);

			//エフェクト再生
			int a2 = PlayEffekseer3DEffect(impactEfcHandle);
			SetPosPlayingEffekseer3DEffect(a2, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
			SetRotationPlayingEffekseer3DEffect(a2, _modelInf.dir.x * (DX_PI_F / 180), _modelInf.dir.y * (DX_PI_F / 180), _modelInf.dir.z * (DX_PI_F / 180));
		}
		//ガード中だがカウンター判定が出ていない
		else if (isFastGuard && isCounter == 0)
		{
			animSpd = 10.f;
			animSpd = _valData->counterSpd;
			animChange(PL_GUARD_1, &_modelInf, false, false, false);//アニメーションをガードモーション１に変更
		}
		else if (isCounter != 2)
		{
			animSpd = 1.f;
			animChange(PL_GUARD_2, &_modelInf, true, false, false);//アニメーションをガードモーション２に変更
			Estate = _estate::NORMAL;
			BPmath(-1);
		}
		break;
	case pushButton::Neutral://入力なし
		//一部判定中待機モーションに移行しないよう設定
		if (_modelInf.animHandleOld == PL_huttobi1 || _modelInf.animHandleOld == PL_huttobi2) { break; }
		if (attackNumOld > 0) { break; }

		//待機モーションに移行
		Estate = _estate::NORMAL;
		animChange(PL_IDLE, &_modelInf, true, true, false);//アニメーションを待機モーションに変更
		spd = 0.f;
		break;
	default://その他
		break;
	}

	//入れ替え技や回避などの移動処理
	if (waitCAChargeTime > 0) { waitCAChargeTime -= animSpd + _modelInf.animSpdBuff; }
	else if (CAChargeTime > 0)
	{
		waitCAChargeTime = 0;
		CAChargeTime -= animSpd + _modelInf.animSpdBuff;
		charMove(CAChargeSpd, _modelInf.dir.y, false);
	}
	else { CAChargeTime = 0; }

	if (dodgeTime > 0)
	{
		if (_modelInf.playTime < _modelInf.totalTime - (_modelInf.totalTime / 6.f) && _modelInf.playTime > _modelInf.totalTime / 6.f)
		{
			animSpd = 2.f;
			charMove(spd, dodgeDir, false);
		}
		else { animSpd = 3.f; }
		dodgeTime -= animSpd;
	}

	//ボスと重ならないように
	for (auto i = charBox->begin(); i != charBox->end() && !isGhost; ++i)
	{
		if (i->second->type == 1) { continue; }
		auto  bossDisVector = VSub(i->second->getInf()->pos, _modelInf.pos);
		float bossDisFloat = sqrt(bossDisVector.x * bossDisVector.x + bossDisVector.y * bossDisVector.y + bossDisVector.z * bossDisVector.z);
		float plzDis = i->second->collCap.r + collCap.r;
		if (bossDisFloat < plzDis)
		{
			_modelInf.pos = VAdd(VScale(VNorm(bossDisVector), -plzDis), i->second->getInf()->pos);
			CAChargeTime = 0;
		}
	}

	//ベクトル計算などの舞フレーム必要な処理
	immortalTime > 0 ? immortalTime-- : immortalTime = 0;
	waitNextAttack > 0 ? waitNextAttack-- : attackNumOld = 0;
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;
	isImmortal = immortalTime > 0;
	collCap.r = 30.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 30, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 190, 0));
	int insNum;
	isAwakening > 0 ? insNum = 10 : insNum = 1;
	caRecastX > 0 ? caRecastX -= insNum : caRecastX = 0;
	caRecastY > 0 ? caRecastY -= insNum : caRecastY = 0;
	waitBlowTime > 0 ? waitBlowTime-- : waitBlowTime = 0;
	awakeSeCoolTime > 0 ? awakeSeCoolTime-- : awakeSeCoolTime = 0;

	//攻撃ヒット時のSE再生
	if (isHit)
	{
		BPmath(100);
		isHit = false;
	}

	//modeGameでのカウンター受付時間再生用
	_valData->plCTimeN = counterTime;

	return true;
}

bool	player::Render(float timeSpeed)
{
	int i = 0;
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);//アニメーション再生

	//回復、ガードエフェクトがキャラに追従するように
	if (IsEffekseer3DEffectPlaying(insGuardEfcHandle) == 0)
	{
		SetPosPlayingEffekseer3DEffect(insGuardEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 100.f, _modelInf.pos.z);
	}
	if (IsEffekseer3DEffectPlaying(insHealEfcHandle) == 0)
	{
		SetPosPlayingEffekseer3DEffect(insHealEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
	}
	return true;
}

void player::charMove(float Speed, float _Dir, bool isAnimChange)
{
	//設定処理
	_Dir -= 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	//キャラ移動時アニメーション変更処理
	if (isAnimChange)
	{
		animChange(PL_RUN, &_modelInf, true, true, false);//アニメーションを走りモーションに変更
		animSpd = 1.f;
		_modelInf.dir.y = _Dir + 180.f;
	}
}

bool player::HPmath(float math, float Stan)
{
	isBlow = false;
	//ダメージか回復かの判定
	if (math < 0)
	{
		//カウンター判定処理
		if (counterTime > 0) { isCounter = 1; }
		else if (immortalTime <= 0)
		{
			//ガード中か判定
			if (!isGuard || isFastGuard)
			{
				//覚醒時無敵判定
				if (isAwakening == 0)
				{
					if (!deadVoice) { PlaySoundMem(soundHandle[voiceStartNum + 27 + rand() % 4], DX_PLAYTYPE_BACK); }
					_statusInf.hitPoint += math; BPmath(std::abs(math) * 6);
				}
				PlaySoundMem(soundHandle[11], DX_PLAYTYPE_BACK);
				Estate = _estate::DAMAGE;
				//吹き飛びモーションに移行するかの判定
				if (math < -50 || waitBlowTime > 0) { isBlow = true; }
				else { waitBlowTime = 100; }
			}

			//ノックバック処理
			auto ACDisV = VSub(_modelInf.pos, charBox->find(attackChar)->second->_modelInf.pos);
			ACDisV = VNorm(ACDisV);
			_modelInf.vec = VScale(ACDisV, 50);
		}
	}
	else
	{
		//回復処理
		_statusInf.hitPoint += math;
		if (_statusInf.hitPoint > _statusInf.maxHitPoint) { _statusInf.hitPoint = _statusInf.maxHitPoint; }
	}

	//回避成功時ボイス再生
	if (dodgeTime > 0) { PlaySoundMem(soundHandle[voiceStartNum + 24 + rand() % 3], DX_PLAYTYPE_BACK); }

	return isBlow;
}

bool player::BPmath(float math)
{
	//覚醒時処理
	if (isAwakening > 0 && math > 0) { return false; }

	//BP増減処理
	_statusInf.bloodPoint += math;
	if (_statusInf.bloodPoint > _statusInf.maxBloodPoint)
	{
		_statusInf.bloodPoint = _statusInf.maxBloodPoint;
		if (awakeSeCoolTime <= 0) { PlaySoundMem(soundHandle[20], DX_PLAYTYPE_BACK);			awakeSeCoolTime = 240; }
	}
	if (_statusInf.bloodPoint < 0) { _statusInf.bloodPoint = 0; }

	return true;
}

pushButton player::setAction()
{
	//設定処理
	bool isNext = false;
	isPushButtonAct = false;
	pushButton insEnum = pushButton::Neutral;

	//一部モーション時判定スキップ
	if (Estate == _estate::DAMAGE) { return pushButton::Damage; }
	if (isCounter == 1) { return pushButton::R1; }

	//アニメーションの再生終了後の判定
	if (_modelInf.isAnimEnd)
	{
		isAnimEnd = false, isGhost = false, isBlow = false;
		StopJoypadVibration(DX_INPUT_PAD1);
		if (Estate != _estate::NORMAL && isCharge == 0 && !isGuard) { Estate = _estate::NORMAL; }
		if (isFastGuard) { isFastGuard = false; }
		if (isCounter == 2) { isCounter = 0; }
		if (CAChargeAttackNum > 0) { if (lastAttackState == _estate::changeATTACKX) { return pushButton::LBX; } else { return pushButton::LBY; } }
		else { CAChargeAttackNum = 0; }
		recastSet();
	}
	else if (Estate != _estate::NORMAL) { isNext = true; }

	//先行入力処理
	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1 && !isGuard && Estate != _estate::DODGE) { insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (checkTrgImput(KEY_INPUT_SPACE, XINPUT_BUTTON_B))//回避
	{
		attackNumOld = 0;
		Estate = _estate::DODGE;
		return insEnum = pushButton::B;
	}
	else if (checkKeyImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER) && dodgeTime <= 0)//ガード
	{
		if (isFastGuard && _modelInf.playTime > 17.5f) { KATANAIO(&_modelInf, false); }
		if (checkTrgImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER)) { isFastGuard = true, isGuard = true; }
		counterTime > 0 ? counterTime-- : counterTime = 0;
		return insEnum = pushButton::R1;
	}
	else
	{
		KATANAIO(&_modelInf, true);
		isGuard = false, counterTime = 0;
	}

	//移動
	if (checkKeyImput(KEY_INPUT_LSHIFT, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0)
	{
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
		walkTime < 10 ? walkTime++ : walkTime = 10;
	}
	else { walkTime = 0; }
	if (isNext) { insEnum = pushButton::Irregular; }

	//入れ替え技
	if (checkKeyImput(KEY_INPUT_C, XINPUT_BUTTON_LEFT_SHOULDER))
	{
		//X入力入れ替え技
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && caRecastX <= 0) {
			if (isNext) { if (Estate != _estate::changeATTACKX) { nextKey = pushButton::LBX; } }
			else { insEnum = pushButton::LBX; }
		}
		//Y入力入れ替え技
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && caRecastY <= 0) {
			if (isNext) { if (Estate != _estate::changeATTACKY) { nextKey = pushButton::LBY; } }
			else { insEnum = pushButton::LBY; }
		}
		//回復
		if (checkTrgImput(KEY_INPUT_V, XINPUT_BUTTON_A) && _statusInf.bloodPoint > 500.f)
		{
			HPmath(100, 10); BPmath(-500);
			insHealEfcHandle = PlayEffekseer3DEffect(healEfcHandle);
			SetPosPlayingEffekseer3DEffect(insHealEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
			PlaySoundMem(soundHandle[1], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		//弱攻撃
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X))
		{
			if (isNext) { if (attackNumOld != 4) { nextKey = pushButton::X; } }
			else { insEnum = pushButton::X; }
			isPushButtonAct = true;
		}
		//強攻撃
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::Y : insEnum = pushButton::Y; }
	}

	//X入力時入れ替え技のチャージ判定
	if (checkKeyImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && Estate == _estate::changeATTACKX)
	{
		if (isCharge >= 1) { insEnum = pushButton::LBX; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKX) { insEnum = pushButton::LBX, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }
	//Y入力時入れ替え技のチャージ判定
	if (checkKeyImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && Estate == _estate::changeATTACKY)
	{
		if (isCharge >= 1) { insEnum = pushButton::LBY; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKY) { insEnum = pushButton::LBY, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }

	//覚醒時フィニッシュ技
	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 2)
	{
		isNext ? nextKey = pushButton::finishAttack : insEnum = pushButton::finishAttack;
	}
	if (Estate == _estate::finishAttack) { return pushButton::Irregular; }

	//弱攻撃後無入力で出る抜刀攻撃
	if (waitNextAttack <= 0 && lastAttackState == _estate::quickATTACK && attackNumOld == 4)
	{
		insEnum = pushButton::X;
	}

	return insEnum;
}

float player::getMoveDir(bool checkUseCamDir)
{
	float _addDir = 0.f;

	//移動先の角度指定
	_addDir = (std::atan2(-_imputInf->lStickX, -_imputInf->lStickY) * 180.f) / DX_PI_F;
	if (_imputInf->lStickY != 0 && _addDir == 0.f) { _addDir = 360.f; }
	if (_addDir != 0) { _addDir += *_cameraDir + 180.f; }
	if (camDir != -1.f && checkUseCamDir)
	{
		_addDir = camDir;
	}

	return _addDir;
}

bool player::CA_change(std::string name, const char* XorY)
{
	//X入力時入れ替え技設定
	if ("X" == XorY)
	{
		if ("牙突" == name) { changeAttackX = &CA_charge; }
		if ("切抜" == name) { changeAttackX = &CA_kirinuke; }
		if ("竜閃" == name) { changeAttackX = &CA_debugAttack; }
		if ("NODATA" == name) { changeAttackX = &CA_noData; }
	}
	//Y入力時入れ替え技設定
	else if ("Y" == XorY)
	{
		if ("牙突" == name) { changeAttackY = &CA_charge; }
		if ("切抜" == name) { changeAttackY = &CA_kirinuke; }
		if ("竜閃" == name) { changeAttackY = &CA_debugAttack; }
		if ("NODATA" == name) { changeAttackY = &CA_noData; }
	}

	return true;
}


bool player::CA_debugAttack(player* insPL)
{
	//設定処理
	insPL->setRecastTime = 60;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	animChange(PL_motion_hissatsu, &insPL->_modelInf, false, false, true);//アニメーションを覚醒時必殺技モーションに変更
	insPL->animSpd = 1.f;
	insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, 200.f + insPL->atkBuff, 5.f, rWeponParentFrame, VGet(0, 0, 0), 1);

	//飛ぶ斬撃への設定
	VECTOR underPos, overPos, modeDir;
	insPL->mathFlyingSlashPos(&underPos, &overPos, &modeDir, insPL);
	insPL->makeAttackCap(underPos, overPos, 40.f, 10.f, 50 - 10.f, insPL->animSpd, false, insPL->_valData->plAtkNum[debugNum] + insPL->atkBuff, 5.f, -1, modeDir, 0);

	//エフェクト、SE再生
	int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
	SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
	SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));
	PlaySoundMem(insPL->soundHandle[6], DX_PLAYTYPE_BACK);
	return true;
}

bool player::CA_charge(player* insPL)
{
	//チャージ攻撃後の飛ぶ斬撃処理
	if (insPL->CAChargeAttackNum > 0)
	{
		//設定処理
		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
		insPL->animSpd = 3.f;
		PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
		animChange(PL_COUNTER, &insPL->_modelInf, false, false, true);//アニメーションをカウンターモーションに変更
		insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 16.f, 41.f, insPL->animSpd, true, insPL->_valData->plAtkNum[charge3Num] + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		insPL->waitCAChargeTime = 16.f;
		insPL->CAChargeTime = 41.f;
		insPL->CAChargeSpd = 0.f;
		insPL->immortalTime = insPL->_modelInf.totalTime;
		insPL->CAChargeAttackNum--;

		//飛ぶ斬撃への設定
		VECTOR underPos, overPos, modeDir;
		insPL->mathFlyingSlashPos(&underPos, &overPos, &modeDir, insPL);
		insPL->makeAttackCap(underPos, overPos, 40.f, 10.f, insPL->getAnimPlayTotalTime() - 10.f, insPL->animSpd, false, insPL->_valData->plAtkNum[debugNum] + insPL->atkBuff, 5.f, -1, modeDir, 0);

		//エフェクト、SE再生
		int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
		SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
		SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));
		PlaySoundMem(insPL->soundHandle[6], DX_PLAYTYPE_BACK);
		return true;
	}

	//コントローラー振動
	insPL->chargeTime < 1000 ? insPL->chargeTime += 12 : insPL->chargeTime = 100;
	StartJoypadVibration(DX_INPUT_PAD1, insPL->chargeTime, -1);

	//チャージ開始後一度だけの処理
	if (insPL->isCharge == 0)
	{
		insPL->isCharge = 1;
		insPL->chargeLevel = 0;
		insPL->chargeTime = 0;
		animChange(PL_arts_tsuki_1, &insPL->_modelInf, false, false, true);//アニメーションを突きモーション１に変更
		setNextAnim(PL_arts_tsuki_2, &insPL->_modelInf, true, true);//次モーションに突きモーション２をセット
	}
	//チャージ解放後処理
	if (insPL->isCharge == 2)
	{
		//コントローラー振動停止
		StopJoypadVibration(DX_INPUT_PAD1);

		//チャージ強度判定
		if (insPL->_modelInf.animHandleNext == -1)
		{
			insPL->chargeLevel = 2;
		}
		else if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 5.f)
		{
			insPL->chargeLevel = 1;
		}
		insPL->_modelInf.animHandleNext = -1;

		//設定処理
		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
		insPL->isCharge = 0;

		//最大チャージ時処理
		if (insPL->chargeLevel == 2)
		{
			insPL->animSpd = 3.f;
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			PlaySoundMem(insPL->soundHandle[4], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);//アニメーションを突きモーション３に変更
			insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 16.f, 41.f, insPL->animSpd, true, insPL->_valData->plAtkNum[charge3Num] + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 200.f;
			insPL->immortalTime = insPL->_modelInf.totalTime;
			insPL->CAChargeAttackNum = 2;
		}
		//無チャージ時処理
		else
		{
			insPL->animSpd = 3.f;
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			PlaySoundMem(insPL->soundHandle[4], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);//アニメーションを突きモーション３に変更

			//攻撃時設定適用
			float insDamage = insPL->atkBuff;
			insPL->chargeLevel == 1 ? insDamage += insPL->_valData->plAtkNum[charge2Num] : insDamage += insPL->_valData->plAtkNum[charge1Num];
			insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 16.f, 41.f, insPL->animSpd, true, insDamage, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 120.f;
		}
		insPL->isCharge = 0;
		insPL->chargeLevel = 0;
		insPL->setRecastTime = 60;
	}

	return true;
}

bool player::CA_kirinuke(player* insPL)
{
	//設定処理
	insPL->animSpd = 1.f;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	//SE、ボイス再生
	PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 22 + rand() % 2], DX_PLAYTYPE_BACK);
	PlaySoundMem(insPL->soundHandle[4], DX_PLAYTYPE_BACK);
	animChange(PL_arts_kirinuke, &insPL->_modelInf, false, false, true);//アニメーションを切り抜けモーションに変更

	//攻撃時設定適用
	insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, insPL->_valData->plAtkNum[kirinukeNum] + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
	insPL->waitCAChargeTime = 12.f;
	insPL->CAChargeTime = 53.f - insPL->waitCAChargeTime;
	insPL->CAChargeSpd = 40.f;
	insPL->isGhost = true;
	insPL->setRecastTime = 60;

	return true;
}

bool player::CA_noData(player* insPL) { return false; }