#pragma once

#include <string>

class CFile
{
public:
	// �ǂݍ���
	CFile(const std::string filename);
	// ��������
	CFile(const std::string filename, void* data, int size);		// �o�C�i���f�[�^
	CFile(const std::string filename, const std::string writestr) : CFile(filename, (void*)writestr.c_str(), (int)writestr.size()) {		// ������f�[�^
		// �ϔC�R���X�g���N�^�ŁA�o�C�i���f�[�^�������݂��Ăяo��
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
