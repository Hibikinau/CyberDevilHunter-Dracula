#include"bossTentative.h"
#include <math.h>
#define walkSpd 6.f
#define runSpd 20.f
#define motion_idel 0
#define motion_walk 1
#define motion_run 2


bool Boss::Initialize()
{
	_modelManager.modelImport("game/res/reimu/nigareimu/nigareimu.pmx", 10.0f, &_modelInf);
	//_handle = MV1LoadModel("game/res/reimu/nigareimu/nigareimu.pmx");
	//Anim_handle = MV1LoadModel("game/res/nigareimu/taiki.mv1");
	//Anim_handle2 = MV1LoadModel("game/res/nigareimu/aruku.mv1");
	//Anim_handle3 = MV1LoadModel("game/res/nigareimu/kick.mv1");
	//_attach_index = -1;		// アニメーションアタッチはされていない
	useAnim = 0;
	//MV1SetScale(_handle, VGet(10, 10, 10));
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
	//_vPos = VGet(0.0f, 0.0f, 15000.f);
	//_vDir = VGet(0, 0, 1);		// キャラモデルはデフォルトで-Z方向を向いている

	_modelInf.importCnt = 0;
	_modelInf.pos = VGet(0.0f, 0.0f, 500.f);
	_modelInf.dir = VGet(0.0f, 180.0f, 0.0f);
	// 腰位置の設定
	_colSubY = 40.f;
	return true;
}

bool	Boss::Terminate()
{
	CB::Terminate();
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

	for (auto i = charBox->begin(); i != charBox->end(); i++) {
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
		}
	}


	//変更点
	auto x = plMI->pos.x - _modelInf.pos.x;
	auto y = plMI->pos.y - _modelInf.pos.y;
	dir=atan2(x,y);



	if (_vPos.x < plMI->pos.x) {
		status = STATUS::KICK;
	}
	else {
		status = STATUS::WAIT;
	}

	// ステータスが変わっていないか？
	if (oldStatus == status) {
		// 再生時間を進める
		//_play_time += 0.5f;
	}
	else {
		//// アニメーションがアタッチされていたら、デタッチする
		//if (_attach_index != -1) {
		//	//MV1DetachAnim(_handle, _attach_index);
		//	_attach_index = -1;
		//}
		// ステータスに合わせてアニメーションのアタッチ
		switch (status) {
		case STATUS::WAIT:
			_modelManager.animChange(motion_idel, &_modelInf, true, true);
			spd = 0.f;
			animSpd = 0.5f;
			break;
		case STATUS::WALK:
			_modelInf.playTime = 0.f;
			_modelManager.animChange(1, &_modelInf, false, false);
			animSpd = 0.5f;
			break;
		case STATUS::KICK:
			_modelInf.playTime = 0.f;
			_modelManager.animChange(2, &_modelInf, false, false);
			animSpd = 1.0f;
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		//_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		// 再生時間を初期化
		//_play_time = 0.0f;
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	//if (_play_time >= _total_time) {
		//_play_time = 0.0f;
	//}
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;

	return true;
}

bool	Boss::Render()
{
	//DrawFormatString(600, 20, GetColor(0, 0, 0), "%f",dir);
	isAnimEnd = _modelManager.modelRender(&_modelInf, animSpd);
	return true;
}

void Boss::charMove(float Speed, float _Dir)
{

}

bool Boss::step()
{
	return true;
}
