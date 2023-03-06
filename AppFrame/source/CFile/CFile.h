#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include "../AppFrame/source/ResourceServer/ResourceServer.h"
#include "../Application/UtilMacro.h"

class CFile
{
public:
	// 読み込み
	CFile(const std::string filename);
	// 書き込み
	CFile(const std::string filename, void* data, int size);		// バイナリデータ
	CFile(const std::string filename, const std::string writestr) : CFile(filename, (void*)writestr.c_str(), (int)writestr.size()) {		// 文字列データ
		// 委任コンストラクタで、バイナリデータ書き込みを呼び出す
	}

	~CFile();
	std::string	Filename() { return _filename; }
	void* Data() { return _data; }
	std::string DataStr() { return std::string(_data); }
	int Size() { return _size; }
	bool Success() { return _success; }


private:
	void Init();

	std::string		_filename;

	int		_size;
	char* _data;
	bool	_success;
};

namespace fileIO
{
	bool loadCSV(std::vector<std::string>* data, const char* dir, bool isComUtoS);
	bool writeCSV(valData* data, const char* dir, bool isComUtoS);
	std::vector<std::string> split(std::string& input, char delimiter);
	std::string UTF8toSjis(std::string srcUTF8);
}
