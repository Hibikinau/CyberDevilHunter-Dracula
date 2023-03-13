/*****************************************************************//**
 * \file   CFile.h
 * \brief  ファイル読み書き用関数まとめ
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include "../AppFrame/source/Mode/ModeServer.h"
#include "../Application/UtilMacro.h"

namespace fileIO
{
	bool loadCSV(std::vector<std::string>* data, const char* dir, bool isComUtoS);
	std::vector<std::string> split(std::string& input, char delimiter);
	std::string UTF8toSjis(std::string srcUTF8);
}
