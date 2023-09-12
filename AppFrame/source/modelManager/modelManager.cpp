/*****************************************************************//**
 * \file   modelManager.cpp
 * \brief  モデル制御クラスの定義
 *
 * \author raide
 * \date   March 2023
 *********************************************************************/
#include"modelManager.h"

bool model::modelImport(const char* dir, const float scale, modelInf* MI, Rserver* _Rserver)
{
	_Rserver->modelImportR(dir, &MI->modelHandle);

	//if (MI->modelHandle == -1) { return false; }
	MI->playTime = 0.0f;
	MI->scale = scale;

	return true;
}

bool model::changeScale(modelInf* MI)
{
	MV1SetScale(MI->modelHandle, VGet(MI->scale, MI->scale, MI->scale));

	int MaterialNum = MV1GetMaterialNum(MI->modelHandle);
	for (int i = 0; i < MaterialNum; i++)
	{
		float dotwidth = MV1GetMaterialOutLineDotWidth(MI->modelHandle, i);
		MV1SetMaterialOutLineDotWidth(MI->modelHandle, i, dotwidth / MI->scale);
	}
	for (int j = 0; j < MI->wepons.size(); j++)
	{
		int MaterialNumW = MV1GetMaterialNum(MI->wepons[j]->weponHandle);
		for (int k = 0; k < MaterialNumW; k++)
		{
			float dotwidthW = MV1GetMaterialOutLineDotWidth(MI->wepons[j]->weponHandle, k);
			MV1SetMaterialOutLineDotWidth(MI->wepons[j]->weponHandle, k, dotwidthW / MI->wepons[j]->scale);
		}
	}

	return true;
}

bool model::weponAttach(const char* dir, modelInf* MI, int attachFrameNum, const float scale, bool activate, const char* name, Rserver* _Rserver)
{
	std::shared_ptr<weponModelInf> weponMI(new weponModelInf);
	weponMI->isActive = activate;
	weponMI->name = name;
	weponMI->scale = scale;
	_Rserver->modelImportR(dir, &weponMI->weponHandle);
	//if (weponMI.weponHandle == -1) { return false; }
	weponMI->weponAttachFrameNum = attachFrameNum;

	MI->wepons.emplace_back(std::move(weponMI));
	return true;
}

bool model::animChange(int _animHandle, modelInf* MI, bool isLoop, bool isBlend, bool isOverride)
{
	if (MI->animHandleOld == _animHandle && !isOverride) { return false; }
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

bool model::setNextAnim(int _animHandle, modelInf* MI, bool isLoop, bool isBlend)
{
	MI->isBrendingNext = isBlend;
	MI->animHandleNext = _animHandle;
	MI->animLoopNext = isLoop;

	return true;
}

bool model::modelRender(modelInf* MI, float animSpeed, float timeSpead)
{
	if (timeSpead != 0) { MI->isAnimEnd = false; }
	if (MI->isBrending) { MI->rate = 0.f, MI->isBrending = false; }
	if (MI->rate <= 1.0f)
	{
		MI->rate >= 1.0f ? MI->rate = 1.0f : MI->rate += 0.1f * timeSpead;

		MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndexOld, MI->playTimeOld);
	}
	else
	{
		MI->playTime += (animSpeed + MI->animSpdBuff) * timeSpead;
		if (MI->playTime >= MI->totalTime)
		{
			MI->isAnimEnd = true;
			if (MI->animHandleNext != -1)
			{
				animChange(MI->animHandleNext, MI, MI->animLoopNext, MI->isBrendingNext, true);
				MI->animHandleNext = -1;
			}
			else
			{
				if (MI->animOldLoop) { MI->playTime = 0.f; }
				else { MI->playTime = MI->totalTime; }
			}
		}
	}

	MV1SetAttachAnimBlendRate(MI->modelHandle, MI->attachIndexOld, 1.0f - MI->rate);
	MV1SetAttachAnimBlendRate(MI->modelHandle, MI->attachIndex, MI->rate);

	MV1SetAttachAnimTime(MI->modelHandle, MI->attachIndex, MI->playTime);

	MV1SetPosition(MI->modelHandle, VAdd(MI->pos, MI->addPos));
	MV1SetRotationXYZ(MI->modelHandle, VScale(MI->dir, (DX_PI_F / 180.0f)));
	MV1DrawModel(MI->modelHandle);

	for (int i = 0; i < MI->wepons.size(); i++)
	{
		if (!MI->wepons[i]->isActive) { continue; }
		MI->wepons[i]->weponFrameMatrix = MV1GetFrameLocalWorldMatrix(MI->modelHandle, MI->wepons[i]->weponAttachFrameNum);

		MV1SetMatrix(MI->wepons[i]->weponHandle, MI->wepons[i]->weponFrameMatrix);

		MV1DrawModel(MI->wepons[i]->weponHandle);
	}

	return MI->isAnimEnd;
}

