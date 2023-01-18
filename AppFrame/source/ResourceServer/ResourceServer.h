//#pragma once
//#include	<map>
//#include	<string>
//#include	<vector>
//#include "../modelManager/modelManager.h"
//
//// リソース管理サーバ
//class ResourceServer {
//public:
//	void	Init();
//	void	Release();
//
//	void	clearGraph();
//	bool	loadGraph(const char* dir, int* variablePoint, bool isReserve);
//	bool	loadModel(const char* dir, const float scale, modelInf* MI, bool isReserve);
//	int		loadDivGraph(const char* FileName, const char* name, int AllNum,
//		int XNum, int YNum,
//		int XSize, int YSize, int* HandleBuf);
//	void loadReserve();
//	int		loadSound(const char* FileName, const char* name);
//
//private:
//	modelManager _modelManager;
//	std::map<std::string, int> loadHandle;
//	std::map<std::string, int> loadSoundHandle;
//	std::map<std::string, int*> loadDivHandle;
//	std::map<std::string, modelInf*> modelLoadReserveList;
//	std::map<std::string, modelInf*> modelInfList;
//	std::map<std::string, int*> graphLoadReserveList;
//
//};
//
