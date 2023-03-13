/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  �f�ނ��Ǘ����郊�\�[�X�T�[�o�N���X
 *
 * \author �R�{介v
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
	int modelImportR(const char* dir);
	int loadGraphR(const char* dir);
	int loadDivGraphR(const char* dir, int allNum, int xNum, int yNum, int xSize, int ySize, int* handle);

	std::vector<modelHandleInf> modelHandleList;
	std::vector<graphHandleInf> graphHandleList;
	std::vector<divGraphHandleInf> divGraphHandleList;
};