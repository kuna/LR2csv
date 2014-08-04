#include "DXFont.h"

int DXFont::DXFontCnt=0;
FT_Library DXFont::ftLib;
	
BOOL DXFont::InitDXFont() {
	if (!(DXFontCnt++)) {
		if (FT_Init_FreeType(&ftLib)) {
			return FALSE;
		}
	}

	// make font (fallback)
	int err = FT_New_Face(ftLib, "C:\\Users\\kuna\\Documents\\Visual Studio 2010\\Projects\\lr2csv\\Debug\\gulim.ttc", 0, &ftFace);
	if (err)
		return FALSE;

	if (FT_Stroker_New(ftLib, &ftStroker))
		return FALSE;

	texture_cache.clear();
	return TRUE;
}

BOOL DXFont::Release() {
	if (!--DXFontCnt) {
		return FT_Done_FreeType(ftLib);
	}
	
	std::map<int,DXFontTexture*>::iterator it;
	for (it=texture_cache.begin(); it != texture_cache.end(); ++it) {
		delete it->second;
	}
	texture_cache.clear();

	return TRUE;
}


BOOL DXFont::RenderChar(TCHAR chr, int *width, int *height) {
	// get glyph index
	int index = FT_Get_Char_Index(ftFace, chr);
	if (index == 0) {
		return FALSE;
	}

	// set size
	FT_Set_Char_Size(ftFace, fontData->fontHeight * 64, 0,
		96, 96);	// 96 is default DPI
 
	// load glyph
	FT_Load_Glyph(ftFace,index,FT_LOAD_DEFAULT);
 
	// do rendering
	FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);

	if (width!=0)
		*width = ftFace->glyph->bitmap.width;
	if (height != 0)
		*height = fontData->fontHeight;//ftFace->glyph->bitmap.rows;

	return TRUE;
}

BOOL DXFont::drawChar(HDC hdc, int x, int y) {
	int width = ftFace->glyph->bitmap.width;
	int height = ftFace->glyph->bitmap.rows;

	if (hdc != 0 && width>0 && height>0) {
		for (int i=0; i<width; i++) {
			for (int j=0; j<height; j++) {
				int Color = ftFace->glyph->bitmap.buffer[j*width+i];
				if (Color > 0) {
					SetPixel(hdc, 
						ftFace->glyph->bitmap_left +x+i, 
						32-ftFace->glyph->bitmap_top +y+j,
						RGB(255,255,255));
				}
			}
		}
	}

	// if stroke exists?
	// then draw it
	if (fontData->borderWidth > 0 && FALSE) {
		FT_Glyph glyph;
		FT_Get_Glyph(ftFace->glyph, &glyph);

		FT_Vector origin = {0,0};
		FT_Stroker_Set(ftStroker, 32, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND,
			0);
		FT_Glyph_Stroke(&glyph, ftStroker, 1);
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &origin, 1);
		
		// TODO just check it.
		width = ((FT_BitmapGlyph)glyph)->bitmap.width;
		height = ((FT_BitmapGlyph)glyph)->bitmap.rows;

		if (hdc != 0) {
			for (int i=0; i<width; i++) {
				for (int j=0; j<height; j++) {
					int Color = ((FT_BitmapGlyph)glyph)->bitmap.buffer[j*width+i];
					if (Color > 0) {
						SetPixel(hdc, x+i, y+j, RGB(128,128,128));
					}
				}
			}
		}

		FT_Done_Glyph(glyph);
	}

	return TRUE;
}

DXFontTexture* DXFont::getFontTexture(IDirect3DDevice9* pd3dDevice, TCHAR chr, int *width, int *height) {
	std::map<int, DXFontTexture*>::iterator it;
	int index = FT_Get_Char_Index(ftFace, chr);
	it = texture_cache.find(index);
	if (it == texture_cache.end()) {
		// render character
		int wid, hei;
		if (RenderChar(chr, &wid, &hei)) {
			// create texture
			LPDIRECT3DTEXTURE9 pTex;
			if( FAILED( pd3dDevice->CreateTexture( wid, hei, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL ) ) )
			{
				// texture creation failed
				return 0;
			}

			HDC hdc;
			LPDIRECT3DSURFACE9 pSurface;

			// draw text
			pTex->GetSurfaceLevel( 0, &pSurface );
			pSurface->GetDC( &hdc );
			if( hdc )
			{
				drawChar(hdc);
			}
			pSurface->Release();

			// add to array
			DXFontTexture *t = new DXFontTexture();
			t->width = wid;
			t->height = hei;
			t->texture = pTex;
			texture_cache.insert(std::make_pair(index, t));

			if (width != 0)
				*width = wid;
			if (height != 0)
				*height = hei;

			return t;
		} else {
			return 0;
		}
	} else {
		return it->second;
	}
}

VOID DXFont::drawString(HDC hdc, TCHAR *str, int width, int size, int align, LPRECT r) {
	// first calculate all string size
	/*
	int strLen = wcslen(str);
	RECT chrRect;
	int strWidth=0, strHeight=0;
	for (int i=0; i<strLen; i++) {
		drawChar(0, str[i], 0, 0, &chrRect);
		strWidth += chrRect.right;
		if (chrRect.bottom > strHeight) strHeight = chrRect.bottom;
	}

	// calculated, then input scale
	double sHeight = (double)size / fontData->fontHeight;
	double sWidth = (double)width / (strWidth*sHeight);
	if (sWidth > 1) sWidth = 1;
	double sWidth2 = sWidth * sHeight;	// for scaling

	// now draw font (transformed)
	// calculate start x point
	int sx = 0;
	if (align = 1) {
		sx = (width - sWidth2*strWidth) / 2;
	} else if (align = 2) {
		sx = width - sWidth2*strWidth;
	}

	if (r != 0) {
		r->right = sWidth;
		r->bottom = sHeight;
	}*/
}

DXFontTexture::~DXFontTexture() {
	if (texture)
		texture->Release();
}