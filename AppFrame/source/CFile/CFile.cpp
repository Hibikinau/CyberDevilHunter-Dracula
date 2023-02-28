#include "CFile.h"

using namespace std;

vector<string> CFile::split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

string CFile::UTF8toSjis(string srcUTF8) {
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

bool CFile::loadCSV(vector<string> *data, const char *name, bool isComUtoS)
{
	ifstream ifs(name);

	string line;
	while (getline(ifs, line)) {
		auto insText = split(line, ',');

		for (int i = 0; i < insText.size(); i++) {
			if (isComUtoS) { data->emplace_back(UTF8toSjis(insText.at(i))); }
			else{ data->emplace_back(insText.at(i)); }
		}
	}

	return true;
}


// �ǂݍ���
CFile::CFile(const std::string filename) {
	// �����o�̏�����
	Init();
	_filename = filename;

	// �t�@�C�����J��
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);	// ���͂��o�C�i���`����
	if (!ifs) {
		// �J���̎��s
		return;
	}

	// �t�@�C���T�C�Y�̎擾
	ifs.seekg(0, std::ios_base::end);
	_size = (int)ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	// ���������m��
	// ������Ƃ��ė��p�ł���悤�ɁA+1byte�]�v�Ɋm��
	_data = new char[_size + 1];

	// �t�@�C�������[�h
	ifs.read(_data, _size);

	// �t�@�C�������
	ifs.close();

	// ������Ƃ��ė��p�ł���悤�ɁA�f�[�^�̖��[��\0��t����
	_data[_size] = '\0';

	// ��������
	_success = true;
}

// ��������
CFile::CFile(const std::string filename, void* data, int size) {
	// �����o�̏�����
	Init();
	_filename = filename;

	// �t�@�C�����J��
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);	// �o�͂��o�C�i���`����
	if (!ofs) {
		// �J���̎��s
		return;
	}

	// �f�[�^�̏�������
	ofs.write((char*)data, size);

	// �t�@�C�������
	ofs.close();

	// ��������
	_success = true;
}

// ��������


CFile::~CFile() {
	SAFE_DELETE(_data);
}

void CFile::Init() {
	_size = 0;
	_data = NULL;
	_filename = "";
	_success = false;
}
