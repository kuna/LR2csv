#include "DXTexture.h"
#include <string>
#include <algorithm>

// for avi playback
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "vfw32.lib")

//
bool ends_with(std::wstring const &a, std::wstring const &b) {
    auto len = b.length();
    auto pos = a.length() - len;
    if (pos < 0)
        return false;
    auto pos_a = &a[pos];
    auto pos_b = &b[0];
    while (*pos_a)
        if (*pos_a++ != *pos_b++)
            return false;
    return true;
}

TCHAR easytolower(TCHAR in){
  if(in<=L'Z' && in>=L'A')
    return in-(L'Z'-L'z');
  return in;
}

#define TOLOWERW(x) (std::transform((x).begin(), (x).end(), (x).begin(), easytolower))

BOOL DXTexture::LoadTexture(const TCHAR *path, IDirect3DDevice9* pd3dDevice)
{
	// check is file avi?
	std::wstring data(path);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	if (ends_with(data, L".avi")) {
		// in case of avi
		// http://www.gamedev.net/page/resources/_/technical/game-programming/working-with-avi-files-r840
		AVIFileInit();
		if (AVIFileOpen(&pAvi, path, OF_READ, 0)) {
			// error
			return FALSE;
		}

		// get avi info
		// to get width, height.
		AVIFileInfo(pAvi, &aviInfo, sizeof(aviInfo));

		// get video stream
		/*PAVISTREAM pVideo[100];
		int nNumAviStream = 0;
		do {
			if (AVIFileGetStream(pAvi, &pVideo[nNumAviStream],
				streamtypeVIDEO, nNumAviStream))
				break;
		} while (++nNumAviStream && nNumAviStream < 100);
		if (!nNumAviStream)
			return FALSE;
		PAVISTREAM pStream = pVideo[0];	// first one is default stream*/
		if (AVIFileGetStream(pAvi, &pStream, streamtypeVIDEO, 0))
			return FALSE;

		// get end time
		aviTime = AVIStreamEndTime(pStream);

		// processing video stream
		frame =	AVIStreamGetFrameOpen(pStream, 0);
		if (!frame)
			return FALSE;

		// create empty texture for future drawing
		if (FAILED(D3DXCreateTexture(pd3dDevice, aviInfo.dwWidth, aviInfo.dwHeight,
			1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture)))
			return FALSE;
	} else {
		// in case of image
		// check if texture is already exists ...
		if (pTexture != 0)
			pTexture->Release();

		D3DXIMAGE_INFO pImgInf;
		if (FAILED(D3DXCreateTextureFromFileEx(pd3dDevice, path,
			D3DX_DEFAULT, D3DX_DEFAULT, 
			1,	// Mip level
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 0x00000001, 0x00000001,
			0x00000000,	//
			&pImgInf,
			NULL,
			&pTexture))) return FALSE;

		width = txtRect.right = pImgInf.Width;
		height = txtRect.bottom = pImgInf.Height;
	}

	return TRUE;
}

RECT* DXTexture::GetRect() {
	return &txtRect;
}

int DXTexture::getMovieTime() {
	if (!aviTime)
		return 0;
	
	DWORD ntime = GetTickCount();
	int r = ntime - aviTime;
	aviTime = ntime;
	return r;
}

VOID DXTexture::updateMovieTime() {
	aviTime = GetTickCount();
}

LPDIRECT3DTEXTURE9 DXTexture::GetTexture() {
	// you should not store texture
	// because texture will be created every time
	// in case of movie file.
	if (pAvi) {
		// get frame
		int frameTime = AVIStreamTimeToSample(pStream, getMovieTime());

		LPBITMAPINFOHEADER bitmap;
		bitmap = (LPBITMAPINFOHEADER)AVIStreamGetFrame(frame, frameTime);

		// For 16, 24, or 32bit image formats
		//LPBYTE pData = (LPBYTE)bitmap + bitmap->biSize;

		// For 8bit image formats
		//LPBYTE pData = lpbi + lpbi->biSize + 256 * sizeof(RGBQUAD);

		// create surface
		LPDIRECT3DSURFACE9 pSurface;
		HDC hdc;
		pTexture->GetSurfaceLevel(0, &pSurface);
		if (SUCCEEDED(pSurface->GetDC(&hdc))) {
			// prepare
			LPBYTE pGDIBits;
			HBITMAP hBmp = CreateDIBSection(hdc, (LPBITMAPINFO)bitmap, DIB_RGB_COLORS, (LPVOID*)&pGDIBits, NULL, 0);

			// copy bitmap data
			HDC hBmpDC = CreateCompatibleDC(hdc);
			HGDIOBJ pOldObj = SelectObject(hBmpDC, hBmp);
			BitBlt(hdc, 0, 0, aviInfo.dwWidth, aviInfo.dwHeight,
				hBmpDC, 0, 0, SRCCOPY);

			DeleteObject(hBmp);
			DeleteDC(hBmpDC);
			pSurface->ReleaseDC(hdc);
		}

		pSurface->Release();

		// For 16, 24, or 32bit image formats
		//LPBYTE pData = (LPBYTE)bitmap + bitmap->biSize;

		// For 8bit image formats
		//LPBYTE pData = lpbi + lpbi->biSize + 256 * sizeof(RGBQUAD);
	} else {
	}

	return pTexture;
}

BOOL DXTexture::isTextureLoaded() {
	return (pTexture != 0);
}

BOOL DXTexture::Release() {
	// texture release
	if (pTexture) {
		pTexture->Release();
		pTexture = 0;
	}

	// stream & avi file release
	if (pAvi) {
		AVIStreamRelease(pStream);
		AVIFileRelease(pAvi);
		AVIFileExit();
		pAvi = 0;
	}

	return TRUE;
}