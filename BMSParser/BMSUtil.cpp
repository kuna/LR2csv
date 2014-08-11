#include "Stdafx.h"
#include "BMSUtil.h"
#include "BMSKeyData.h"
#include <boost/regex.hpp>

std::wstring BMSUtil::GetHash(BYTE data[], int len) {
	return md5::GetHash((const char*)data, len);
}
	
bool BMSUtil::IsInteger(std::wstring str) {
	// requires boost
	boost::wregex condition(L"-?[0-9]+");
	return boost::regex_match(str, condition);
}
	
int BMSUtil::ExtHexToInt(std::wstring hex) {
	TCHAR sample[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int r = 0;
	for (int i=0; i<hex.length(); i++) {
		r *= 36;
		for (int j=0; j<wcslen(sample); j++) {
			if (hex[i] == sample[j]) {
				r += j;
				continue;
			}
		}
	}
		
	return r;
}
	
std::wstring BMSUtil::IntToExtHex(int val) {
	TCHAR sample[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	TCHAR res[2];
	res[2] = 0;
	res[0] = sample[val/36];
	res[1] = sample[val%36];
	return res;
}
	
int BMSUtil::HexToInt(std::wstring hex) {
	int r = wcstol(hex.c_str(), 0, 16);
	return r;
}
	
std::wstring BMSUtil::IntTo2Hex(int val) {
	TCHAR r[3];
	wsprintf(r, L"%02X", val);
	return r;
}
	
std::vector<BMSKeyData*> BMSUtil::cloneKeyArray(std::vector<BMSKeyData*> &bmsdata) {
	std::vector<BMSKeyData*> a = bmsdata;
	return a;
}
	
std::vector<BMSKeyData*> BMSUtil::ExtractChannel(std::vector<BMSKeyData*> data, int channel) {
	std::vector<BMSKeyData*> r;
	for (int i=0; i<data.size(); i++) {
		BMSKeyData *b = data[i];
		if (b->getChannel() == channel) {
			r.push_back(b);
		}
	}
		
	return r;
}
	
std::vector<BMSKeyData*> BMSUtil::ExtractLayer(std::vector<BMSKeyData*> data, int layer) {
	std::vector<BMSKeyData*> r;
	for (int i=0; i<data.size(); i++) {
		BMSKeyData *b = data[i];
		if (b->getLayerNum() == layer) {
			r.push_back(b);
		}
	}
		
	return r;
}