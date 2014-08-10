#include "Stdafx.h"
#include "BMSData.h"
#include "BMSUtil.h"

// both of enum are unused now ...
enum BMS_LOCALE {
	BMS_LOCALE_NONE = 0,
	BMS_LOCALE_JP = 1,
	BMS_LOCALE_KR = 2,
};

enum BMSPARSEMODE {
	BMS_PARSER_HEADER = 1,
	BMS_PARSER_MAINDATA = 2,
	BMS_PARSER_BGA = 3,
};

class BMSParser {
private:
	static int LNType;
	static int LNprevVal[120];
	static BMSKeyData* LNKey[120];
	static int BGALayerCount[1322];

	static int BMSParseMode;

	static int BMSKeyCount[14];
	static int randomStackCnt;
	static int randomVal[256];		// Maximum stack: 256
	static int condition[256];		// 0: read line, 1: ignore line, 2: executing command, 3: command already executed

	// for cpp
	static std::wstring getBMSCommand(std::wstring &line, std::wstring spliter);
	static std::wstring getBMSData(std::wstring &line, std::wstring spliter);
	static int checkBMSCommand(std::wstring &line);
public:
	static bool LoadBMSFile(std::wstring path, BMSData bd);
	static bool LoadBMSFile(BYTE *data, BMSData bd);
	static bool ReadBMSFile(const TCHAR *path, BYTE **out);
	static bool ParseBMSData(std::wstring& data, BMSData bd);
	static void ExecutePreProcessor(BMSData bd);
	static void PreProcessBMSLine(std::wstring& line, BMSData bd);
	static void ProcessBMSLine(std::wstring& line, BMSData bd);
	
	// setTimemark has migrated into BMSData...
	// MUST USE AFTER PARSING & SORTING!
};