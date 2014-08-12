/*
 * Uses DXUT
 */

#include "Stdafx.h"
#include "DXGame.h"
#include "DXFont.h"
#include "GameManager.h"
#include "GameSetting.h"
#include "SceneCommon.h"

/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
/*#pragma warning(disable:4074)//initializers put in compiler reserved initialization area
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

	// fadein/out
	dxGame.fadeAlpha = GameManager::getFadeAlpha();
	dxGame.FadeInOut();
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
	CSVOption::InitOption();
	GameManager::InitGame(&dxGame);
	GameManager::LoadSounds();

	GameManager::loadScene(GAMEMODE::SELECT);
	GameManager::startScene();

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
	GameManager::Release();
	GameManager::ReleaseSkinResource();
	GameManager::ReleaseSounds();
	dxGame.Release();

	return 0;
}