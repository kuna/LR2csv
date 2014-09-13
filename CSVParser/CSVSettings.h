#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#pragma once

/*

this class manages all about CSV Settings that need to be storaged
about custom DSTOPTION, *.ext paths and so on ...

*/

class CSVData;	// prevent each-referencing

namespace CSVPlayOptionConst {
	enum CSVPlayOptionConst {
		GUAGEMODE_1P = 0,
		GUAGEMODE_2P = 1,
		ASSIST_1P = 2,
		ASSIST_2P = 3,
		RANDOM_1P = 4,
		RANDOM_2P = 5,
		EFFECT_1P = 6,
		EFFECT_2P = 7,
		HISPEED_1P = 8,
		HISPEED_2P = 9,
		FLIP = 10,
		SCOREGRAPH = 11,
		HISPEED_FIX = 12,
		GHOST = 13,
		BGA = 14,
		BGASIZE = 15,
		BATTLE = 16,
		JUDGETIMING = 17,
		JUDGETIMING_AUTO = 18,
		TARGET = 19,
		SCREENMODE = 20,
		VSYNC = 21,
		SAVE_REPLAY = 22,
		FAVORITE = 23,
		SHUTTER = 24,
		TARGETRATE = 25,
		PLAYMODE = 26,
	};
};

class CSVPlayOption {
public:
	CSVPlayOption();

	void option_next(int opt);
	void option_prev(int opt);
	const std::wstring& option_getstring(int opt);
	void option_applytext();
private:
	std::vector<std::wstring> settingString[50];	// stores setting string value
	int settingStringIdx[50];						// stores setting string index

	int option[1000];
};

class CSVCustomOption {
public:
	CSVCustomOption();
	std::wstring optName;
	std::wstring optPath;
	std::wstring optValue;
	int optType;		// if 0, then CUSTOMFILE, else, CUSTOMOPTION.
	int optNum;			// for CUSTOMOPTION
	std::vector<std::wstring> optValues;

public:
	void setValue(int val);
	void setValue(const TCHAR *val);
	void getValue(TCHAR *out);
	int getValue();
};

class CSVSettings {
private:
	static std::map<std::wstring, CSVCustomOption> settings;
	static CSVData *csv;

public:
	static CSVPlayOption playOption;

	static bool LoadSettings(CSVData *csvData);
	static bool SaveSettings();
	static void ClearSettings();
	static void SetDefaultValue(CSVData *csvData, bool overwrite=true);
	static void ChangeDSTOpt();
	static CSVCustomOption& GetOption(std::wstring name);
	static void SetOption(std::wstring name, CSVCustomOption &val);
	static bool GetPathValue(const TCHAR* lr2Path, std::wstring &val);
};