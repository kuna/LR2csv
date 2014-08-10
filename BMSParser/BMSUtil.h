#pragma once
#include "Stdafx.h"
#include "md5.h"

class BMSKeyData;	// redeclaration;

class BMSUtil {
public:
	static void Log(std::wstring title, std::wstring desc) {
		/* your customize part ! */
		OutputDebugString((title + L" - " + desc).c_str());
	}
	
	static std::wstring CheckEncoding(BYTE BOM[]);
	static std::wstring GetHash(BYTE data[], int len);
	static bool IsInteger(std::wstring str);
	static int ExtHexToInt(std::wstring hex);
	static std::wstring IntToExtHex(int val);
	static int HexToInt(std::wstring hex);
	static std::wstring IntTo2Hex(int val);
	static std::vector<BMSKeyData*> cloneKeyArray(std::vector<BMSKeyData*> &bmsdata);
	static std::vector<BMSKeyData*> ExtractChannel(std::vector<BMSKeyData*> data, int channel);
	static std::vector<BMSKeyData*> ExtractLayer(std::vector<BMSKeyData*> data, int layer);
};