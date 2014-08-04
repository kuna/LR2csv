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
	TCHAR optName[256];
	TCHAR optKey[256];
	TCHAR optValue[256];
	std::vector<std::wstring> optValues;

public:
	void setValue(int val);
	void setValue(const TCHAR *val);
	void getValue(TCHAR *out);
	int getValue();
};

class CSVSettings {
private:
	static std::map<std::wstring, std::wstring> settings;
	static TCHAR currentCSV[256];

public:
	static bool LoadSettings(TCHAR *csvName);
	static bool SaveSettings();
	static void SetDefaultValue(CSVData *csvData, bool overwrite=true);

	static bool GetKeyValue(const TCHAR *key, TCHAR *out);
	static bool GetKeyValue(const TCHAR *key, int *out);
	static bool GetKeyValue(int key, int *out);

	static TCHAR* GetCurrentCSV();
};