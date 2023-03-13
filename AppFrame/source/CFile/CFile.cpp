/*****************************************************************//**
 * \file   CFile.cpp
 * \brief  �t�@�C���ǂݏ����p�֐��܂Ƃ�
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#include "CFile.h"

using namespace std;

vector<string> fileIO::split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

string fileIO::UTF8toSjis(string srcUTF8) {
	//Unicode�֕ϊ���̕����񒷂𓾂�
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8����Unicode�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJIS�֕ϊ���̕����񒷂𓾂�
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
	char* bufShiftJis = new char[lengthSJis];

	//Unicode����ShiftJIS�֕ϊ�
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
	while (getline(ifs, line)) {
		auto insText = split(line, ',');

		for (int i = 0; i < insText.size(); i++) {
			if (isComUtoS) { data->emplace_back(UTF8toSjis(insText.at(i))); }
			else { data->emplace_back(insText.at(i)); }
		}
	}

	ifs.close();
	return true;
}
