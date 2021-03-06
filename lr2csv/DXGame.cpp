﻿#include "Stdafx.h"
#include "DXGame.h"
#include "GameManager.h"

DXGame *dxGame;		// warning: static method?

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
        case WM_PAINT:
            //ValidateRect( hWnd, NULL );
			return 0;
		case WM_LBUTTONDOWN:
			dxGame->currentScene->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_LBUTTONUP:
			dxGame->currentScene->OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_RBUTTONDOWN:
			dxGame->currentScene->OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_RBUTTONUP:
			dxGame->currentScene->OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_MOUSEMOVE:
			dxGame->currentScene->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_MOUSEWHEEL:
			dxGame->currentScene->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
			return 0;
		/*case WM_KEYDOWN:
			dxGame->currentScene->OnKeyDown(wParam);
			return 0;
		case WM_KEYUP:
			dxGame->currentScene->OnKeyUp(wParam);
			return 0;*/
	}

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

HWND DXGame::MakeWindow(TCHAR *wndName, int width, int height) {
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
	dxGame = this;
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
	pp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;   //Default Presentation rate
	// VSync : D3DPRESENT_INTERVAL_ONE
	// no VSync : D3DPRESENT_INTERVAL_IMMEDIATE
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

	// create black screen
	CreateBlackScreen();

	return TRUE;
}

BOOL DXGame::InitalizeInput() {
	if (!dinput) {
		if (FAILED(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, 0)))
			return FALSE;

		// create keyboard
		dinput->CreateDevice(GUID_SysKeyboard, &dinputKeyboard, 0);

		dinputKeyboard->SetDataFormat(&c_dfDIKeyboard);
		dinputKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
		dinputKeyboard->Acquire();

		// create joystick TODO
		//dinput->CreateDevice(GUID_Joystick, &dinputKeyboard, 0);

		return TRUE;
	} else {
		return FALSE;
	}
}


/*
 * VOID Render()
 * called when DXGame::GameLoop called.
 * renders screen - need another thread to do rendering.
 *
 */

VOID DXGame::Render() {
	if (BeginScene()) {
		Clear(0, 0, 0, 0);

		// update time per each frame
		CSVTimer::invalidateTime();

		// call handler each time
		GameManager::Invalidate();

		// draw sprites
		BeginSprite();
		CSVRenderer::drawAll(GameManager::getCSVData());

		// fadein/out
		fadeAlpha = GameManager::getFadeAlpha();
		FadeInOut();
		EndSprite();

		EndScene();
	}
}

VOID DXGame::GameLoop() {
	// execute render thread
	//renderThreadAlive = true;
	//renderThread = boost::thread(&DXGame::Render, this);
	//renderThread.join(); [do not need to blocking]

	// execute main window
    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );

		// update input
		updateDirectInput();

		// call render thread
		Render();
    }

	// finish render thread
	//renderThreadAlive = false;
}

/*
 * checkDirectInput() - refreshes keyboardState[] variable.
 * access it using checkKeyboard(int)
 * should be called on every event handling.
 * 
 * ex:
 * updateDirectInput()
 * if (checkKeyboard(DIK_ESCAPE)) {
 *     ASSERT(_T("PRESSED!"));
 * }
 */
void DXGame::updateDirectInput() {
	// get previous device state first
	char currStat[256];
	memset(currStat, 0, sizeof(currStat));
	if (FAILED(dinputKeyboard->GetDeviceState(sizeof (currStat), (void*)&currStat))) {
		dinputKeyboard->Acquire();
		return;
	}

	// compare with previous state, and if changed, then send input event
	// and copy state
	for (int i=0; i<256; i++) {
		/*
		 * depreciated method
		 *
		 *
		if (inputhandler && (keyboardState[i] != currStat[i])) {
			if (currStat[i]) {
				dxGame->currentScene->OnKeyDown(wParam);
			} else {
				dxGame->currentScene->OnKeyDown(wParam);
			}
			//inputhandler(i, currStat[i]);
		}*/
		currStat[i] /= 128;

		if (keyboardState[i] != currStat[i]) {
			if (currStat[i]) {
				dxGame->currentScene->OnKeyDown(i);
			} else {
				dxGame->currentScene->OnKeyUp(i);
			}
		}
		keyboardState[i] = currStat[i];
	}
}

BOOL DXGame::checkKeyboard(int val) {
	return keyboardState[val]/128;
}

/*
 * depreciated method
 *
 */
void DXGame::setInputeventHandler(void (*h)(int, int)) {
	inputhandler = h;
}

VOID DXGame::ChangeMode(BOOL fullscreen) {
	pp.Windowed = !fullscreen;
	pd3dDevice->Reset(&pp);
}

BOOL DXGame::CreateSprite(LPD3DXSPRITE *sprite) {
	D3DXCreateSprite(pd3dDevice, sprite);
	return TRUE;
}

