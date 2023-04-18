/*****************************************************************//**
 * \file   charBase.cpp
 * \brief  キャラベースクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"charBase.h"

using namespace model;

VECTOR CB::getDirVecP(float dir, int powar)
{
	float _Dir = dir - 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	VECTOR dirZ = VGet(0, 0, 0);
	dirZ.x = sin(radian) * powar;
	dirZ.z = cos(radian) * powar;
	dirZ.y = 0;
	return dirZ;
}

bool	CB::gravity()
{
	if (_modelInf.pos.y < 1100)
	{
		_modelInf.pos.y = 1090;
		isGround = true;
		if (_modelInf.vec.y < 0) { _modelInf.vec.y = 0; }
	}
	else
	{
		_modelInf.vec.y -= g;
		isGround = false;
	}

	//マップ(円)の中から出ないように
	auto a = _modelInf.pos;
	float c = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	if (c > 4600.f)
	{
		_modelInf.pos = VScale(VNorm(a), 4600.f);
	}
	return true;
}

bool	CB::hitCheck(const char* name, VECTOR* hitPos, VECTOR* hitDir, float* damage)
{
	bool _isHit = false;
	for (int i = 0; i < allColl->size(); i++)
	{
		if (allColl->at(i).attackChar == name || allColl->at(i).nonActiveTimeF > 0) { continue; }

		allColl->at(i).capColl.overPos = VAdd(allColl->at(i).capColl.overPos, allColl->at(i).Vec);
		allColl->at(i).capColl.underPos = VAdd(allColl->at(i).capColl.underPos, allColl->at(i).Vec);

		MATRIX M = MGetIdent();
		if (allColl->at(i).isUseMat) { M = MV1GetFrameLocalWorldMatrix(allColl->at(i).capColl.parentModelHandle, allColl->at(i).capColl.frameNum); }

		auto insUnderPos = VTransform(allColl->at(i).capColl.underPos, M);
		auto insOverPos = VTransform(allColl->at(i).capColl.overPos, M);
		bool insCheckHit = HitCheck_Capsule_Capsule
		(collCap.underPos, collCap.overPos, collCap.r
			, insUnderPos
			, insOverPos
			, allColl->at(i).capColl.r);

		int insNum = VSize(VSub(allColl->at(i).capColl.overPos, allColl->at(i).capColl.underPos));

		for (int j = 0; j < abs(insNum) / allColl->at(i).capColl.r; j++)
		{
			if (allColl->at(i).capCollOld.r == -1) { break; }
			auto insR = allColl->at(i).capColl.r * j;
			auto insS = VSize(allColl->at(i).capColl.overPos);
			auto insN = VNorm(allColl->at(i).capColl.overPos);
			auto insCapNow = VTransform(VScale(insN, insS - insR), M);
			auto insCapOld = VTransform(VScale(insN, insS - insR), allColl->at(i).Mold);

			insCheckHit = HitCheck_Capsule_Capsule
			(collCap.underPos, collCap.overPos, collCap.r
				, insCapNow
				, insCapOld
				, allColl->at(i).capColl.r);
			auto a = DrawCapsule3D(insCapOld, insCapNow, allColl->at(i).capColl.r, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), false);
			if (insCheckHit)
			{
				*hitPos = VScale(VAdd(insCapNow, insCapOld), 0.5f);
				auto nDir = (VSub(insCapOld, insCapNow));
				VECTOR dir = VGet(std::atan2(nDir.y, nDir.z)
					, std::atan2(nDir.x, nDir.z)
					, std::atan2(nDir.x, nDir.y));

				*hitDir = dir;
				break;
			}
		}

		allColl->at(i).capCollOld.underPos = insUnderPos;
		allColl->at(i).capCollOld.overPos = insOverPos;
		allColl->at(i).capCollOld.r = allColl->at(i).capColl.r;
		allColl->at(i).Mold = M;

		if (insCheckHit && allColl->at(i).isAlive)
		{
			allColl->at(i).isAlive = false;
			attackChar = allColl->at(i).attackChar;
			charBox->at(attackChar)->isHit = true;
			_isHit = HPmath(-allColl->at(i).damage, allColl->at(i).stan);
			*damage = allColl->at(i).damage;
			//_isHit = true;
			//charBox->at(attackChar)->drawStopF += 5;
			//if (attackChar == Char_PL) { _valData->hitstopF = 10; }
		}
	}

	return _isHit;
}

bool	CB::makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r
	, int nonActiveTimeF, int activeTimeF, int timeSpeed, bool isUseMat, float damage, float stan, int frameNum, VECTOR _dir, int _efcNum)
{
	attackColl acoll;
	acoll.isUseMat = isUseMat;
	acoll.capColl.parentModelHandle = _modelInf.modelHandle;
	acoll.capColl.frameNum = frameNum;
	acoll.capColl.underPos = _underPos;
	acoll.capColl.overPos = _overPos;
	acoll.capColl.r = r;
	acoll.attackChar = name;
	activeTimeF == 0 || timeSpeed == 0 ? acoll.activeTimeF = activeTimeF : acoll.activeTimeF = activeTimeF / timeSpeed;
	nonActiveTimeF == 0 || timeSpeed == 0 ? acoll.nonActiveTimeF = nonActiveTimeF : acoll.nonActiveTimeF = nonActiveTimeF / timeSpeed;
	acoll.damage = damage;
	acoll.stan = stan;
	acoll.capCollOld.r = -1;
	acoll.Vec = _dir;
	acoll.efcNum = _efcNum;
	allColl->emplace_back(acoll);

	return true;
}