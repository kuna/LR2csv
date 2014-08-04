#include "GameManager.h"
#include "CSVOption.h"
#include "CSVTimer.h"
#include "CSVConst.h"
#include "CSVNumber.h"
#include "CSVBargraph.h"
#include "CSVSelectList.h"

int GameManager::GameMode;

void GameManager::startGame() {
	// must call first to initalize timer
	CSVTimer::invalidateTime();

	// set option

	// set timer
	CSVTimer::setTime(CSVTimerConst::MAIN);
}

void GameManager::setSelectMode() {
	GameMode = GAMEMODE::SELECT;

	// set option
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_FOLDER, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_PLAYABLE, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_READMETXT_EXIST, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_READMETXT_NOTEXIST, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_BPMCHANGE_EXIST, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_BPMCHANGE_NOTEXIST, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_7KEY, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_7KEY, 1);
	
	CSVOption::setOption(CSVOptionConst::OFFLINE, 1);
	CSVOption::setOption(CSVOptionConst::ONLINE, 0);

	// add some songs...
	CSVSelectList::addData(CSVSelectDataConst::SONG, L"test\\test.bms");
	CSVSelectList::addData(CSVSelectDataConst::SONG, L"test\\test2.bms");
	CSVSelectList::addData(CSVSelectDataConst::SONG, L"123\\test.bms");

	CSVSelectList::makeSelectArray(7, CSVSelectDataDifficulty::ALL);
	CSVSelectList::Select(0);
}

void GameManager::setPlayMode() {
	GameMode = GAMEMODE::PLAY;

	// set option
	CSVOption::setOption(CSVOptionConst::BGA_NORMAL, 1);
	CSVOption::setOption(CSVOptionConst::BGA_EXTEND, 0);
	CSVOption::setOption(CSVOptionConst::BGA_ON, 1);
	CSVOption::setOption(CSVOptionConst::AUTO_OFF, 1);
	CSVOption::setOption(CSVOptionConst::AUTO_ON, 0);
	CSVOption::setOption(CSVOptionConst::GHOST_TYPEB, 1);
	CSVOption::setOption(CSVOptionConst::SCOREGRAPH_ON, 1);
	
	CSVOption::setOption(CSVOptionConst::PLAYER1_NORMAL_GUAGE, 1);
	CSVOption::setOption(CSVOptionConst::PLAYER1_HARD_GUAGE, 0);

	// set timer
	CSVTimer::setTime(CSVTimerConst::READY);
	CSVTimer::setTime(CSVTimerConst::PLAYSTART, 1000);

	// set number
	CSVNumber::setNumber(CSVNumConst::BPM, 120);	// set BPM
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GUAGE, 84);	// set Guage

	// set bargraph
	CSVBargraph::setVal(CSVBargraphConst::PLAY_EXSCORE_1P, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_HIGHSCORE_1P_RESULT, 0.7);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_CURRENT, 0.2);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_RESULT, 0.8);
}