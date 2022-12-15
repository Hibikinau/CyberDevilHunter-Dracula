#include"modelManager.h"

bool modelManager::modelImport(const char* dir, const float scale, modelInf* MI)
{
	MI->modelHandle = MV1LoadModel(dir);
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

bool modelManager::animChange(int _animHandle, modelInf* MI)
{
	if (MI->animHandleOld == _animHandle) { return true; }
	MI->isBrending = true;
	MV1DetachAnim(MI->modelHandle, MI->attachIndex);
	MV1DetachAnim(MI->modelHandle, MI->attachIndexOld);
	MI->attachIndex = MV1AttachAnim(MI->modelHandle, _animHandle, -1, false);
	MI->attachIndexOld = MV1AttachAnim(MI->modelHandle, MI->animHandleOld, -1, false);
	MI->animHandleOld = _animHandle;

	MI->totalTime = MV1GetAttachAnimTotalTime(MI->modelHandle, MI->attachIndex);

	return true;
}

bool modelManager::modelRender(modelInf* MI)
{
	if (MI->isBrending) { MI->rate = 0.f, MI->isBrending = false; }
	if (MI->rate < 1.0f)
	{
		MI->rate += 0.05f;
		if (MI->rate > 1.0f)
		{
			MI->rate = 1.0f;
		}
		MV1SetAttachAnimBlendRate(MI->modelHandle, MI->attachIndexOld, 1.0f - MI->rate);
		MV1SetAttachAnimBlendRate(MI->modelHandle, MI->attachIndex, MI->rate);

		MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndexOld, MI->playTime);
	}

	MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndex, MI->playTime);

	MV1SetPosition(MI->modelHandle, MI->pos);
	MV1SetRotationXYZ(MI->modelHandle, VScale(MI->dir, (DX_PI_F / 180.0f)));
	MV1DrawModel(MI->modelHandle);

	return true;
}