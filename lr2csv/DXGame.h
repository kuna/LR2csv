
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")
#include <d3d9.h>
#include <d3dx9.h>
#include "DXFont.h"

#define MSGBOX_TITLE L"LR2CSV"
#define MSGBOX_INFO(HWND, MSG) (MessageBox(HWND, MSG, MSGBOX_TITLE, MB_ICONINFORMATION))
#define MSGBOX_ERROR(HWND, MSG) (MessageBox(HWND, MSG, MSGBOX_TITLE, MB_ICONERROR))


class DXTexture {
protected:
	RECT txtRect;
	LPDIRECT3DTEXTURE9 pTexture;
public:
	BOOL LoadTexture(const TCHAR *path, IDirect3DDevice9* pd3dDevice);
	BOOL isTextureLoaded();
	RECT* GetRect();
	LPDIRECT3DTEXTURE9 GetTexture();

	int width;
	int height;
};


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
	HWND MakeWindow(LRESULT (WINAPI *MsgProc)( HWND, UINT, WPARAM, LPARAM ), TCHAR *wndName, int width, int height);
	BOOL Initalize(HWND hWnd);

	VOID ChangeMode(BOOL fullscreen);

	BOOL Release();

	BOOL LoadTexture(const TCHAR *path, LPDIRECT3DTEXTURE9 *pTexture);
	BOOL CreateSprite(LPD3DXSPRITE *sprite);
	BOOL CreateFont(LPD3DXFONT *font, TCHAR *name, int height, int width=0);
	BOOL InitalizeFont(DXFont*, CSVFont *font);

	VOID Clear(int a, int r, int g, int b);
	VOID BeginScene();
	VOID EndScene();
	
	VOID BeginSprite();
	VOID DrawTexture(DXTexture *texture, RECT *srcRect, RECT *dstRect, D3DXCOLOR rgba, D3DXVECTOR2 *rotateCentre, double rotation, int blending, int filter);
	VOID DrawString(DXFont *font, TCHAR *str, RECT *r, D3DCOLOR clr, int align);
	VOID EndSprite();

	IDirect3D9* GetD3D9();
	IDirect3DDevice9* GetD3D9Device();
};