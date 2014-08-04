#include "CSVFont.h"

void CSVFont::SetData(TCHAR *args[]) {
	// (index), (width), (border), (fontcolor)a, r, g, b, (bordercolor)a, r, g, b, (texture)
	// width 0 is the default.
	fontWidth = _wtoi(args[2]);
	borderWidth = _wtoi(args[3]);
	fontA = _wtoi(args[4]);
	fontR = _wtoi(args[5]);
	fontG = _wtoi(args[6]);
	fontB = _wtoi(args[7]);
	borderA = _wtoi(args[8]);
	borderR = _wtoi(args[9]);
	borderG = _wtoi(args[10]);
	borderB = _wtoi(args[11]);
	if (args[12] > 0)
		wcscpy(fontTexturePath, args[12]);
}