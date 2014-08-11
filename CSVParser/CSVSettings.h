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
	static bool LoadSettings(CSVData *csvData);
	static bool SaveSettings();
	static void ClearSettings();
	static void SetDefaultValue(CSVData *csvData, bool overwrite=true);
	static void ChangeDSTOpt();
	static CSVCustomOption& GetOption(std::wstring name);
	static void SetOption(std::wstring name, CSVCustomOption &val);
	static bool GetPathValue(const TCHAR* lr2Path, std::wstring &val);
};