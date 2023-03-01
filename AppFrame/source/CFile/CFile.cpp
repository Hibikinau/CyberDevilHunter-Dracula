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


// 読み込み
CFile::CFile(const std::string filename) {
	// メンバの初期化
	Init();
	_filename = filename;

	// ファイルを開く
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);	// 入力をバイナリ形式で
	if (!ifs) {
		// 開くの失敗
		return;
	}

	// ファイルサイズの取得
	ifs.seekg(0, std::ios_base::end);
	_size = (int)ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	// メモリを確保
	// 文字列として利用できるように、+1byte余計に確保
	_data = new char[_size + 1];

	// ファイルをロード
	ifs.read(_data, _size);

	// ファイルを閉じる
	ifs.close();

	// 文字列として利用できるように、データの末端に\0を付ける
	_data[_size] = '\0';

	// 処理成功
	_success = true;
}

// 書き込み
CFile::CFile(const std::string filename, void* data, int size) {
	// メンバの初期化
	Init();
	_filename = filename;

	// ファイルを開く
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);	// 出力をバイナリ形式で
	if (!ofs) {
		// 開くの失敗
		return;
	}

	// データの書き込み
	ofs.write((char*)data, size);

	// ファイルを閉じる
	ofs.close();

	// 処理成功
	_success = true;
}

// 書き込み


CFile::~CFile() {
	SAFE_DELETE(_data);
}

void CFile::Init() {
	_size = 0;
	_data = NULL;
	_filename = "";
	_success = false;
}
