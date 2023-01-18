///*
//** リソースサーバ
//*/
//
//#include "../../../DxLib/DxLib.h"
//#include "ResourceServer.h"
//
//void ResourceServer::Init()
//{
//	loadHandle.clear();
//	loadDivHandle.clear();
//}
//
//void ResourceServer::Release()
//{
//	clearGraph();
//}
//
//void ResourceServer::clearGraph()
//{
//	// すべてのデータの削除をする
//	loadHandle.clear();
//	loadDivHandle.clear();
//}
//
//void ResourceServer::loadReserve()
//{
//	return;
//	/*for (auto ite = graphLoadReserveList.begin(); ite != graphLoadReserveList.end(); )
//	{
//		*ite->second = LoadGraph(ite->first.c_str());
//		ite = graphLoadReserveList.erase(ite);
//	}
//
//	for (auto ite = modelInfList.begin(); ite != modelInfList.end(); )
//	{
//		_modelManager.modelImport();
//		ite = graphLoadReserveList.erase(ite);
//	}*/
//
//	return;
//}
//
//bool ResourceServer::loadGraph(const char* dir, int* variablePoint, bool isReserve)
//{
//	// キーの検索
//	if (loadHandle.find(dir) != loadHandle.end())
//	{
//		// キーがあった
//		*variablePoint = loadHandle[dir];
//		return true;
//	}
//	// キーが無かった
//	if (isReserve)
//	{
//		graphLoadReserveList.emplace(dir, variablePoint);
//		return false;
//	}
//	else
//	{
//		int iHandle = LoadGraph(dir);
//		*variablePoint = iHandle;
//		loadHandle.emplace(dir, iHandle);
//		return true;
//	}
//}
//
//bool ResourceServer::loadModel(const char* dir, const float scale, modelInf* MI, bool isReserve)
//{
//	// キーの検索
//	if (modelInfList.find(dir) != modelInfList.end())
//	{
//		// キーがあった
//		MI = modelInfList[dir];
//		return true;
//	}
//	// キーが無かった
//	if (isReserve)
//	{
//		graphLoadReserveList.emplace(dir, MI);
//		return false;
//	}
//	else
//	{
//		_modelManager.modelImport(dir, scale, MI);
//		modelInfList.emplace(dir, MI);
//		return true;
//	}
//}
//
//int ResourceServer::loadDivGraph(const char* FileName, const char* name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf)
//{
//	// キーの検索
//	if (loadDivHandle.find(name) != loadDivHandle.end())
//	{
//		// キーがあった
//		HandleBuf = loadDivHandle[name];
//		return 0;
//	}
//
//	// キーが無かった
//	int re = LoadDivGraph(FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf);
//	loadDivHandle.emplace(name, HandleBuf);
//	return re;
//}
//
//
//int		ResourceServer::loadSound(const char* FileName, const char* name) {
//	// キーの検索
//	if (loadSoundHandle.find(name) != loadSoundHandle.end())
//	{
//		// キーがあった
//		return loadSoundHandle[name];
//	}
//
//	// キーが無かった
//	int iHandle = LoadSoundMem(FileName);
//	loadSoundHandle.emplace(name, iHandle);
//	return iHandle;
//}
//
