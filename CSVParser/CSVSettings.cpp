#include "Stdafx.h"
#include "CSVSettings.h"
#include "CSVOption.h"
#include "CSVData.h"
#include "CSVFile.h"
#include <boost\filesystem.hpp>


std::map<std::wstring, CSVCustomOption> CSVSettings::settings;
CSVData *CSVSettings::csv;

CSVCustomOption::CSVCustomOption() {
	optType = 0;
}


void CSVSettings::ClearSettings() {
	settings.clear();
}

bool CSVSettings::LoadSettings(CSVData *csvData) {
	// clear
	ClearSettings();
	csv = csvData;

	// first load
	SetDefaultValue(csv);
	
	// make path
	TCHAR fldPath[256];
	CSVFile::GetAbsolutePath(L"LR2Files\\SkinSettings\\", fldPath);
	boost::filesystem::create_directories(fldPath);
	std::wstring fpath = fldPath +
		md5::GetHash((const char*)csv->csvPath.c_str(), csv->csvPath.length()*2) + L".xml";

	// load settings from xml file
	CMarkup xml;
	if (xml.Load(fpath)) {
		xml.IntoElem();
		while (xml.FindElem(L"customfile")) {
			xml.IntoElem();
			std::wstring e_name, e_val;
			xml.FindElem(L"name");
			e_name = xml.GetData();
			xml.FindElem(L"value");
			e_val = xml.GetData();
			settings[e_name].optValue = e_val;
			xml.OutOfElem();
		}
		xml.ResetMainPos();
		while (xml.FindElem(L"customoption")) {
			xml.IntoElem();
			std::wstring e_name;
			int e_val;
			xml.FindElem(L"name");
			e_name = xml.GetData();
			xml.FindElem(L"value");
			e_val = _wtoi(xml.GetData().c_str());
			settings[e_name].optNum = e_val;
			xml.OutOfElem();
		}
	}

	// change DST option
	ChangeDSTOpt();

	return true;
}

bool CSVSettings::SaveSettings() {
	// csv exists?
	if (!csv)
		return false;

	// create folder first
	TCHAR fldPath[256];
	CSVFile::GetAbsolutePath(L"LR2Files\\SkinSettings\\", fldPath);
	boost::filesystem::create_directories(fldPath);

	// and make absolute path (md5 hash)
	std::wstring fpath = fldPath +
		md5::GetHash((const char*)csv->csvPath.c_str(), csv->csvPath.length()*2) + L".xml";

	// save to xml file
	CMarkup xml;
	xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\n");
	xml.AddElem(L"CSVSetting");
	xml.IntoElem();

	for (auto it=settings.begin(); it != settings.end(); ++it) {
		CSVCustomOption *opt = &it->second;
		if (opt->optType == 0) {
			xml.AddElem(L"customfile");
			xml.IntoElem();
			xml.AddElem(L"name", opt->optName);
			xml.AddElem(L"path", opt->optPath);
			xml.AddElem(L"value", opt->optValue);
			xml.OutOfElem();
		} else {
			xml.AddElem(L"customoption");
			xml.IntoElem();
			xml.AddElem(L"name", opt->optName);
			xml.AddElem(L"value", opt->optNum);
			xml.OutOfElem();
		}
	}

	xml.Save(fpath);
	return true;
}

void CSVSettings::SetDefaultValue(CSVData *csvData, bool overwrite) {
	// do insert
	for (int i=0; i< csvData->csvOptions.size(); i++) {
		if (overwrite) {
			settings[csvData->csvOptions[i]->optName] = *csvData->csvOptions[i];
		} else {
			settings.insert(std::make_pair(csvData->csvOptions[i]->optName, *csvData->csvOptions[i]));
		}
	}

	// after inserting, change values of Dst Option
	ChangeDSTOpt();
}

void CSVSettings::ChangeDSTOpt() {
	// only for CUSTOMOPTION
	for (auto it=settings.begin(); it != settings.end(); ++it) {
		int key = it->second.optType;
		if (key >= 900) {
			// disable all opt first
			for (int i=0; i<it->second.optValues.size(); i++)
				CSVOption::setOption(key+i, 0);

			// and set only key
			CSVOption::setOption(key, 1);
		}
	}
}

CSVCustomOption& CSVSettings::GetOption(std::wstring name) {
	return settings[name];
}

void CSVSettings::SetOption(std::wstring name, CSVCustomOption &val) {
	settings[name] = val;
}

/*
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
}*/

bool CSVSettings::GetPathValue(const TCHAR* lr2Path, std::wstring &val) {
	for (auto it=settings.begin(); it!=settings.end(); ++it) {
		if (it->second.optPath == lr2Path) {
			val = it->second.optValue;
			return true;
		}
	}

	return false;
}