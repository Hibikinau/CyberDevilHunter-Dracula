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


