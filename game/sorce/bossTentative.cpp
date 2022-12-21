#include"bossTentative.h"
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 1
#define motion_run 2


bool Boss::Initialize()
{
	_handle = MV1LoadModel("game/res/reimu/にがもん式霊夢V01_s05a/霊夢 [にがもん式]Ver 0.1_s05a.pmx");
	//Anim_handle = MV1LoadModel("game/res/reimu/taiki.mv1");
	//Anim_handle2 = MV1LoadModel("game/res/reimu/aruku.mv1");
	_attach_index = -1;		// アニメーションアタッチはされていない

	MV1SetScale(_handle, VGet(10, 10, 10));
	//輪郭線の大きさを修正する
	int MaterialNum = MV1GetMaterialNum(_handle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// マテリアルの輪郭線の太さを取得
		float dotwidth = MV1GetMaterialOutLineDotWidth(_handle, i);
		// マテリアルの輪郭線の太さを拡大した分小さくする
		MV1SetMaterialOutLineDotWidth(_handle, i, dotwidth / 10);
	}
	status = STATUS::NONE;
	// 再生時間の初期化
	_total_time = 0.f;
	_play_time = 0.0f;
	// 位置,向きの初期化
	_vPos = VGet(0.0f, 0.0f, 15000.f);
	_vDir = VGet(0, 0, 1);		// キャラモデルはデフォルトで-Z方向を向いている
	// 腰位置の設定
	_colSubY = 40.f;

	return true;
}

bool	Boss::Terminate()
{

	return true;
}

bool	Boss::Input()
{

	return true;
}

bool	Boss::Process()
{
	float addDir = 0.f;
	STATUS oldStatus = status;
	status = STATUS::WAIT;
	// ステータスが変わっていないか？
	if (oldStatus == status) {
		// 再生時間を進める
		_play_time += 0.5f;
	}
	else {
		// アニメーションがアタッチされていたら、デタッチする
		if (_attach_index != -1) {
			MV1DetachAnim(_handle, _attach_index);
			_attach_index = -1;
		}
		// ステータスに合わせてアニメーションのアタッチ
		switch (status) {
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_handle, 0, Anim_handle, FALSE);
			//(*iteChara)->_attach_index = MV1AttachAnim((*iteChara)->_handle, MV1GetAnimIndex((*iteChara)->_handle, "Anim000"), -1, FALSE);
			break;
		case STATUS::WALK:
			_attach_index = MV1AttachAnim(_handle, 0, Anim_handle2, FALSE);
			//(*iteChara)->_attach_index = MV1AttachAnim((*iteChara)->_handle, MV1GetAnimIndex((*iteChara)->_handle, "Anim001"), -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		// 再生時間を初期化
		_play_time = 0.0f;
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (_play_time >= _total_time) {
		_play_time = 0.0f;
	}


	return true;
}

bool	Boss::Render()
{
	// 再生時間をセットする
	MV1SetAttachAnimTime(_handle, _attach_index, _play_time);

	// 位置
	MV1SetPosition(_handle, _vPos);
	// 向きからY軸回転を算出
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	MV1SetRotationXYZ(_handle, vRot);
	// 描画
	MV1DrawModel(_handle);
	return true;
}

void Boss::charMove(float Speed, float _Dir)
{

}

bool Boss::step()
{
	return true;
}
