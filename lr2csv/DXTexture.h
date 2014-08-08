#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Vfw.h>

class DXTexture {
private:
	PAVIFILE pAvi;
	AVIFILEINFO aviInfo;
	PAVISTREAM pStream;
	PGETFRAME frame;
	int aviTime;

	RECT txtRect;
	LPDIRECT3DTEXTURE9 pTexture;

	DWORD startTime;
	int getMovieTime();
public:
	BOOL LoadTexture(const TCHAR *path, IDirect3DDevice9* pd3dDevice);
	BOOL isTextureLoaded();
	RECT* GetRect();
	LPDIRECT3DTEXTURE9 GetTexture();
	BOOL Release();
	VOID updateMovieTime();

	int width;
	int height;
};
