/*****************************************************************//**
 * \file   CFile.cpp
 * \brief  ファイル読み書き用関数まとめ
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include "CFile.h"

using namespace std;

vector<string> fileIO::split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) { result.push_back(field); }

	return result;
}

string fileIO::UTF8toSjis(string srcUTF8)
{
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}

bool fileIO::loadCSV(vector<string>* data, const char* dir, bool isComUtoS)
{
	ifstream ifs(dir);

	string line;
	while (getline(ifs, line))
	{
		auto insText = split(line, ',');

		for (int i = 0; i < insText.size(); i++)
		{
			if (isComUtoS) { data->emplace_back(UTF8toSjis(insText.at(i))); }
			else { data->emplace_back(insText.at(i)); }
		}
	}

	ifs.close();
	return true;
}
