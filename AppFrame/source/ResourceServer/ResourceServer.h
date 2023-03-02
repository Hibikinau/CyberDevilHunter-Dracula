#pragma once
#include<map>
#include<string>

class Rserver
{
public:
	int modelImportR(const char* dir);

	std::map<std::string, int> modelHandleList;
};
