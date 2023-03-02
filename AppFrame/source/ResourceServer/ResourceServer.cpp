#include"ResourceServer.h"
#include"DxLib.h"
#include<stdexcept>

int Rserver::modelImportR(const char* dir)
{
	return MV1LoadModel(dir);
	int insHandle = -1;
	if (modelHandleList.count(dir) > 0)
	{
		auto checkHandle = modelHandleList.at(dir);
		insHandle = MV1DuplicateModel(checkHandle);
	}
	else
	{
		insHandle = MV1LoadModel(dir);
		modelHandleList.emplace(dir, insHandle);
	}

	return MV1DuplicateModel(insHandle);
}
