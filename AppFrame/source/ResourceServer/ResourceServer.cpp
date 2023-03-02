#include"ResourceServer.h"
#include"DxLib.h"

int Rserver::modelImportR(const char* dir)
{
	auto insIte = modelHandleList.find(dir);
	if (insIte != modelHandleList.end())
	{
		return MV1DuplicateModel(insIte->second);
	}

	int insHandle = MV1LoadModel(dir);
	modelHandleList.emplace(dir, insHandle);

	return MV1DuplicateModel(insHandle);
}