BOOL DXGame::CreateBlackScreen() {
	if (FAILED(D3DXCreateTexture(pd3dDevice, screenWidth, screenHeight, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pBlackTexture))) {
		return FALSE;
	}

	// fill tex
	D3DLOCKED_RECT lRect;
	pBlackTexture->LockRect(0, &lRect, 0, 0);
	D3DCOLOR* data = (D3DCOLOR*)lRect.pBits;
	for (int i=0; i<screenWidth*screenHeight; i++) {
		data[i] = D3DCOLOR_XRGB(0,0,0);
	}
	pBlackTexture->UnlockRect(0);

	return TRUE;
}

VOID DXGame::SetRenderTarget(LPDIRECT3DTEXTURE9 pTexture) {
	if (!newSurface) {
		if (pTexture->GetSurfaceLevel(0, &newSurface) == S_OK) {
			pd3dDevice->GetRenderTarget(0, &orgSurface);
			pd3dDevice->SetRenderTarget(0, newSurface);
		}
	}
}

VOID DXGame::ResetRenderTarget() {
	if (orgSurface) {
		pd3dDevice->GetRenderTarget(0, &newSurface);
		pd3dDevice->SetRenderTarget(0, orgSurface);
		orgSurface = 0;
		newSurface->Release();
		newSurface = 0;
	}
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
	// check black fadeout
	if (pBlackTexture) {
		pBlackTexture->Release();
		pBlackTexture = 0;
	}

	// input release
	if (dinput)
		dinput->Release();
	
	pd3dDevice->Release();
	pd3d9->Release();

	return TRUE;
}

VOID DXGame::Clear(int a, int r, int g, int b) {
	if (pd3dDevice)
		pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(a,r,g,b), 1.0f, 0);
}

BOOL DXGame::BeginScene() {
	if (pd3dDevice)
		return SUCCEEDED(pd3dDevice->BeginScene());
	else
		return FALSE;
}

VOID DXGame::EndScene() {
	if (pd3dDevice) {
		pd3dDevice->EndScene();
		pd3dDevice->Present(0, 0, 0, 0);

		// for FPS measuring
		TickFPS();
	}
}

VOID DXGame::FadeInOut() {
	if (fadeAlpha) {
		// MUST use when drawing scene
		// ALPHA BLENDING
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		sprite->Draw(pBlackTexture, 0, 0, 0, D3DCOLOR_ARGB((int)(fadeAlpha*255), 255, 255, 255));
		//pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(VERT));
		//pd3dDevice->drawprim
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

VOID DXGame::DrawString(DXFont *font, const TCHAR *str, int x, int y, int width, int size, int align, D3DCOLOR clr) {
	// TODO: align/stretching/resize (matrix) - comparing size with original one
	//

	// TODO: string under 100
	int chrWidth[100];
	int totalWidth = 0;
	for (int i=0; i<wcslen(str); i++) {
		if (!font->getFontTexture(str[i], &chrWidth[i]))
			chrWidth[i] = 0;
		totalWidth += chrWidth[i];
	}

	double multiply = (double)size / font->fontData->fontHeight;	// real multiplication (height)
	double widthMul = (double)width / totalWidth;
	if (widthMul > 1)
		widthMul = 1;
	double totalWidthMul = widthMul * multiply;	// real multiplication (width)
	int currentWidth = width;		// real drawing width
	if (totalWidth < width)
		currentWidth = totalWidth;

	// reset position for align
	int sx = x;
	if (align == 0) {
	} else if (align == 1) {
		sx -= totalWidth/2;
	} else if (align == 2) {
		sx -= totalWidth;
	}

	// ALPHA BLENDING
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	D3DXMATRIX mat, orgmat;
	sprite->GetTransform(&orgmat);

	for (int i=0; i<wcslen(str); i++) {
		DXFontTexture *dxfnt;
		dxfnt = font->getFontTexture(str[i]);
		if (dxfnt) {
			D3DXMatrixTransformation2D(&mat, &D3DXVECTOR2(x, y), 0.0,
				&D3DXVECTOR2(totalWidthMul, multiply), &D3DXVECTOR2(0, 0), 0,
				&D3DXVECTOR2(sx*totalWidthMul, y*multiply));
			sprite->SetTransform(&mat);

			RECT r;
			SetRect(&r, dxfnt->x, dxfnt->y, dxfnt->width+dxfnt->x, dxfnt->height+dxfnt->y);
			sprite->Draw(font->glyphTexture, &r, 0, 0, clr);
		}

		sx += chrWidth[i];
	}
	sprite->Flush();
	
	// restore
	sprite->SetTransform(&orgmat);
}

VOID DXGame::EndSprite() {
	sprite->End();
}

