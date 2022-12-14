#include <fstream>
#include <iostream>
#include "CFile.h"
#include "../Application/UtilMacro.h"

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
