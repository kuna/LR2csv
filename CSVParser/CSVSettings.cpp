#include "Stdafx.h"
#include "CSVSettings.h"
#include "CSVOption.h"
#include "CSVData.h"
#include "CSVFile.h"
#include "CSVConst.h"
#include "CSVText.h"
#include "CSVNumber.h"
#include <boost\filesystem.hpp>
#include <boost\format.hpp>


std::map<std::wstring, CSVCustomOption> CSVSettings::settings;
CSVData *CSVSettings::csv;
CSVPlayOption CSVSettings::playOption;

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



/*
 **********************************************
 * Playoption class
 **********************************************
 */

CSVPlayOption::CSVPlayOption() {
	// initalize setting string
	settingString[CSVPlayOptionConst::ASSIST_1P].push_back(L"OFF");
	settingString[CSVPlayOptionConst::ASSIST_1P].push_back(L"ON");
	settingString[CSVPlayOptionConst::ASSIST_2P].push_back(L"OFF");
	settingString[CSVPlayOptionConst::ASSIST_2P].push_back(L"ON");
	settingString[CSVPlayOptionConst::EFFECT_1P].push_back(L"OFF");
	settingString[CSVPlayOptionConst::EFFECT_1P].push_back(L"HIDDEN");
	settingString[CSVPlayOptionConst::EFFECT_1P].push_back(L"SUDDEN");
	settingString[CSVPlayOptionConst::EFFECT_1P].push_back(L"HID + SUD");
	settingString[CSVPlayOptionConst::EFFECT_2P].push_back(L"OFF");
	settingString[CSVPlayOptionConst::EFFECT_2P].push_back(L"HIDDEN");
	settingString[CSVPlayOptionConst::EFFECT_2P].push_back(L"SUDDEN");
	settingString[CSVPlayOptionConst::EFFECT_2P].push_back(L"HID + SUD");
	settingString[CSVPlayOptionConst::FAVORITE].push_back(L"OFF");
	settingString[CSVPlayOptionConst::FAVORITE].push_back(L"ON");
	settingString[CSVPlayOptionConst::FLIP].push_back(L"OFF");
	settingString[CSVPlayOptionConst::FLIP].push_back(L"FLIP");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"GROOVE");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"SURVIVAL");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"EX-HARD");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"DEATH");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"EASY");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"P-ATTACK");
	settingString[CSVPlayOptionConst::GUAGEMODE_1P].push_back(L"G-ATTACK");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"GROOVE");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"SURVIVAL");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"EX-HARD");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"DEATH");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"EASY");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"P-ATTACK");
	settingString[CSVPlayOptionConst::GUAGEMODE_2P].push_back(L"G-ATTACK");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"OFF");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"MIRROR");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"S-RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"SCATTER");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"H-RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_1P].push_back(L"R-RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"OFF");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"MIRROR");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"S-RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"SCATTER");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"H-RANDOM");
	settingString[CSVPlayOptionConst::RANDOM_2P].push_back(L"R-RANDOM");
	settingString[CSVPlayOptionConst::BATTLE].push_back(L"OFF");
	settingString[CSVPlayOptionConst::BATTLE].push_back(L"BATTLE (RIVAL)");
	settingString[CSVPlayOptionConst::BATTLE].push_back(L"G-BATTLE");
	settingString[CSVPlayOptionConst::BATTLE].push_back(L"D-BATTLE");
	settingString[CSVPlayOptionConst::BATTLE].push_back(L"ONLINE BATTLE");
	settingString[CSVPlayOptionConst::HISPEED_FIX].push_back(L"OFF");
	settingString[CSVPlayOptionConst::HISPEED_FIX].push_back(L"MAXBPM");
	settingString[CSVPlayOptionConst::HISPEED_FIX].push_back(L"MINBPM");
	settingString[CSVPlayOptionConst::HISPEED_FIX].push_back(L"AVERAGE");
	settingString[CSVPlayOptionConst::HISPEED_FIX].push_back(L"CONSTANT");
	settingString[CSVPlayOptionConst::GHOST].push_back(L"OFF");
	settingString[CSVPlayOptionConst::GHOST].push_back(L"TYPE A");
	settingString[CSVPlayOptionConst::GHOST].push_back(L"TYPE B");
	settingString[CSVPlayOptionConst::GHOST].push_back(L"TYPE C");
	settingString[CSVPlayOptionConst::SCOREGRAPH].push_back(L"OFF");
	settingString[CSVPlayOptionConst::SCOREGRAPH].push_back(L"ON");
	settingString[CSVPlayOptionConst::BGA].push_back(L"OFF");
	settingString[CSVPlayOptionConst::BGA].push_back(L"ON");
	settingString[CSVPlayOptionConst::BGA].push_back(L"AUTOPLAY");
	settingString[CSVPlayOptionConst::BGASIZE].push_back(L"NORMAL");
	settingString[CSVPlayOptionConst::BGASIZE].push_back(L"EXTEND");
	settingString[CSVPlayOptionConst::JUDGETIMING_AUTO].push_back(L"ON");
	settingString[CSVPlayOptionConst::JUDGETIMING_AUTO].push_back(L"OFF");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"ALL KEYS");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"SINGLE");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"7KEYS");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"5KEYS");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"DOUBLE");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"14KEYS");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"10KEYS");
	settingString[CSVPlayOptionConst::PLAYMODE].push_back(L"9KEYS");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"NO TARGET");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"MY BEST");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"RANK AAA");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"RANK AA");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"RANK A");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"100%");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"IR TOP");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"IR NEXT");
	settingString[CSVPlayOptionConst::TARGET].push_back(L"IR AVERAGE");


	// initalize index
	settingStringIdx[CSVPlayOptionConst::ASSIST_1P] = CSVTextConst::OPTION_1P_ASSIST;
	settingStringIdx[CSVPlayOptionConst::ASSIST_2P] = CSVTextConst::OPTION_2P_ASSIST;
	settingStringIdx[CSVPlayOptionConst::BATTLE] = CSVTextConst::OPTION_BATTLE;
	settingStringIdx[CSVPlayOptionConst::EFFECT_1P] = CSVTextConst::OPTION_1P_EFFECT;
	settingStringIdx[CSVPlayOptionConst::EFFECT_2P] = CSVTextConst::OPTION_2P_EFFECT;
	//settingStringIdx[CSVPlayOptionConst::FAVORITE] = CSVTextConst::OPTION_;
	settingStringIdx[CSVPlayOptionConst::FLIP] = CSVTextConst::OPTION_FLIP;
	settingStringIdx[CSVPlayOptionConst::GUAGEMODE_1P] = CSVTextConst::OPTION_1P_GUAGE;
	settingStringIdx[CSVPlayOptionConst::GUAGEMODE_2P] = CSVTextConst::OPTION_2P_GUAGE;
	settingStringIdx[CSVPlayOptionConst::RANDOM_1P] = CSVTextConst::OPTION_1P_RANDOM;
	settingStringIdx[CSVPlayOptionConst::RANDOM_2P] = CSVTextConst::OPTION_2P_RANDOM;
	settingStringIdx[CSVPlayOptionConst::BATTLE] = CSVTextConst::OPTION_BATTLE;
	//settingStringIdx[CSVPlayOptionConst::HISPEED_1P] = CSVTextConst::OPTION_S;
	settingStringIdx[CSVPlayOptionConst::HISPEED_FIX] = CSVTextConst::OPTION_SCROLLTYPE;
	settingStringIdx[CSVPlayOptionConst::GHOST] = CSVTextConst::OPTION_GHOST;
	settingStringIdx[CSVPlayOptionConst::SCOREGRAPH] = CSVTextConst::OPTION_SCOREGRAPH;
	settingStringIdx[CSVPlayOptionConst::BGA] = CSVTextConst::OPTION_BGAONOFF;
	settingStringIdx[CSVPlayOptionConst::BGASIZE] = CSVTextConst::OPTION_BGASIZE;
	settingStringIdx[CSVPlayOptionConst::JUDGETIMING_AUTO] = CSVTextConst::OPTION_JUDGEAUTO;
	settingStringIdx[CSVPlayOptionConst::PLAYMODE] = CSVTextConst::OPTION_PLAYMODE;
	settingStringIdx[CSVPlayOptionConst::TARGET] = CSVTextConst::NAME_RIVAL;
}

