#include "Stdafx.h"
#include "CSVText.h"

std::wstring CSVText::data[200];

std::wstring &CSVText::GetText(int num) {
	return data[num];
}

void CSVText::SetText(int num, std::wstring &str) {
	data[num] = str;
}