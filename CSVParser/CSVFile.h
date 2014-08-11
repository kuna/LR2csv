#include <tchar.h>
#include <vector>
#include <string>
#pragma once

class CSVFile {
private:
	static TCHAR exePath[256];

public:
	static void MakePathCorrect(TCHAR *jis_path);
	static void GetCurrentExecutionPath(TCHAR *out=0);
	static void GetAbsolutePath(const TCHAR *lr2Path, TCHAR *absolutePath);
	static bool GetFileList(const TCHAR *lr2Path, std::vector<std::wstring> &resArr);
	static bool GetPathFromSettings(const TCHAR *lr2Path, TCHAR *out);
};