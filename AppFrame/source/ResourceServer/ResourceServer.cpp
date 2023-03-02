#include"ResourceServer.h"
#include"DxLib.h"
#include<stdexcept>
#include <algorithm>

int Rserver::modelImportR(const char* dir)
{
	//return MV1LoadModel(dir);
	OutputDebugString("a");
	for (int i = 0; i < modelHandleList.size(); i++)
	{
		if (modelHandleList[i].dir == dir) { return MV1DuplicateModel(modelHandleList[i].handle); }
	}

	int insHandle = -1;
	insHandle = MV1LoadModel(dir);
	modelHandleInf insInf = { dir, insHandle };
	modelHandleList.emplace_back(insInf);
	return MV1DuplicateModel(insHandle);
}