bool model::modelDelete(modelInf* MI)
{//装備モデルのメモリ解放してからコンテナの消去
	for (int i = 0; i < MI->wepons.size(); i++)
	{
		auto a = MV1DeleteModel(MI->wepons[i]->weponHandle);
	}
	MI->wepons.clear();
	auto b = MV1DeleteModel(MI->modelHandle);//キャラモデルのメモリ解放
	return true;
}

bool model::drawBPolygon(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, int grHandle)
{
	VERTEX3D Vertex[4];
	WORD Index[6];
	DxLib::COLOR_U8 _difColor = GetColorU8(255, 255, 255, 255), _spcColor = GetColorU8(0, 0, 0, 0);

	// ４頂点分のデータをセット
	Vertex[0].pos = rdown;//右下
	Vertex[0].u = 1.f;
	Vertex[0].v = 1.f;
	Vertex[1].pos = ldown;//左下
	Vertex[1].u = 0.f;
	Vertex[1].v = 1.0f;
	Vertex[2].pos = rup;//右上
	Vertex[2].u = 1.f;
	Vertex[2].v = 0.f;
	Vertex[3].pos = lup;//左上
	Vertex[3].u = 0.0f;
	Vertex[3].v = 0.0f;
	Vertex[0].norm = Vertex[1].norm = Vertex[2].norm = Vertex[3].norm = VGet(0.0f, 0.0f, -1.0f);
	Vertex[0].dif = Vertex[1].dif = Vertex[2].dif = Vertex[3].dif = _difColor;
	Vertex[0].spc = Vertex[1].spc = Vertex[2].spc = Vertex[3].spc = _spcColor;
	Vertex[0].su = Vertex[1].su = Vertex[2].su = Vertex[3].su = 0.0f;
	Vertex[0].sv = Vertex[1].sv = Vertex[2].sv = Vertex[3].sv = 0.0f;

	// ２ポリゴン分のインデックスデータをセット
	Index[0] = 0;//右下
	Index[1] = 1;//左下
	Index[2] = 3;//右上
	Index[3] = 0;//左上
	Index[4] = 3;//右上
	Index[5] = 2;//左下

	auto K = DrawPolygonIndexed3D(Vertex, 4, Index, 2, grHandle, true);
	return K;
}

