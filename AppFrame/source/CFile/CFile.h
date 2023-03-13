/*****************************************************************//**
 * \file   CFile.h
 * \brief  �t�@�C���ǂݏ����p�֐��܂Ƃ�
 *
 * \author �R�{介v
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
