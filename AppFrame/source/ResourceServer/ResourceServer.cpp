/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  素材を管理するリソースサーバクラスの定義
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"ResourceServer.h"
#include"DxLib.h"
#include<stdexcept>
#include <algorithm>

int Rserver::modelImportR(const char* dir)
{
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

int Rserver::loadGraphR(const char* dir)
{
	for (int i = 0; i < graphHandleList.size(); i++)
	{
		if (graphHandleList[i].dir == dir) { return graphHandleList[i].handle; }
	}

	int insHandle = -1;
	insHandle = LoadGraph(dir);
	if (insHandle != -1)
	{
		graphHandleInf insInf = { dir, insHandle };
		graphHandleList.emplace_back(insInf);
	}
	return insHandle;
}

int Rserver::loadDivGraphR(const char* dir, int allNum, int xNum, int yNum, int xSize, int ySize, int* handle)
{
	for (int i = 0; i < divGraphHandleList.size(); i++)
	{
		if (divGraphHandleList[i].dir == dir)
		{
			for (int j = 0; j < allNum; j++) { handle[j] = divGraphHandleList[i].handle[j]; }
			return 1;
		}
	}
	if (LoadDivGraph(dir, allNum, xNum, yNum, xSize, ySize, handle) == 0)
	{

		divGraphHandleInf insInf;
		insInf.dir = dir;
		for (int i = 0; i < allNum; i++) { insInf.handle.emplace_back(handle[i]); }
		divGraphHandleList.emplace_back(insInf);
	}
	else { return -1; }

	return 0;
}