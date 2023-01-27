#include"modelManager.h"

bool modelManager::modelImport(const char* dir, const float scale, modelInf* MI)
{
	SetUseASyncLoadFlag(TRUE);

	MI->modelHandle = MV1LoadModel(dir);

	SetUseASyncLoadFlag(FALSE);
	SetDrawScreen(DX_SCREEN_BACK);
	int i = 0;
	int B = GetASyncLoadNum();
	while (GetASyncLoadNum() > 0)
	{
		ProcessMessage();
		ClearDrawScreen();
		if (i < 20) { DrawString(640, 360, "loading.", GetColor(255, 255, 255)); }
		else if (i < 40) { DrawString(640, 360, "loading..", GetColor(255, 255, 255)); }
		else if (i < 60) { DrawString(640, 360, "loading...", GetColor(255, 255, 255)); }
		else { i = 0; }
		i++;
		ScreenFlip();
	}

	if (MI->modelHandle == -1) { return false; }
	MI->playTime = 0.0f;

	MV1SetPosition(MI->modelHandle, MI->pos);
	MV1SetRotationXYZ(MI->modelHandle, MI->dir);

	MV1SetScale(MI->modelHandle, VGet(scale, scale, scale));
	//輪郭線の大きさを修正する
	//通常MV1SetScaleで拡大すると，輪郭線の大きさも拡大されてしまう
	//全ての輪郭線にアクセスし，MV1SetScaleで拡大した値で割ることで，太さをもとに戻す
	int MaterialNum = MV1GetMaterialNum(MI->modelHandle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// マテリアルの輪郭線の太さを取得
		float dotwidth = MV1GetMaterialOutLineDotWidth(MI->modelHandle, i);
		// マテリアルの輪郭線の太さを拡大した分小さくする
		MV1SetMaterialOutLineDotWidth(MI->modelHandle, i, dotwidth / scale);
	}
	return true;
}

bool modelManager::weponAttach(const char* dir, modelInf* MI, const char* attachFrame, const float scale, bool activate, const char* name)
{
	weponModelInf weponMI;
	weponMI.isActive = activate;
	weponMI.name = name;
	weponMI.weponHandle = MV1LoadModel(dir);
	if (weponMI.weponHandle == -1) { return false; }
	weponMI.weponAttachFrameNum = MV1SearchFrame(MI->modelHandle, attachFrame);
	MV1SetScale(weponMI.weponHandle, VGet(scale, scale, scale));
	//輪郭線の大きさを修正する
	int MaterialNum = MV1GetMaterialNum(weponMI.weponHandle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// マテリアルの輪郭線の太さを取得
		float dotwidth = MV1GetMaterialOutLineDotWidth(weponMI.weponHandle, i);
		// マテリアルの輪郭線の太さを拡大した分小さくする
		MV1SetMaterialOutLineDotWidth(weponMI.weponHandle, i, dotwidth / scale);
	}
	MV1SetRotationXYZ(weponMI.weponHandle, VGet(0.f, 0.f, 0.f));
	MI->wepons.emplace_back(weponMI);
	return true;
}

bool modelManager::animChange(int _animHandle, modelInf* MI, bool isLoop, bool isBlend)
{
	if (MI->animHandleOld == _animHandle) { return false; }
	MI->isBrending = isBlend;
	MV1DetachAnim(MI->modelHandle, MI->attachIndex);
	MV1DetachAnim(MI->modelHandle, MI->attachIndexOld);
	MI->attachIndex = MV1AttachAnim(MI->modelHandle, _animHandle, -1, false);
	MI->attachIndexOld = MV1AttachAnim(MI->modelHandle, MI->animHandleOld, -1, false);
	MI->animHandleOld = _animHandle;
	MI->animOldLoop = isLoop;
	MI->playTimeOld = MI->playTime;
	MI->playTime = 0.f;

	MI->totalTime = MV1GetAttachAnimTotalTime(MI->modelHandle, MI->attachIndex);

	return true;
}

bool modelManager::modelRender(modelInf* MI, float animSpeed, float timeSpead)
{
	MI->isAnimEnd = false;
	if (MI->isBrending) { MI->rate = 0.f, MI->isBrending = false; }
	if (MI->rate <= 1.0f)
	{
		MI->rate >= 1.0f ? MI->rate = 1.0f : MI->rate += 0.1f * timeSpead;

		MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndexOld, MI->playTimeOld);
	}
	else
	{
		MI->playTime += animSpeed * timeSpead;
		if (MI->playTime >= MI->totalTime)
		{
			MI->isAnimEnd = true;
			if (MI->animOldLoop) { MI->playTime = 0.f; }
			else { MI->playTime = MI->totalTime; }
		}
	}

	MV1SetAttachAnimBlendRate(MI->modelHandle, MI->attachIndexOld, 1.0f - MI->rate);
	MV1SetAttachAnimBlendRate(MI->modelHandle, MI->attachIndex, MI->rate);

	MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndex, MI->playTime);

	MV1SetPosition(MI->modelHandle, MI->pos);
	MV1SetRotationXYZ(MI->modelHandle, VScale(MI->dir, (DX_PI_F / 180.0f)));
	MV1DrawModel(MI->modelHandle);

	for (auto _weponInf : MI->wepons)
	{
		if (!_weponInf.isActive) { continue; }
		_weponInf.weponFrameMatrix = MV1GetFrameLocalWorldMatrix(MI->modelHandle, _weponInf.weponAttachFrameNum);
		
		MV1SetMatrix(_weponInf.weponHandle, _weponInf.weponFrameMatrix);

		MV1DrawModel(_weponInf.weponHandle);
	}

	return MI->isAnimEnd;
}

bool modelManager::modelDelete(modelInf* MI)
{//装備モデルのメモリ解放してからコンテナの消去
	for (int i = 0; i < MI->wepons.size(); i++)
	{
		DeleteGraph(MI->wepons[i].weponHandle);
	}
	MI->wepons.clear();
	DeleteGraph(MI->modelHandle);//キャラモデルのメモリ解放
	return true;
}
