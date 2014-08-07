/*
 * Uses DXUT
 */

#include "DXGame.h"
#include "DXFont.h"
#include "CSVFont.h"
#include "CSVData.h"
#include "CSVReader.h"
#include "CSVTimer.h"
#include "CSVConst.h"
#include "CSVOption.h"
#include "CSVSelectList.h"
#include "CSVFile.h"
#include "CSVRenderer.h"
#include "CSVButton.h"
#include "CSVSlider.h"
#include "GameManager.h"
#include "GameSetting.h"
#include "SceneCommon.h"

/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#pragma warning(disable:4074)//initializers put in compiler reserved initialization area
#pragma init_seg(compiler)//global objects in this file get constructed very early on
struct CrtBreakAllocSetter {
    CrtBreakAllocSetter() {
		_CrtDumpMemoryLeaks();
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
		_CrtSetBreakAlloc(700);
		_CrtMemDumpAllObjectsSince(0);
    }
};
CrtBreakAllocSetter g_crtBreakAllocSetter;
*/
// instead using WinMain...
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

// driver & main...
HWND hWnd;
DXGame dxGame;

// key input
SceneCommon sceneCommon;

bool drawFunc(int imgnum, TCHAR *text, CSVSRC *src, CSVDST *dst) {
	// check is argument font
	if (text == 0) {
		if (!GameManager::getTexture(imgnum)->isTextureLoaded())
			return false;
	
		// make color, rect, rotation centre
		D3DXCOLOR rgba = D3DXCOLOR(dst->getR()/256.0f,
			dst->getG()/256.0f, dst->getB()/256.0f, dst->getA()/256.0f);
		RECT src_rect, dst_rect;
		int wid = src->getWidth();
		if (wid < 0)
			wid = GameManager::getTexture(imgnum)->width;
		int hei = src->getHeight();
		if (hei < 0)
			hei = GameManager::getTexture(imgnum)->height;
		SetRect(&src_rect, src->getX(), src->getY(), src->getX()+wid, src->getY()+hei);
		SetRect(&dst_rect, dst->getX(), dst->getY(), dst->getX2(), dst->getY2());
		D3DXVECTOR2 rotateCentre = D3DXVECTOR2(dst->getCenterX(), dst->getCenterY());
		double rotation = dst->getAngle()/360.0f*2*3.14f;

		// draw!
		dxGame.DrawTexture(GameManager::getTexture(imgnum), &src_rect, &dst_rect, rgba, &rotateCentre, rotation, dst->getBlend(), dst->getFilter());
	} else {
		D3DXCOLOR rgba = D3DXCOLOR(dst->getR()/256.0f,
			dst->getG()/256.0f, dst->getB()/256.0f, dst->getA()/256.0f);

		// src is only for align
		//dxGame.DrawString(&fonts[ src->getFontNum() ], text, 
		//	dst->getX(), dst->getY(), dst->getWidth(), dst->getHeight(),
		//	src->getTextAlign(), rgba);
	}
}

void notedrawFunc() {
	/*
	CSVRenderer::drawLine(csvData.csvLine, 80);
	CSVRenderer::drawNote(csvData.csvNote, 0, 100);
	CSVRenderer::drawNote(csvData.csvNote, 2, 100);
	CSVRenderer::drawNote(csvData.csvNote, 3, 100);
	CSVRenderer::drawNote(csvData.csvNote, 4, 100);
	CSVRenderer::drawLNNote(csvData.csvNote, csvData.csvLNStart,
		csvData.csvLNBody, csvData.csvLNEnd, 5, 60, 120);
		*/
}

VOID Render() {
	// update time per each frame
	CSVTimer::invalidateTime();

	// start scene
	dxGame.Clear(0, 0, 0, 0);
	dxGame.BeginScene();

	// http://irrlicht.sourceforge.net/docu/example006.html
	// draw sprites
	dxGame.BeginSprite();
	CSVRenderer::drawAll(GameManager::getCSVData());

	//dxGame.DrawString(&fonts[0], L"한글 ヒラギ 다 됩니다!!!", &r, D3DCOLOR_XRGB(255, 255, 255), 1);
	
	dxGame.EndSprite();

	/*TCHAR msg[30];
	swprintf(msg, 30, L"FPS %d / MAIN %d", driver->getFPS(), CSVTimer::getTime(CSVTimerConst::PLAYSTART));
	font->draw(msg,
                core::rect<s32>(10,30,300,50),
                video::SColor(255,255,255,255));*/

	dxGame.EndScene();
}


INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	// load game setting
	GameSetting::LoadSetting();

	// create window
	hWnd = dxGame.MakeWindow(L"LR2csv Alpha 140728", 
		GameSetting::screen.width, GameSetting::screen.height);

	// init device
	if (!dxGame.Initalize(hWnd)) {
		MSGBOX_ERROR(hWnd, L"Failed to initalize DirectX!");
		return FALSE;
	}
	
	// show Window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );

	// init
	CSVRenderer::SetdrawFunc(drawFunc);
	GameManager::InitGame(&dxGame);
	CSVOption::InitOption();

	GameManager::loadScene(GAMEMODE::SELECT);
	GameManager::startScene();

	// set key input
	dxGame.currentScene = (Scene*)&sceneCommon;

    // Enter the message loop
	// TODO: include it in DXGame ...?
    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
		
        Render();
    }

	// clear program...?
	CSVSelectList::clearData();
	GameManager::ReleaseSkinResource();
	dxGame.Release();

	return 0;
}