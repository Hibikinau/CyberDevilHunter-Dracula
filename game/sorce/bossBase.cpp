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
	Pvector = VSub(plMI->pos, this->_modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerRange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
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

/**
 * @brief キャラクターの移動
 * @param speed 移動速度
 * @param radian 移動方向
 * @return true
 */
void BossBase::Move(float speed, float radian) 
{
	float Speed = speed;
	float Radian = (this->_modelInf.dir.y + radian) * DX_PI_F / 180.0f;
	this->_modelInf.pos.x -= sin(Radian) * Speed;
	this->_modelInf.pos.z -= cos(Radian) * Speed;
};
