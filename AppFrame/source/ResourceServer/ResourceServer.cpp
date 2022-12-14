/*
** リソースサーバ
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
	// すべてのデータの削除をする
	loadHandle.clear();
	loadDivHandle.clear();
}


int ResourceServer::loadGraph(const char* FileName, const char* name)
{
	// キーの検索
	if (loadHandle.find(name) != loadHandle.end())
	{
		// キーがあった
		return loadHandle[name];
	}
	// キーが無かった
	int iHandle = LoadGraph(FileName);
	loadHandle.emplace(name, iHandle);
	return iHandle;
}

int ResourceServer::loadDivGraph(const char* FileName, const char* name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf)
{
	// キーの検索
	if (loadDivHandle.find(name) != loadDivHandle.end())
	{
		// キーがあった
		HandleBuf = loadDivHandle[name];
		return 0;
	}

	// キーが無かった
	int re = LoadDivGraph(FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf);
	loadDivHandle.emplace(name, HandleBuf);
	return re;
}


int		ResourceServer::loadSound(const char* FileName, const char* name) {
	// キーの検索
	if (loadSoundHandle.find(name) != loadSoundHandle.end())
	{
		// キーがあった
		return loadSoundHandle[name];
	}

	// キーが無かった
	int iHandle = LoadSoundMem(FileName);
	loadSoundHandle.emplace(name, iHandle);
	return iHandle;
}