bool model::drawBord(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, DxLib::COLOR_U8 difColor, DxLib::COLOR_U8 spcColor)
{
	VERTEX3D Vertex[4];
	WORD Index[6];

	// ４頂点分のデータをセット
	Vertex[0].pos = rdown;//右下
	Vertex[0].u = 1.f;
	Vertex[0].v = 1.f;
	Vertex[1].pos = ldown;//左下
	Vertex[1].u = 0.f;
	Vertex[1].v = 1.0f;
	Vertex[2].pos = rup;//右上
	Vertex[2].u = 1.f;
	Vertex[2].v = 0.f;
	Vertex[3].pos = lup;//左上
	Vertex[3].u = 0.0f;
	Vertex[3].v = 0.0f;
	Vertex[0].dif = Vertex[1].dif = Vertex[2].dif = Vertex[3].dif = difColor;
	Vertex[0].spc = Vertex[1].spc = Vertex[2].spc = Vertex[3].spc = spcColor;
	Vertex[0].su = Vertex[1].su = Vertex[2].su = Vertex[3].su = 0.0f;
	Vertex[0].sv = Vertex[1].sv = Vertex[2].sv = Vertex[3].sv = 0.0f;
	Vertex[0].norm = Vertex[1].norm = Vertex[2].norm = Vertex[3].norm = VGet(0.0f, 0.0f, -1.0f);

	// ２ポリゴン分のインデックスデータをセット
	Index[0] = 0;//右下
	Index[1] = 1;//左下
	Index[2] = 3;//右上
	Index[3] = 0;//左上
	Index[4] = 3;//右上
	Index[5] = 2;//左下

	auto K = DrawPolygonIndexed3D(Vertex, 4, Index, 2, DX_NONE_GRAPH, true);
	return K;
}

bool model::drawCube(VECTOR pos, VECTOR siz, DxLib::COLOR_U8 difColor, DxLib::COLOR_U8 spcColor)
{
	siz = VScale(siz, 0.5);
	drawBord(VGet(pos.x - siz.x, pos.y - siz.y, pos.z - siz.z), VGet(pos.x - siz.x, pos.y + siz.y, pos.z - siz.z), VGet(pos.x - siz.x, pos.y - siz.y, pos.z + siz.z), VGet(pos.x - siz.x, pos.y + siz.y, pos.z + siz.z), GetColorU8(0, 0, 255, 255), GetColorU8(0, 0, 255, 255));
	drawBord(VGet(pos.x + siz.x, pos.y - siz.y, pos.z - siz.z), VGet(pos.x + siz.x, pos.y + siz.y, pos.z - siz.z), VGet(pos.x + siz.x, pos.y - siz.y, pos.z + siz.z), VGet(pos.x + siz.x, pos.y + siz.y, pos.z + siz.z), GetColorU8(0, 0, 255, 255), GetColorU8(0, 0, 255, 255));
	drawBord(VGet(pos.x - siz.x, pos.y - siz.y, pos.z - siz.z), VGet(pos.x + siz.x, pos.y - siz.y, pos.z - siz.z), VGet(pos.x - siz.x, pos.y - siz.y, pos.z + siz.z), VGet(pos.x + siz.x, pos.y - siz.y, pos.z + siz.z), GetColorU8(0, 0, 255, 255), GetColorU8(0, 0, 255, 255));
	drawBord(VGet(pos.x - siz.x, pos.y + siz.y, pos.z - siz.z), VGet(pos.x + siz.x, pos.y + siz.y, pos.z - siz.z), VGet(pos.x - siz.x, pos.y + siz.y, pos.z + siz.z), VGet(pos.x + siz.x, pos.y + siz.y, pos.z + siz.z), GetColorU8(0, 0, 255, 255), GetColorU8(0, 0, 255, 255));
	drawBord(VGet(pos.x - siz.x, pos.y - siz.y, pos.z - siz.z), VGet(pos.x + siz.x, pos.y - siz.y, pos.z - siz.z), VGet(pos.x - siz.x, pos.y + siz.y, pos.z - siz.z), VGet(pos.x + siz.x, pos.y + siz.y, pos.z - siz.z), GetColorU8(0, 0, 255, 255), GetColorU8(0, 0, 255, 255));
	drawBord(VGet(pos.x - siz.x, pos.y - siz.y, pos.z + siz.z), VGet(pos.x + siz.x, pos.y - siz.y, pos.z + siz.z), VGet(pos.x - siz.x, pos.y + siz.y, pos.z + siz.z), VGet(pos.x + siz.x, pos.y + siz.y, pos.z + siz.z), GetColorU8(0, 0, 255, 255), GetColorU8(0, 0, 255, 255));

	return true;
}
