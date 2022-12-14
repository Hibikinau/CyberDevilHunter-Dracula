#include"modelManager.h"

bool modelManager::modelImport(const char* dir, const float scale, modelInf* MI)
{
	MI->modelHandle = MV1LoadModel(dir);
	if (MI->modelHandle == -1) { return false; }
	MI->playTime = 0.0f;

	MV1SetPosition(MI->modelHandle, MI->pos);
	MV1SetRotationXYZ(MI->modelHandle, MI->dir);

	MV1SetScale(MI->modelHandle, VGet(scale, scale, scale));
	//—ÖŠsü‚Ì‘å‚«‚³‚ðC³‚·‚é
	//’ÊíMV1SetScale‚ÅŠg‘å‚·‚é‚ÆC—ÖŠsü‚Ì‘å‚«‚³‚àŠg‘å‚³‚ê‚Ä‚µ‚Ü‚¤
	//‘S‚Ä‚Ì—ÖŠsü‚ÉƒAƒNƒZƒX‚µCMV1SetScale‚ÅŠg‘å‚µ‚½’l‚ÅŠ„‚é‚±‚Æ‚ÅC‘¾‚³‚ð‚à‚Æ‚É–ß‚·
	int MaterialNum = MV1GetMaterialNum(MI->modelHandle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// ƒ}ƒeƒŠƒAƒ‹‚Ì—ÖŠsü‚Ì‘¾‚³‚ðŽæ“¾
		float dotwidth = MV1GetMaterialOutLineDotWidth(MI->modelHandle, i);
		// ƒ}ƒeƒŠƒAƒ‹‚Ì—ÖŠsü‚Ì‘¾‚³‚ðŠg‘å‚µ‚½•ª¬‚³‚­‚·‚é
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
	}

	MV1SetPosition(MI->modelHandle, MI->pos);
	MV1SetRotationXYZ(MI->modelHandle, VScale(MI->dir, (DX_PI_F / 180.0f)));
	MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndex, MI->playTime);
	MV1DrawModel(MI->modelHandle);

	return true;
}