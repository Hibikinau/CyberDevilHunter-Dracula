#pragma once
#include"charBase.h"
#include "picojson/picojson.h"

namespace {
	constexpr auto BossUtility = "res/ResourcePathData.json";
}

class BossBase : public CharBase 
{
public:
	enum class TYPE
	{
		KNIGHT,
		LION,
		LAST
	};
	TYPE type;

	enum class RANGE
	{
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

	enum class STATUS 
	{
		NONE
	};
	STATUS status;

	bool loadJson(TYPE _type);
	int RangeJ() {
		auto Pvector = VSub(plMI->pos, _modelInf.pos);
		auto playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
		auto playerRange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
		if (playerRange < 300)
		{
			range = RANGE::CrossRange;
		}
		if (300 <= playerRange && playerRange <= 2000)
		{
			range = RANGE::MidRange;
		}
		if (playerRange > 2000)
		{
			range = RANGE::LongRange;
		}
		return static_cast<int>(range);
	};
	STATUS UtilityJudge() {}
	float PlayerDir() {
		auto Pvector = VSub(plMI->pos, _modelInf.pos);
		auto playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
		return playerDir;}
	modelInf* plMI;
};