#include "Stdafx.h"
#include "CSVFile.h"
#include "CSVSettings.h"

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

void CSVFile::GetAbsolutePath(const TCHAR *lr2Path, TCHAR *out) {
	GetCurrentExecutionPath();		// initalization
	TCHAR t[256];
	wcscpy(t, exePath);
	wcscat(t, lr2Path);
	wcscpy(out, t);
}

bool CSVFile::GetFileList(const TCHAR *lr2Path, std::vector<std::wstring> &resArr) {
	TCHAR absolutePath[256];
	GetAbsolutePath(lr2Path, absolutePath);

	// check if absolutePath is already exists ...
	FILE *f = _wfopen(absolutePath, L"r");
	if (f) {
		// not directory just add one
		fclose(f);
		resArr.push_back(absolutePath);
		return true;
	}
	
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(absolutePath, &fd); 
    if(hFind != INVALID_HANDLE_VALUE) 
    { 
        do
        {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
            {
				TCHAR drv[256], dir[256];
				_wsplitpath(absolutePath, drv, dir, 0, 0);
				wcscat(drv, dir);
				wcscat(drv, fd.cFileName);
                resArr.push_back(drv);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 

		return true;
    } else {
		return false;
	}
}

bool CSVFile::GetPathFromSettings(const TCHAR *lr2Path, TCHAR *out) {
	std::vector<std::wstring> files;
	GetFileList(lr2Path, files);

	if (files.size() == 0) {
		return false;
	} else if (files.size() == 1) {
		wcscpy(out, files[0].c_str());
		return true;
	} else {
		// search settings
		TCHAR keyval[256];
		if (CSVSettings::GetKeyValue(lr2Path, keyval)) {
			// TODO: copy path
			//wcscpy(out, files[0].c_str());
		} else {
			// index 0
			wcscpy(out, files[0].c_str());
		}
		return true;
	}
}