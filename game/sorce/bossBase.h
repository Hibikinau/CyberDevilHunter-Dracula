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

	enum class K_STATUS 
	{
		NONE,
		WAIT,
		DAMAGE,
		STAN,
		DEAD,
		RUN,
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,
		SRASH,
		SLAM,
		STAB,
		ROBES,
		JUMPACT,
		ONESLASH,
	};
	

	bool loadJson(TYPE _type);
	float RangeJ() {
		for (auto i = charBox->begin(); i != charBox->end(); i++)
		{
			if (i->second->getType() == 1)
			{
				plMI = i->second->getInf();
			}
		}
		auto Pvector = VSub(plMI->pos, this->_modelInf.pos);
		auto playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
		auto playerRange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
		if (playerRange < 300)
		{
			this->range = RANGE::CrossRange;
		}
		if (300 <= playerRange && playerRange <= 2000)
		{
			this->range = RANGE::MidRange;
		}
		if (playerRange > 2000)
		{
			this->range = RANGE::LongRange;
		}
		return playerDir;
	};
	//STATUS UtilityJudge() {}
	/*float PlayerDir() {
		auto Pvector = VSub(plMI->pos, _modelInf.pos);
		auto playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
		return playerDir;}*/
	modelInf* plMI;
};