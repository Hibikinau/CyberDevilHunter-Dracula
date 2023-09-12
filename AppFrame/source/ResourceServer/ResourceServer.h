/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  素材を管理するリソースサーバクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include<map>
#include<vector>
#include<string>

struct modelHandleInf
{
	std::string dir;
	int handle;
};

struct duplicateModelHandleInf
{
	std::string dir;
	int motherModelHandle, *duplicateModelhandle;
};

struct graphHandleInf
{
	std::string dir;
	int handle;
};

struct divGraphHandleInf
{
	std::string dir;
	std::vector<int> handle;
};


class Rserver
{
public:
	int modelImportR(const char* dir, int* handle);
	int loadGraphR(const char* dir);
	int loadDivGraphR(const char* dir, int allNum, int xNum, int yNum, int xSize, int ySize, int* handle);
	int duplicateModelListImportR();

	std::vector<modelHandleInf> modelHandleList;
	std::vector<graphHandleInf> graphHandleList;
	std::vector<divGraphHandleInf> divGraphHandleList;
	std::vector<duplicateModelHandleInf> duplicateModelPointerList;
};