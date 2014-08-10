#include "Stdafx.h"
#include "BMSUtil.h"
#include "BMSKeyData.h"
#include <boost/regex.hpp>

std::wstring BMSUtil::CheckEncoding(BYTE BOM[]) {
	if( (BOM[0] & 0xFF) == 0xEF && (BOM[1] & 0xFF) == 0xBB && (BOM[2] & 0xFF) == 0xBF )
		return L"UTF-8";
	else if( (BOM[0] & 0xFF) == 0xFE && (BOM[1] & 0xFF) == 0xFF )
		return L"UTF-16BE";
	else if( (BOM[0] & 0xFF) == 0xFF && (BOM[1] & 0xFF) == 0xFE )
		return L"UTF-16LE";
	else if( (BOM[0] & 0xFF) == 0x00 && (BOM[1] & 0xFF) == 0x00 && 
			(BOM[0] & 0xFF) == 0xFE && (BOM[1] & 0xFF) == 0xFF )
		return L"UTF-32BE";
	else if( (BOM[0] & 0xFF) == 0xFF && (BOM[1] & 0xFF) == 0xFE && 
			(BOM[0] & 0xFF) == 0x00 && (BOM[1] & 0xFF) == 0x00 )
		return L"UTF-32LE";
	else
		return L"ANSI";
}

std::wstring BMSUtil::GetHash(BYTE data[], int len) {
	// requires md5.h
	md5_state_t state;
	md5_byte_t digest[16];
	TCHAR hex_output[16*2 + 1];
	int di;

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)data, len);
	md5_finish(&state, digest);

	for (di = 0; di < 16; ++di)
		wsprintf(hex_output + di * 2, L"%02x", digest[di]);
	
	return hex_output;
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