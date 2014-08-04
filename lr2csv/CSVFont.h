// we use fontface, not bitmapped font.
#include <tchar.h>
#pragma once

class CSVFont {
public:
	TCHAR fontFace[20];
	TCHAR fontTexturePath[256];
	int fontHeight, fontWidth;
	int fontThickness;
	int fontA, fontR, fontG, fontB;
	int borderA, borderR, borderG, borderB;
	int borderWidth;

public:
	void SetData(TCHAR *args[]);
};