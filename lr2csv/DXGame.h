#pragma once
#include "Stdafx.h"
#include "DXFont.h"
#include "Scene.h"

#define CLASSNAME L"DXFREETYPE"
#define MSGBOX_TITLE L"DXFREETYPE"
#define MSGBOX_INFO(HWND, MSG) (MessageBox(HWND, MSG, MSGBOX_TITLE, MB_ICONINFORMATION))
#define MSGBOX_ERROR(HWND, MSG) (MessageBox(HWND, MSG, MSGBOX_TITLE, MB_ICONERROR))

class DXGame {
private:
	WNDCLASSEX wc;
	HWND m_hWnd;

	IDirect3D9*		pd3d9;
	IDirect3DDevice9* pd3dDevice;
	IDirect3DSurface9* BackBuff;

	D3DPRESENT_PARAMETERS pp;

	int screenWidth;
	int screenHeight;

	int fps;
	int countedFrame;
	DWORD fpsTermStart;
	VOID TickFPS();
public:
	LPD3DXSPRITE sprite;
	HWND MakeWindow(TCHAR *wndName, int width, int height);
	BOOL Initalize(HWND hWnd);

	VOID ChangeMode(BOOL fullscreen);

	BOOL Release();

	BOOL LoadTexture(const TCHAR *path, LPDIRECT3DTEXTURE9 *pTexture);
	BOOL CreateSprite(LPD3DXSPRITE *sprite);
	BOOL CreateFont(LPD3DXFONT *font, TCHAR *name, int height, int width=0);

	VOID Clear(int a, int r, int g, int b);
	VOID BeginScene();
	VOID EndScene();
	
	VOID BeginSprite();
	VOID DrawTexture(DXTexture *texture, RECT *srcRect, RECT *dstRect, D3DXCOLOR rgba, D3DXVECTOR2 *rotateCentre, double rotation, int blending, int filter);
	VOID DrawString(DXFont *font, const TCHAR *str, int x, int y, int width, int size, int align, D3DCOLOR clr=0xFFFFFFFF);
	VOID EndSprite();

	IDirect3D9* GetD3D9();
	IDirect3DDevice9* GetD3D9Device();

public:
	Scene *currentScene;
};