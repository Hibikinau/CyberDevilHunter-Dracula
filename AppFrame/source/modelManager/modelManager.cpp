/*****************************************************************//**
 * \file   modelManager.cpp
 * \brief  ���f������N���X�̒�`
 *
 * \author raide
 * \date   March 2023
 *********************************************************************/
#include"modelManager.h"

bool model::modelImport(const char* dir, const float scale, modelInf* MI, Rserver* _Rserver)
{
	MI->modelHandle = _Rserver->modelImportR(dir);

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
		int MaterialNumW = MV1GetMaterialNum(MI->wepons[j].weponHandle);
		for (int k = 0; k < MaterialNumW; k++)
		{
			float dotwidthW = MV1GetMaterialOutLineDotWidth(MI->wepons[j].weponHandle, k);
			MV1SetMaterialOutLineDotWidth(MI->wepons[j].weponHandle, k, dotwidthW / MI->wepons[j].scale);
		}
	}

	return true;
}

bool model::weponAttach(const char* dir, modelInf* MI, int attachFrameNum, const float scale, bool activate, const char* name, Rserver* _Rserver)
{
	weponModelInf weponMI;
	weponMI.isActive = activate;
	weponMI.name = name;
	weponMI.scale = scale;
	weponMI.weponHandle = _Rserver->modelImportR(dir);
	//if (weponMI.weponHandle == -1) { return false; }
	weponMI.weponAttachFrameNum = attachFrameNum;

	MI->wepons.emplace_back(weponMI);
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

	for (auto _weponInf : MI->wepons)
	{
		if (!_weponInf.isActive) { continue; }
		_weponInf.weponFrameMatrix = MV1GetFrameLocalWorldMatrix(MI->modelHandle, _weponInf.weponAttachFrameNum);

		MV1SetMatrix(_weponInf.weponHandle, _weponInf.weponFrameMatrix);

		MV1DrawModel(_weponInf.weponHandle);
	}

	return MI->isAnimEnd;
}

bool model::modelDelete(modelInf* MI)
{//�������f���̃�����������Ă���R���e�i�̏���
	for (int i = 0; i < MI->wepons.size(); i++)
	{
		auto a = MV1DeleteModel(MI->wepons[i].weponHandle);
	}
	MI->wepons.clear();
	auto b = MV1DeleteModel(MI->modelHandle);//�L�������f���̃��������
	return true;
}

bool model::drawBPolygon(VECTOR ldown, VECTOR lup, VECTOR rdown, VECTOR rup, int grHandle)
{
	VERTEX3D Vertex[4];
	WORD Index[6];

	// �S���_���̃f�[�^���Z�b�g
	Vertex[0].pos = rdown;//�E��
	Vertex[0].norm = VGet(0.0f, 0.0f, -1.0f);
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].u = 1.f;
	Vertex[0].v = 1.f;
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;

	Vertex[1].pos = ldown;//����
	Vertex[1].norm = VGet(0.0f, 0.0f, -1.0f);
	Vertex[1].dif = GetColorU8(255, 255, 255, 255);
	Vertex[1].spc = GetColorU8(0, 0, 0, 0);
	Vertex[1].u = 0.f;
	Vertex[1].v = 1.0f;
	Vertex[1].su = 0.0f;
	Vertex[1].sv = 0.0f;

	Vertex[2].pos = rup;//�E��
	Vertex[2].norm = VGet(0.0f, 0.0f, -1.0f);
	Vertex[2].dif = GetColorU8(255, 255, 255, 255);
	Vertex[2].spc = GetColorU8(0, 0, 0, 0);
	Vertex[2].u = 1.f;
	Vertex[2].v = 0.f;
	Vertex[2].su = 0.0f;
	Vertex[2].sv = 0.0f;

	Vertex[3].pos = lup;//����
	Vertex[3].norm = VGet(0.0f, 0.0f, -1.0f);
	Vertex[3].dif = GetColorU8(255, 255, 255, 255);
	Vertex[3].spc = GetColorU8(0, 0, 0, 0);
	Vertex[3].u = 0.0f;
	Vertex[3].v = 0.0f;
	Vertex[3].su = 0.0f;
	Vertex[3].sv = 0.0f;

	// �Q�|���S�����̃C���f�b�N�X�f�[�^���Z�b�g
	Index[0] = 0;//�E��
	Index[1] = 1;//����
	Index[2] = 3;//�E��
	Index[3] = 0;//����
	Index[4] = 3;//�E��
	Index[5] = 2;//����

	auto K = DrawPolygonIndexed3D(Vertex, 4, Index, 2, grHandle, true);
	return K;
}