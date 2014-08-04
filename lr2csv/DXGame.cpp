﻿#include <Windows.h>
#include "DXGame.h"

// for GDI+ (font processing)
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#define CLASSNAME L"LR2CSV"

// for GDIPLUS
ULONG_PTR m_gdiplusToken;

HWND DXGame::MakeWindow(LRESULT (WINAPI *MsgProc)( HWND, UINT, WPARAM, LPARAM ), TCHAR *wndName, int width, int height) {
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        CLASSNAME, NULL
    };
    RegisterClassEx( &wc );
	
	RECT wndRect;
	SetRect(&wndRect, 0, 0, width, height);
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindow( CLASSNAME, wndName,
                              WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
							  wndRect.right-wndRect.left, wndRect.bottom-wndRect.top,
                              NULL, NULL, wc.hInstance, NULL );

	screenWidth = width;
	screenHeight = height;

	return m_hWnd;
}

BOOL DXGame::Initalize(HWND hWnd) {
	m_hWnd = hWnd;
	HRESULT hr;

	// IDirect9 Initalize
	pd3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d9) return FALSE;

	// IDirectx9 Device Initalize
	D3DFORMAT format=D3DFMT_R5G6B5; //For simplicity we'll hard-code this for now.
	ZeroMemory(&pp,sizeof(D3DPRESENT_PARAMETERS));
	pp.BackBufferCount= 1;  //We only need a single back buffer
	pp.MultiSampleType=D3DMULTISAMPLE_NONE; //No multi-sampling
	pp.MultiSampleQuality=0;                //No multi-sampling
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // Throw away previous frames, we don't need them
	pp.hDeviceWindow=hWnd;  //This is our main (and only) window
	pp.Windowed = TRUE;
	pp.BackBufferWidth = screenWidth;
	pp.BackBufferHeight = screenHeight;
	pp.Flags = 0;            // No flags to set
	pp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
	pp.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;   //Default Presentation rate
	pp.BackBufferFormat=format;      //Display format
	pp.EnableAutoDepthStencil=FALSE; //No depth/stencil buffer

	hr = pd3d9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&pp,
		&pd3dDevice);
	if (FAILED(hr)) return FALSE;

	// initalize sprite also
	// refer to http://skmagic.tistory.com/entry/ID3DXSprite-Interface-LPD3DXSPRITE-%EC%82%AC%EC%9A%A9%ED%95%98%EA%B8%B0
	CreateSprite(&sprite);

	// also initalize GDIPlus ...
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	return TRUE;
}

VOID DXGame::ChangeMode(BOOL fullscreen) {
	pp.Windowed = !fullscreen;
	pd3dDevice->Reset(&pp);
}

BOOL DXGame::CreateSprite(LPD3DXSPRITE *sprite) {
	D3DXCreateSprite(pd3dDevice, sprite);
	return TRUE;
}

BOOL DXGame::InitalizeFont(DXFont *font, CSVFont *csvFont) {
	font->fontData = csvFont;
	if (FAILED(CreateFont(&font->font, csvFont->fontFace, csvFont->fontHeight, csvFont->fontWidth))) {
		// create with default font face
		return CreateFont(&font->font, L"Malgun Gothic", csvFont->fontHeight, csvFont->fontWidth);
	}

	return TRUE;
}

BOOL DXGame::CreateFont(LPD3DXFONT *font, TCHAR *name, int height, int width) {
    D3DXFONT_DESC desc = {
        height,
        width,
        400,
        0,
        false,
        DEFAULT_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_PITCH
    };
	//ZeroMemory(&fontdesc, sizeof(fontdesc));
	/*
	fontdesc.Height = height;
	fontdesc.Width = width;
	fontdesc.Weight = 400;
	fontdesc.MipLevels = 0;
	fontdesc.Italic = 0;

	fontdesc.CharSet = DEFAULT_CHARSET;
	fontdesc.Quality = DEFAULT_QUALITY;
	fontdesc.PitchAndFamily = FIXED_PITCH;*/
	wcscpy(desc.FaceName, name);

	return SUCCEEDED(D3DXCreateFontIndirect(pd3dDevice, &desc, font));
}

BOOL DXGame::LoadTexture(const TCHAR *path, LPDIRECT3DTEXTURE9 *pTexture) {
	//D3DXCreateTextureFromFile(pd3dDevice, path, pTexture);
	D3DXIMAGE_INFO pImgInf;
	if (FAILED(D3DXCreateTextureFromFileEx(pd3dDevice, path,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1,	// Mip level
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 0x00000001, 0x00000001,
		0x00000000,
		&pImgInf,
		NULL,
		pTexture))) return FALSE;

	return TRUE;
}

BOOL DXGame::Release() {
	// GDI+
	GdiplusShutdown(m_gdiplusToken);

	pd3dDevice->Release();
	pd3d9->Release();

	return TRUE;
}

VOID DXGame::Clear(int a, int r, int g, int b) {
	if (pd3dDevice)
		pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(a,r,g,b), 1.0f, 0);
}

VOID DXGame::BeginScene() {
	if (pd3dDevice)
		pd3dDevice->BeginScene();
}

VOID DXGame::EndScene() {
	if (pd3dDevice) {
		pd3dDevice->EndScene();
		pd3dDevice->Present(0, 0, 0, 0);

		// for FPS measuring
		TickFPS();
	}
}

