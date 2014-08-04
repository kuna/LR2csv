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

// instead using WinMain...
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

// driver & main...
HWND hWnd;
DXGame dxGame;

// skin
DXTexture images[256];
DXFont fonts[256];
CSVData csvData;

bool drawFunc(int imgnum, TCHAR *text, CSVSRC *src, CSVDST *dst) {
	// check is argument font
	if (text == 0) {
		if (!images[imgnum].isTextureLoaded())
			return false;
	
		// make color, rect, rotation centre
		D3DXCOLOR rgba = D3DXCOLOR(dst->getR()/256.0f,
			dst->getG()/256.0f, dst->getB()/256.0f, dst->getA()/256.0f);
		RECT src_rect, dst_rect;
		int wid = src->getWidth();
		if (wid < 0)
			wid = images[imgnum].width;
		int hei = src->getHeight();
		if (hei < 0)
			hei = images[imgnum].height;
		SetRect(&src_rect, src->getX(), src->getY(), src->getX()+wid, src->getY()+hei);
		SetRect(&dst_rect, dst->getX(), dst->getY(), dst->getX2(), dst->getY2());
		D3DXVECTOR2 rotateCentre = D3DXVECTOR2(dst->getCenterX(), dst->getCenterY());
		double rotation = dst->getAngle()/360.0f*2*3.14f;

		// draw!
		dxGame.DrawTexture(&images[imgnum], &src_rect, &dst_rect, rgba, &rotateCentre, rotation, dst->getBlend(), dst->getFilter());
	} else {
		D3DXCOLOR rgba = D3DXCOLOR(dst->getR()/256.0f,
			dst->getG()/256.0f, dst->getB()/256.0f, dst->getA()/256.0f);
		RECT src_rect, dst_rect;
		SetRect(&src_rect, src->getX(), src->getY(), src->getX2(), src->getY2());
		if (src->getTextAlign() == 1) {
			SetRect(&dst_rect, dst->getX()-dst->getWidth()/2, dst->getY(),
				dst->getX()+dst->getWidth()/2, dst->getY2());
		} else if (src->getTextAlign() == 2) {
			SetRect(&dst_rect, dst->getX()-dst->getWidth(), dst->getY(),
				dst->getX(), dst->getY2());
		} else if (src->getTextAlign() == 0) {
			SetRect(&dst_rect, dst->getX(), dst->getY(),
				dst->getX2(), dst->getY2());
		}

		// TODO: src_rect is useless ... only for align
		dxGame.DrawString(&fonts[ src->getFontNum() ], text, &dst_rect, rgba, src->getTextAlign());
	}
}

void notedrawFunc() {
	CSVRenderer::drawLine(csvData.csvLine, 80);
	CSVRenderer::drawNote(csvData.csvNote, 0, 100);
	CSVRenderer::drawNote(csvData.csvNote, 2, 100);
	CSVRenderer::drawNote(csvData.csvNote, 3, 100);
	CSVRenderer::drawNote(csvData.csvNote, 4, 100);
	CSVRenderer::drawLNNote(csvData.csvNote, csvData.csvLNStart,
		csvData.csvLNBody, csvData.csvLNEnd, 5, 60, 120);
}

VOID InitCSV() {
	// clear data
	csvData.Clear();

	// MUST CALL BEFORE FIRST CSV LOAD
	CSVOption::InitOption();
	//CSVOption::setOption(906, 0);	// select Layout 1
	//CSVOption::setOption(905, 1);	// select Layout 2
	CSVOption::setOption(CSVOptionConst::PANEL_NOTSTART, 1);	// DEFAULT
	//CSVOption::setOption(965, 1);	// for tricoro skin
	CSVOption::setOption(CSVOptionConst::PLAYER1_NORMAL_GUAGE, 1);
	CSVOption::setOption(CSVOptionConst::PLAYER1_HARD_GUAGE, 0);
	
	// for BG
	CSVOption::setOption(CSVOptionConst::EXTRAMODE_OFF, 1);
	CSVOption::setOption(CSVOptionConst::EXTRAMODE_ON, 0);
	
	// IR
	CSVOption::setOption(CSVOptionConst::IR_RANK_LOADCOMPLETE, 1);
	//CSVOption::setOption(CSVOptionConst::IR_DISCONNECTED, 1);

	//CSVOption::setOption(999, 1);	// that shouldn't be drawn - for debug
	
	// play
	CSVOption::setOption(912, 1);	// guage number ...??

	// TODO: load csv, image & print image & character to background.
	TCHAR csvPath[256];
	//wcscpy(csvPath, L"LR2Files\\Theme\\REMI-S\\Play\\7key_Left.csv");
	//wcscpy(csvPath, L"LR2Files\\Theme\\LR2\\Select\\select.csv");
	//wcscpy(csvPath, L"LR2Files\\Theme\\REMI-S\\Play\\7key.lr2skin");
	//wcscpy(csvPath, L"LR2Files\\Theme\\LR2\\Select\\select.lr2skin");
	wcscpy(csvPath, L"LR2Files\\Theme\\20th tricoro for HD\\Select\\select.lr2skin");
	//wcscpy(csvPath, L"LR2Files\\Theme\\20th tricoro for HD\\play\\play_7.lr2skin");
	
	if (!CSVReader::readCSVFile(csvPath, &csvData)) {
		MSGBOX_ERROR(hWnd, L"Failed to load csv file!");
	}

	for (int i=0; i<csvData.images.size(); i++) {
		// TODO: *.* name should be preprocessed
		TCHAR absolutePath[256];
		if (CSVFile::GetPathFromSettings(csvData.images[i].c_str(), absolutePath)) {
			if (!images[i].LoadTexture(absolutePath, dxGame.GetD3D9Device())) {
				//MSGBOX_ERROR(hWnd, L"failed to load some texture");
				OutputDebugString(L"Failed to load some texture - ");
				OutputDebugString(absolutePath);
				OutputDebugString(L"\n");
			}
		}
	}

	// init fonts
	for (int i=0; i<csvData.csvFont.size(); i++) {
		dxGame.InitalizeFont(&fonts[i], csvData.csvFont[i]);
		fonts[i].InitDXFont();
	}

	// test
	//fonts[0].drawChar(0, L',');
	//fonts[0].drawChar(0, L'A');

	/*// TODO testfont...
	wcscpy(csvFont[0].fontFace, L"Malgun Gothic");
	csvFont[0].fontHeight = 20;
	dxGame.InitalizeFont(&csvFont[0]);*/

	// game just begun!
	GameManager::startGame();

	// select scene you want
	GameManager::setPlayMode();
	GameManager::setSelectMode();
	//CSVRenderer::SetnotedrawFunc(notedrawFunc);
	CSVRenderer::SetnotedrawFunc(0);

	// shutter test
	CSVSlider::setSliderValue(CSVSliderConst::SHUTTER_1P, 0.3);
}

