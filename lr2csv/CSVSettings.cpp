#include "CSVSettings.h"
#include "CSVOption.h"
#include "CSVData.h"


std::map<std::wstring, std::wstring> CSVSettings::settings;
TCHAR CSVSettings::currentCSV[256];


bool CSVSettings::LoadSettings(TCHAR *csvName) {
	// TODO
	return false;
}

bool CSVSettings::SaveSettings() {
	// TODO
	return false;
}

void CSVSettings::SetDefaultValue(CSVData *csvData, bool overwrite) {
	// do insert
	for (int i=0; i< csvData->csvOptions.size(); i++) {
		if (overwrite) {
			settings[csvData->csvOptions[i]->optKey] = csvData->csvOptions[i]->optValue;
		} else {
			settings.insert(std::make_pair(csvData->csvOptions[i]->optKey, csvData->csvOptions[i]->optValue));
		}
	}

	// after inserting, change values of Dst Option
	// TODO: else option should be set to 0 (need to parse CSVOptions)
	std::map<std::wstring, std::wstring>::iterator it = settings.begin();
	for (; it != settings.end(); ++it) {
		int key = _wtoi(it->first.c_str());
		if (key >= 900)
			CSVOption::setOption(key, 1);
	}
}

bool CSVSettings::GetKeyValue(const TCHAR *key, TCHAR *out) {
	if (settings.count(key) == 0) {
		return false;
	} else {
		wcscpy(out, settings.find(key)->second.c_str());
		return true;
	}
}

bool CSVSettings::GetKeyValue(const TCHAR *key, int *out) {
	TCHAR buf[256];
	if (!GetKeyValue(key, buf)) {
		return false;
	} else {
		*out = _wtoi(buf);
		return true;
	}
}

bool CSVSettings::GetKeyValue(int key, int *out) {
	TCHAR bufkey[256];
	_itow(key, bufkey, 10);
	return GetKeyValue(bufkey, out);
}

TCHAR* CSVSettings::GetCurrentCSV() {
	return currentCSV;
}