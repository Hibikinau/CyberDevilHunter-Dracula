#include"bossBase.h"

bool BossBase::loadJson(TYPE _type)
{
    // ファイルからjsonデータの読み込み
    std::ifstream ifs(BossUtility);
    picojson::value json;
    ifs >> json;

    const std::string err = picojson::get_last_error();

    if (!err.empty()) 
    {
        return false;
    }

    picojson::array root = json.get<picojson::array>();

	return true;
}



const char* Get(std::string key)
{
   /* if (_resourcePath.find(key) != _resourcePath.end()) {
        return _resourcePath[key].c_str();
    }*/

    return "null";
}

float BossBase::RangeJ(){
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
