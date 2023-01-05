#include"modelManager.h"

bool modelManager::modelImport(const char* dir, const float scale, modelInf* MI)
{
	MI->modelHandle = MV1LoadModel(dir);
	if (MI->modelHandle == -1) { return false; }
	MI->playTime = 0.0f;

	MV1SetPosition(MI->modelHandle, MI->pos);
	MV1SetRotationXYZ(MI->modelHandle, MI->dir);

	MV1SetScale(MI->modelHandle, VGet(scale, scale, scale));
	//—ÖŠsü‚Ì‘å‚«‚³‚ğC³‚·‚é
	//’ÊíMV1SetScale‚ÅŠg‘å‚·‚é‚ÆC—ÖŠsü‚Ì‘å‚«‚³‚àŠg‘å‚³‚ê‚Ä‚µ‚Ü‚¤
	//‘S‚Ä‚Ì—ÖŠsü‚ÉƒAƒNƒZƒX‚µCMV1SetScale‚ÅŠg‘å‚µ‚½’l‚ÅŠ„‚é‚±‚Æ‚ÅC‘¾‚³‚ğ‚à‚Æ‚É–ß‚·
	int MaterialNum = MV1GetMaterialNum(MI->modelHandle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// ƒ}ƒeƒŠƒAƒ‹‚Ì—ÖŠsü‚Ì‘¾‚³‚ğæ“¾
		float dotwidth = MV1GetMaterialOutLineDotWidth(MI->modelHandle, i);
		// ƒ}ƒeƒŠƒAƒ‹‚Ì—ÖŠsü‚Ì‘¾‚³‚ğŠg‘å‚µ‚½•ª¬‚³‚­‚·‚é
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
	//—ÖŠsü‚Ì‘å‚«‚³‚ğC³‚·‚é
	int MaterialNum = MV1GetMaterialNum(weponMI.weponHandle);
	for (int i = 0; i < MaterialNum; i++)
	{
		// ƒ}ƒeƒŠƒAƒ‹‚Ì—ÖŠsü‚Ì‘¾‚³‚ğæ“¾
		float dotwidth = MV1GetMaterialOutLineDotWidth(weponMI.weponHandle, i);
		// ƒ}ƒeƒŠƒAƒ‹‚Ì—ÖŠsü‚Ì‘¾‚³‚ğŠg‘å‚µ‚½•ª¬‚³‚­‚·‚é
		MV1SetMaterialOutLineDotWidth(weponMI.weponHandle, i, dotwidth / scale);
	}

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

bool modelManager::modelRender(modelInf* MI, float animSpeed)
{
	MI->isAnimEnd = false;
	if (MI->isBrending) { MI->rate = 0.f, MI->isBrending = false; }
	if (MI->rate <= 1.0f)
	{
		MI->rate >= 1.0f ? MI->rate = 1.0f : MI->rate += 0.1f;

		MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndexOld, MI->playTimeOld);
	}
	else
	{
		MI->playTime += animSpeed;
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
{
	for (int i = 0; i < MI->wepons.size(); i++)
	{
		DeleteGraph(MI->wepons[i].weponHandle);
	}
	MI->wepons.clear();
	DeleteGraph(MI->modelHandle);
	return true;
}
