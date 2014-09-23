#include "StdAfx.h"
#include "CSVParser.h"
#include "CSVFile.h"

/*
 * CSVParser::ReadCSVFile(TCHAR *path)
 * read CSV file and fill self object.
 */
bool CSVParser::ReadCSVFile(const wchar_t *path) {
	// read csv file
	std::wstring fileData_wstr;
	if (!ReadFileString(path, fileData_wstr))
		return false;
	TCHAR *fileData;
	fileData = (TCHAR*)calloc(fileData_wstr.length()+10, sizeof(TCHAR));
	wcscpy(fileData, fileData_wstr.c_str());
	
	// parse
	splitLine(fileData);

	// release
	free(fileData);
	fileData = 0;

	return true;
}

bool CSVParser::ReadFileString(const wchar_t *path, std::wstring& text) {
	// convert path
	TCHAR absolutePath[256];
	CSVFile::GetPathFromSettings(path, absolutePath);

	// read csv file
	if (!AutoEncoder::readfileEncodingAuto(absolutePath, text))
		return false;
	return true;
}

void CSVParser::splitLine(TCHAR *data) {
	// line parse
	TCHAR *n = NULL;
	n = wcstok(data, L"\n");
	while (n != NULL)
	{
		if (n[wcslen(n)-1] == L'\r')
			n[wcslen(n)-1] = 0;
		
		addLine(n);

		n = wcstok(NULL, L"\n");
	}
}

// TODO: remove comment
void CSVParser::addLine(TCHAR *t) {
	if (wcsncmp(t, L"//", 2) == 0)
		return;

	lines.push_back(t);
}

// used when include file
// in current line
bool CSVParser::includeFile(TCHAR *path) {
	// read csv file
	std::wstring fileData_wstr;
	if (!ReadFileString(path, fileData_wstr))
		return false;
	TCHAR *fileData;
	fileData = (TCHAR*)calloc(fileData_wstr.length()+10, sizeof(TCHAR));
	wcscpy(fileData, fileData_wstr.c_str());
	
	// parse
	splitLine(fileData);

	// release
	free(fileData);
	fileData = 0;
	return true;
}