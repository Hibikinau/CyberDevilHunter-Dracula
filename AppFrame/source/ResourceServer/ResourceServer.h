#pragma once
#include	<map>
#include	<string>

// リソース管理サーバ
class ResourceServer {
public:
	void	Init();
	void	Release();

	void	clearGraph();
	int		loadGraph(const char* FileName, const char* name);
	int		loadDivGraph(const char* FileName, const char* name, int AllNum,
		int XNum, int YNum,
		int XSize, int YSize, int* HandleBuf);

	int		loadSound(const char* FileName, const char* name);

private:
	std::map<std::string, int> loadHandle;
	std::map<std::string, int> loadSoundHandle;
	std::map<std::string, int*> loadDivHandle;
};

