#pragma once
#include"charBase.h"

class BossBase : public CharBase 
{
	enum class RANGE {
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

	virtual enum class STATUS {
		NONE
	};
	STATUS status;

	bool loadJson(){}
	RANGE RangeJ() { return range; }
	STATUS UtilityJudge() {}
};