VOID DXGame::TickFPS() {
	DWORD ntime = GetTickCount();
	if (ntime - fpsTermStart > 1000) {
		fps = countedFrame;
		countedFrame = 0;
		fpsTermStart = ntime;

		// FOR DEBUG
		TCHAR test[256];
		wsprintf(test, L"FPS is %d\n", fps);
		OutputDebugString(test);
	}

	countedFrame++;
}

IDirect3D9* DXGame::GetD3D9() {
	return pd3d9;
}

IDirect3DDevice9* DXGame::GetD3D9Device() {
	return pd3dDevice;
}

VOID DXGame::BeginSprite() {
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

VOID DXGame::DrawTexture(DXTexture *texture, RECT *srcRect, RECT *dstRect, D3DXCOLOR rgba, D3DXVECTOR2 *rotateCentre, double rotation, int blending, int filter) {
	// create position vector
	D3DXVECTOR2 position(dstRect->left, dstRect->top);

	// calculate scaling vector
#define GETWIDTH(X) ((X)->right-(X)->left)
#define GETHEIGHT(X) ((X)->bottom-(X)->top)
	D3DXVECTOR2 scaling( GETWIDTH(dstRect)/(float)GETWIDTH(srcRect),
		GETHEIGHT(dstRect)/(float)GETHEIGHT(srcRect) );
#undef GETWIDTH
#undef GETHEIGHT

	D3DXMATRIX mat, orgmat;
	sprite->GetTransform(&orgmat);

	// make new transform
	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat, &D3DXVECTOR2(0.0f, 0.0f), 0.0,
		&scaling, rotateCentre, rotation, &position);
	sprite->SetTransform(&mat);

	// set render state
	if (blending == 0) {
		// NONE
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	} else if (blending == 1) {
		// ALPHA BLENDING
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	} else if (blending == 2) {
		// ADD
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	} else if (blending == 3) {
		// SUBTRACT
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// set filter
	if (!filter) {
		pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	} else {
		pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	// draw to sprite
	sprite->Draw(texture->GetTexture(), srcRect, NULL, NULL, rgba);
	sprite->Flush();	// necessary for different blend drawing

	// restore
	sprite->SetTransform(&orgmat);
}

#define USEDRAW
#define USEFREETYPE
VOID DXGame::DrawString(DXFont *font, TCHAR *str, RECT *dst, D3DCOLOR clr, int align) {
	// TODO: textured-masked font
	// TODO: border
	// TODO: shadow
	// TODO: stretching/resize (matrix) - comparing size with original one

#ifdef USEDX
	D3DXMATRIX mat, orgmat;
	sprite->GetTransform(&orgmat);
	
	RECT rText;
	font->font->DrawTextW(sprite, str, -1, &rText, DT_CALCRECT, clr);

	// make new transform
	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	double scale = (double)(rText.bottom - rText.top) / (dst->bottom - dst->top);
	D3DXMatrixTransformation2D(&mat, &D3DXVECTOR2(dst->right, dst->top), 0.0,
		&D3DXVECTOR2(1.0f, 1.0f), &D3DXVECTOR2(0, 0), 0, &D3DXVECTOR2(0, 0));
	sprite->SetTransform(&mat);

	int align_val = DT_LEFT;
	if (align == 1)
		align_val = DT_CENTER;
	else if (align == 2)
		align_val = DT_RIGHT;
	font->font->DrawTextW(sprite, str, -1, dst, align, clr);
	sprite->Flush();
	
	sprite->SetTransform(&orgmat);
#endif

#ifdef USEFREETYPE
	int width = dst->right;
	int size = dst->bottom;

	RECT rText;
	font->drawString(0, str, width, size, align, &rText);

	// get texture (if not exists, then make new one)
	for (int i=0; i<wcslen(str); i++) {

	}
	
	// create texture
	LPDIRECT3DTEXTURE9 pTex;
	if( FAILED( pd3dDevice->CreateTexture( rText.right, rText.bottom, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL ) ) )
	{
		// texture creating failed. stop drawing
		return;
	}

	HDC hdc;
	LPDIRECT3DSURFACE9 pSurface;

	// draw text
	pTex->GetSurfaceLevel( 0, &pSurface );
	pSurface->GetDC( &hdc );
	if( hdc )
	{
		font->drawString(hdc, str, width, size, align, &rText);
	}
	
	// make new transform
	D3DXMATRIX mat, orgmat;
	sprite->GetTransform(&orgmat);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat, &D3DXVECTOR2(0.0f, 0.0f), 0.0,
		&D3DXVECTOR2(1, 1), &D3DXVECTOR2(0, 0), 0, &D3DXVECTOR2(dst->left, dst->top));
	sprite->SetTransform(&mat);

	// ALPHA BLENDING
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	sprite->Draw(pTex, 0, 0, 0, D3DCOLOR_XRGB(255,255,255));
	sprite->Flush();
	
	// restore
	sprite->SetTransform(&orgmat);
	pSurface->Release();
	pTex->Release();
#endif
}

VOID DXGame::EndSprite() {
	sprite->End();
}


//
BOOL DXTexture::LoadTexture(const TCHAR *path, IDirect3DDevice9* pd3dDevice)
{
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

	return TRUE;
}

RECT* DXTexture::GetRect() {
	return &txtRect;
}

LPDIRECT3DTEXTURE9 DXTexture::GetTexture() {
	return pTexture;
}

BOOL DXTexture::isTextureLoaded() {
	return (pTexture != 0);
}