#include <d3d9.h>
#include <d3dx9.h>
#include <list>
#include <map>
#include "CSVFont.h"
#pragma once

// freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
//#include FT_CACHE_H
#pragma comment(lib,"freetype253_D.lib")

class DXFontTexture {
public:
	~DXFontTexture();
	int width;
	int height;
	LPDIRECT3DTEXTURE9 texture;
};

class DXFont {
private:
	// drawing

	// for caching
	std::map<int, DXFontTexture*> texture_cache;

	// freetype
	static int DXFontCnt;
	static FT_Library ftLib;
	FT_Face ftFace;
	FT_Stroker ftStroker;

	// refer
	// http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=51&MAEULNO=20&no=8373
public:
	LPD3DXFONT font;		// unnecessary
	CSVFont *fontData;

	BOOL InitDXFont();
	BOOL Release();
	DXFontTexture* getFontTexture(IDirect3DDevice9* device, TCHAR chr, int *wid=0, int *hei=0);
	VOID drawString(HDC hdc, TCHAR *str, int width, int size, int align, LPRECT r=0);

	BOOL RenderChar(TCHAR chr, int *width=0, int *height=0);
	BOOL drawChar(HDC hdc, int x=0, int y=0);
};