/* *************************************
 * CSVPlayOption - change value
 * *************************************
 */

void CSVPlayOption::option_next(int val) {
	// TODO: process some exception case (hispeed, shutter)
	switch (val) {
	case CSVPlayOptionConst::HISPEED_1P:
	case CSVPlayOptionConst::HISPEED_2P:
		option[val]+=10;
		break;
	case CSVPlayOptionConst::TARGETRATE:
		option[val]++;
		if (option[val] > 100)
			option[val] = 100;
		break;
	case CSVPlayOptionConst::SHUTTER:
		option[val]+=10;
		break;
	case CSVPlayOptionConst::JUDGETIMING:
		option[val]++;
		break;
	default:
		option[val]++;
		if (option[val] >= settingString[val].size())
			option[val] = 0;
		break;
	}
}

void CSVPlayOption::option_prev(int val) {
	// TODO: process some exception case (hispeed, shutter)
	switch (val) {
	case CSVPlayOptionConst::HISPEED_1P:
	case CSVPlayOptionConst::HISPEED_2P:
		option[val]-=10;
		if (option[val]<0) option[val]=0;
		break;
	case CSVPlayOptionConst::TARGETRATE:
		option[val]--;
		if (option[val] < 0)
			option[val] = 0;
		break;
	case CSVPlayOptionConst::SHUTTER:
		option[val]-=10;
		if (option[val]<0) option[val]=0;
		break;
	case CSVPlayOptionConst::JUDGETIMING:
		option[val]--;
		break;
	default:
		option[val]--;
		if (option[val] < 0)
			option[val] = settingString[val].size()-1;
		break;
	}
}

const std::wstring& CSVPlayOption::option_getstring(int val) {
	// TODO: process some exception case (hispeed, shutter)
	switch (val) {
	case CSVPlayOptionConst::HISPEED_1P:
	case CSVPlayOptionConst::HISPEED_2P:
		return boost::str(boost::wformat(L"HISPEED %d") % option[val]);
	case CSVPlayOptionConst::SHUTTER:
		return boost::str(boost::wformat(L"SHUTTER %d") % option[val]);
	default:
		return settingString[ val ][ option[val] ];
	}
}

void CSVPlayOption::option_applytext() {
	std::wstring str;
	for (int i=0; i<50; i++) {
		if (settingStringIdx[i]) {
			CSVText::SetText(settingStringIdx[i], option_getstring(i));
		}
	}

	// apply number
	CSVNumber::setNumber(CSVNumConst::HISPEED_1P, option[CSVPlayOptionConst::HISPEED_1P]);
	CSVNumber::setNumber(CSVNumConst::HISPEED_2P, option[CSVPlayOptionConst::HISPEED_2P]);

	// apply switch
	// DOUBLE or DOUBLE BATTLE: op12 on
	// TODO: D-BATTLE, G-BATTLE(ghost), SP-TO-DP (or reverse)
}