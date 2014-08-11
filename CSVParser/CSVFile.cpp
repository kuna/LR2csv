#include "Stdafx.h"
#include "CSVFile.h"
#include "CSVSettings.h"
#include <boost/filesystem.hpp>	// for scan
#include <boost/algorithm/string/predicate.hpp>	// endswith

TCHAR CSVFile::exePath[256];

void CSVFile::GetCurrentExecutionPath(TCHAR *out) {
	if (wcslen(exePath) == 0) {
		GetModuleFileName(NULL, exePath, 256);
		for (int i=wcslen(exePath)-1; i>=0 && exePath[i]!=L'\\'; i--) {
			exePath[i] = 0;
		}
	}

	if (out != 0) {
		wcscpy(out, exePath);
	}
}

void CSVFile::MakePathCorrect(TCHAR *jis_path) {
	for (int i=0; i<wcslen(jis_path); i++) {
		if (jis_path[i] == L'¥') {
			jis_path[i] = L'\\';
		}
	}
}

void CSVFile::GetAbsolutePath(const TCHAR *lr2Path, TCHAR *out) {
	GetCurrentExecutionPath();		// initalization
	TCHAR t[256];
	wcscpy(t, exePath);
	wcscat(t, lr2Path);
	MakePathCorrect(t);
	wcscpy(out, t);
}

bool CSVFile::GetFileList(const TCHAR *lr2Path, std::vector<std::wstring> &resArr) {
	TCHAR absolutePath[256];
	GetAbsolutePath(lr2Path, absolutePath);

	// split path
	int i;
	for (i=0; i<wcslen(absolutePath); i++) {
		if (absolutePath[i] == L'*')
			break;
	}
	if (i == wcslen(absolutePath)) {
		// just return
		if (boost::filesystem::exists(absolutePath)) {
			resArr.push_back(absolutePath);
		} else {
			return false;
		}
	} else {
		// split path
		absolutePath[i] = 0;
		std::wstring path_first = absolutePath;
		std::wstring path_last = absolutePath+i+1;

		// is we searching directory or file?
		bool searchFile = false;
		if (path_last.find(L'\\') == std::wstring::npos) {
			searchFile = true;
		}

		// find all and add
		boost::filesystem::path dir(path_first);
		boost::filesystem::directory_iterator end_it;
		if (boost::filesystem::exists(path_first)) {
			for (boost::filesystem::directory_iterator it(dir); it!=end_it; ++it) {
				if ((searchFile && boost::filesystem::is_regular_file(it->status())) ||
					(!searchFile && !boost::filesystem::is_regular_file(it->status()))) {
					std::wstring path = (*it).path().wstring();
					if (searchFile) {
						// check ext
						if (!boost::algorithm::ends_with(path, path_last))
							continue;
					} else {
						// add last path
						// and check existance
						path += path_last;
						if (!boost::filesystem::exists(path))
							continue;
					}
					resArr.push_back(path);
				}
			}
		} else {
			return false;
		}
	}
}

bool CSVFile::GetPathFromSettings(const TCHAR *lr2Path, TCHAR *out) {
	// first of all, get setting and replace path
	std::wstring keyval;
	bool foundSetting = false;
	if (CSVSettings::GetPathValue(lr2Path, keyval)) {
		std::wstring wpath = lr2Path;
		int p = wpath.find(L'*');
		if (p != std::wstring::npos) {
			wpath.replace(p, 1, keyval);
			if (boost::filesystem::exists(wpath)) {
				wcscpy(out, wpath.c_str());
				foundSetting = true;
			}
		}
	}

	if (foundSetting)
		return true;

	// when no setting found, then get default value
	std::vector<std::wstring> files;
	GetFileList(lr2Path, files);

	if (files.size() == 0) {
		return false;
	} else if (files.size() == 1) {
		wcscpy(out, files[0].c_str());
		return true;
	} else {
		// index 0 is default
		wcscpy(out, files[0].c_str());
		return true;
	}
}

void CSVFile::getOnlyFileName(std::wstring &path, std::wstring &outfn) {
	TCHAR fn[256];
	_wsplitpath(path.c_str(), 0, 0, fn, 0);
	outfn = fn;
}