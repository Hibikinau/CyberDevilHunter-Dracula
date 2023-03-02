#pragma once
#include<map>
#include<vector>
#include<string>

struct modelHandleInf
{
	std::string dir;
	int handle;
};

class Rserver
{
public:
	int modelImportR(const char* dir);

	std::vector<modelHandleInf> modelHandleList;
};
