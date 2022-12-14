/*
** ���\�[�X�T�[�o
*/

#include "../../../DxLib/DxLib.h"
#include "ResourceServer.h"

void ResourceServer::Init()
{
	loadHandle.clear();
	loadDivHandle.clear();
}

void ResourceServer::Release()
{
	clearGraph();
}

void ResourceServer::clearGraph()
{
	// ���ׂẴf�[�^�̍폜������
	loadHandle.clear();
	loadDivHandle.clear();
}


int ResourceServer::loadGraph(const char* FileName, const char* name)
{
	// �L�[�̌���
	if (loadHandle.find(name) != loadHandle.end())
	{
		// �L�[��������
		return loadHandle[name];
	}
	// �L�[����������
	int iHandle = LoadGraph(FileName);
	loadHandle.emplace(name, iHandle);
	return iHandle;
}

int ResourceServer::loadDivGraph(const char* FileName, const char* name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf)
{
	// �L�[�̌���
	if (loadDivHandle.find(name) != loadDivHandle.end())
	{
		// �L�[��������
		HandleBuf = loadDivHandle[name];
		return 0;
	}

	// �L�[����������
	int re = LoadDivGraph(FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf);
	loadDivHandle.emplace(name, HandleBuf);
	return re;
}


int		ResourceServer::loadSound(const char* FileName, const char* name) {
	// �L�[�̌���
	if (loadSoundHandle.find(name) != loadSoundHandle.end())
	{
		// �L�[��������
		return loadSoundHandle[name];
	}

	// �L�[����������
	int iHandle = LoadSoundMem(FileName);
	loadSoundHandle.emplace(name, iHandle);
	return iHandle;
}