VOID Render() {
	// update time per each frame
	CSVTimer::invalidateTime();

	// start scene
	dxGame.Clear(0, 0, 0, 0);
	dxGame.BeginScene();

	// http://irrlicht.sourceforge.net/docu/example006.html
	// test
	dxGame.BeginSprite();
	csvData.drawAll(drawFunc);

	// test: FONT draw
//	dxGame.sprite->Draw(fonts[0].getFontTexture(dxGame.GetD3D9Device(), L'T')->texture,
//		0, 0, 0, 0xFFFFFFFF);

	RECT r;
	SetRect(&r, 50, 50, 200, 200);
	dxGame.DrawString(&fonts[0], L"한글 ヒラギ 다 됩니다!!!", &r, D3DCOLOR_XRGB(255, 255, 255), 1);
	
	dxGame.EndSprite();

	/*TCHAR msg[30];
	swprintf(msg, 30, L"FPS %d / MAIN %d", driver->getFPS(), CSVTimer::getTime(CSVTimerConst::PLAYSTART));
	font->draw(msg,
                core::rect<s32>(10,30,300,50),
                video::SColor(255,255,255,255));*/


	dxGame.EndScene();
}

// TODO: include it to DXGame
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
		case WM_KEYDOWN:
			if (wParam == VK_F5) {
				// refresh
				InitCSV();
			}

			if (wParam == 'Z') {
				// timer
				CSVTimer::deActiviateTimer(CSVTimerConst::KEYOFF_TIMER);
				CSVTimer::setTime(CSVTimerConst::KEYDOWN_TIMER);
			} else if (wParam == VK_UP) {
				CSVSelectList::MoveUp();
			} else if (wParam == VK_DOWN) {
				CSVSelectList::MoveDown();
			}
			return 0;
		case WM_KEYUP:
			if (wParam == 'Z') {
				// key input test
				CSVTimer::deActiviateTimer(CSVTimerConst::KEYDOWN_TIMER);
				CSVTimer::setTime(CSVTimerConst::KEYOFF_TIMER);
				CSVTimer::setTime(CSVTimerConst::BOMB_TIMER);
			}
			if (wParam == 'Q') {
				// combo test
				CSVTimer::setTime(CSVTimerConst::JUDGE_1P);
			}
			if (wParam == 'W') {
				// fullcombo test
				CSVTimer::setTime(CSVTimerConst::FULLCOMBO_1P);
			}
			if (wParam == 'R') {
				// rhythm timer test
				CSVTimer::setTime(CSVTimerConst::RHYTHM_TIMER);
			}
			return 0;
		case WM_MOUSEWHEEL:
			// menu move or shutter(sudden+) move
			return 0;
		case WM_LBUTTONDOWN:
			// click event
			// get button of the position and call event
			{
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
				CSVButton::Click(x, y);
				CSVSlider::mouseDown(x, y);
				return 0;
			}
		case WM_LBUTTONUP:
			CSVSlider::mouseUp();
			return 0;
		case WM_RBUTTONDOWN:
			// cancel event
			// like shutter close, parent folder, cancel song.
			return 0;
		case WM_MOUSEMOVE:
			// move cursor
			{
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
				CSVRenderer::SetCurPos(x, y);
				if (CSVSlider::mouseMove(x, y) == CSVSliderConst::SELECT_SLIDER) {
					CSVSelectList::checkSlider();
				}
				return 0;
			}
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	// create window
	hWnd = dxGame.MakeWindow(MsgProc, L"LR2csv Alpha 140728", SCREENWIDTH, SCREENHEIGHT);

	// init device
	if (!dxGame.Initalize(hWnd)) {
		MSGBOX_ERROR(hWnd, L"Failed to initalize DirectX!");
		return FALSE;
	}
	
	// show Window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );

	// init
	InitCSV();

    // Enter the message loop
    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
		
        Render();
    }

	// clear program...?
	dxGame.Release();

	return 0;
}