﻿#include "Stdafx.h"
#include "GameManager.h"
#include "GameSetting.h"
#include "GameResource.h"

int GameManager::GameMode;
DXGame *GameManager::dxGame;

// game resource
GameResource gameResource;

// sqlite
sqlite3 *sql;

// fmod
FMOD::System *fmod_system;
FMOD::Sound *sounds[256];
FMOD::Channel *channel = 0;

// resources (will be depreciated)
DXTexture images[256];
DXFont fonts[256];
CSVData csvData;
CSVData csvSoundData;

// common functions
std::string w2s(std::wstring w) {
	return std::string(w.begin(), w.end());
}

std::wstring getPath(std::wstring w) {
	TCHAR absolutePath[256];
	CSVFile::GetPathFromSettings(w.c_str(), absolutePath);
	return absolutePath;
}

void GameManager::InitGame(DXGame *dxGame_) {
	dxGame = dxGame_;

	// init FMOD Soundsystem
	FMOD::System_Create(&fmod_system);
	fmod_system->init(32, FMOD_INIT_NORMAL, 0);

	// open song db
	if (sqlite3_open(w2s(getPath(L"song.db")).c_str(), &sql) == SQLITE_OK) {
		InitSelectList();
	}
}


/*
 *************************************
 * Scene functions
 *************************************
 TODO: 1. disable input during fadeout/closing
 2. make simple class for fast function forwarding and make it as argument
 3. fadeout effect - if there's scene's limit time, then it shows fadeout.
   - if canceled fadeout, then limit time should (current+fadeout) time.
   - when fadeout time started, all input are invalid.
 */

void play_end() {
	// change scene
	// TODO: maybe opt set is necessary for result screen...
	GameManager::loadScene(GAMEMODE::RESULT);
}

void play_start() {
	CSVTimer::setTime(CSVTimerConst::PLAYSTART);
	CSVTimer::setTime(CSVTimerConst::RHYTHM_TIMER);

	// TODO callback 'end' function at the end of the timer ...
	int bmsTime = 0 + csvData.fadeOutTime;
	// TODO: get bms time from BMSData
	CSVTimer::setCallback(CSVTimerConst::PLAYSTART, bmsTime, play_end);
}

void load_end_callback() {
	// check if load end
	// if not, then this function should be signaled by GameResource class.
	gameResource.isSongLoaded = true;	// TEMP
	if (gameResource.isSongLoaded) {
		CSVTimer::setTime(CSVTimerConst::READY);
		CSVTimer::setCallback(CSVTimerConst::READY, csvData.playStartTime, play_start);
	}
}

void load_start() {
	// start load
	// TODO

	// and set callback timer
	CSVTimer::setCallback(CSVTimerConst::MAIN, 
		CSVTimer::getTime(CSVTimerConst::MAIN)+csvData.loadEndTime, load_end_callback);
}

void scene_play() {
	// first load scene
	GameManager::loadScene(GAMEMODE::PLAY);

	// and set load start time
	CSVTimer::setCallback(CSVTimerConst::MAIN, csvData.loadStartTime, load_start);
}

// works after FADEOUT time
void cancel_current_scene() {
}


/*
 ***************************************
 * class functions
 ***************************************
 */
void GameManager::LoadSounds() {
	// load sound xml file
	if (CSVReader::readCSVFile(L"LR2files\\Sound\\lr2.lr2ss", &csvSoundData)) {
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_select)).c_str(), FMOD_DEFAULT, 0, &sounds[0]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_decide)).c_str(), FMOD_DEFAULT, 0, &sounds[1]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_exselect)).c_str(), FMOD_DEFAULT, 0, &sounds[2]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_exdecide)).c_str(), FMOD_DEFAULT, 0, &sounds[3]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_folder_open)).c_str(), FMOD_DEFAULT, 0, &sounds[4]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_folder_close)).c_str(), FMOD_DEFAULT, 0, &sounds[5]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_panel_open)).c_str(), FMOD_DEFAULT, 0, &sounds[6]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_panel_close)).c_str(), FMOD_DEFAULT, 0, &sounds[7]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_option_change)).c_str(), FMOD_DEFAULT, 0, &sounds[8]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_difficulty)).c_str(), FMOD_DEFAULT, 0, &sounds[9]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_screenshot)).c_str(), FMOD_DEFAULT, 0, &sounds[10]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_clear)).c_str(), FMOD_DEFAULT, 0, &sounds[11]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_fail)).c_str(), FMOD_DEFAULT, 0, &sounds[12]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_stop)).c_str(), FMOD_DEFAULT, 0, &sounds[13]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_mine)).c_str(), FMOD_DEFAULT, 0, &sounds[14]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_scratch)).c_str(), FMOD_DEFAULT, 0, &sounds[15]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_courseclear)).c_str(), FMOD_DEFAULT, 0, &sounds[16]);
		fmod_system->createSound(w2s(getPath(csvSoundData.sound_coursefail)).c_str(), FMOD_DEFAULT, 0, &sounds[17]);

		sounds[0]->setMode(FMOD_LOOP_NORMAL);
	}
}

void GameManager::ReleaseSounds() {
	for (int i=0; i<18; i++)
		sounds[i]->release();
	fmod_system->close();
	fmod_system->release();
}

void GameManager::PlaySound(int num) {
	if (sounds[num]) {
		fmod_system->playSound(sounds[num], 0, false, &channel);
	}
}

void GameManager::StopSound(int num) {
	if (channel) {
		/*if (num < 0) {
			for (int i=0; i<18; i++)
				channel->stop();
		}*/
		channel->stop();
	}
}

int callback_addselectlist(void *data, int argc, char **argv, char **azColName){
	if ((int)data == 0) { // initalize
		int type = atoi(argv[1]);
		if (type == 6) {
			// course
			std::wstring wstr;
			AutoEncoder::convertEncodingAuto(argv[0], wstr);
			CSVSelectList::addCourseFolder(wstr);
		} else if (type == 2) {
			// folder
			std::wstring wstr;
			AutoEncoder::convertEncodingAuto(argv[0], wstr);
			CSVSelectList::addCustomFolder(wstr);
		} else if (type == 1) {
			// folder
			std::wstring wfolder, wpath;
			AutoEncoder::convertEncodingAuto(argv[0], wfolder);
			AutoEncoder::convertEncodingAuto(argv[2], wpath);
			CSVSelectList::addFolder(wpath, wfolder);
		}
	} else if ((int)data == 1) {// load song
		// 0 is hash

		CSVSongData songdata;
		AutoEncoder::convertEncodingAuto(argv[1], songdata.title);
		AutoEncoder::convertEncodingAuto(argv[2], songdata.subtitle);
		AutoEncoder::convertEncodingAuto(argv[3], songdata.genre);
		AutoEncoder::convertEncodingAuto(argv[4], songdata.artist);
		AutoEncoder::convertEncodingAuto(argv[6], songdata.path);

		char *p;
		songdata.folder = strtoul(argv[9], &p, 16);
		songdata.parent = strtoul(argv[14], &p, 16);
		songdata.level = atoi(argv[14]);
		songdata.difficulty = (CSVSongDataDifficulty)atoi(argv[15]);
		songdata.maxBPM = atoi(argv[16]);
		songdata.minBPM = atoi(argv[17]);
		songdata.key = atoi(argv[18]);
		songdata.judge = (CSVSongDataJudge::CSVSongDataJudge)atoi(argv[19]);
		songdata.longnote = atoi(argv[20]);
		songdata.readme = atoi(argv[25]);
		songdata.notecnt = atoi(argv[26]);
		
		CSVSongRecord songrecord;
		songrecord.rate = rand()%100 / 100.0;			// TODO
		songrecord.exscore = 100;
		songrecord.playCount = rand()%20;
		songrecord.clear = rand()%4;
		CSVSelectList::addSong(songdata, songrecord);	// TODO: clear
	}

	return 0;
}

void GameManager::InitSelectList() {
	// read from db
	std::string query = "SELECT title, type, path FROM folder WHERE parent is null;";
	sqlite3_exec(sql, query.c_str(), callback_addselectlist, 0, 0);
}

void GameManager::SelectList() {
	// get current one
	CSVSelectData* sel = CSVSelectList::getSelectedSongData();
	if (!sel)
		return;
	
	// case
	std::string query;
	switch (sel->type) {
	case CSVSelectType::FOLDER:
		// load folder data
		CSVSelectList::makeNewList();
		query = "SELECT * FROM song WHERE path like \'" + w2s(sel->songData.path) + "%\'";
		sqlite3_exec(sql, query.c_str(), callback_addselectlist, (void*)1, 0);
		CSVSelectList::makeSelectArray();
		CSVSelectList::Select(0);
		break;
	case CSVSelectType::CUSTOMFOLDER:
		break;
	case CSVSelectType::COURSEFOLDER:
		break;
	case CSVSelectType::SONG:
		// goto decide screen
		loadScene(GAMEMODE::DECIDE);

		// and set timer callback function for play screen...
		CSVTimer::setCallback(CSVTimerConst::MAIN, csvData.sceneTime, scene_play);
		
		break;
	}
}

bool GameManager::setGameMode(int mode) {
	GameMode = mode;
	switch (mode) {
		case GAMEMODE::SELECT:
			// set option
			CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_7KEY, 1);
			CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_7KEY, 1);
	
			CSVOption::setOption(CSVOptionConst::OFFLINE, 1);
			CSVOption::setOption(CSVOptionConst::ONLINE, 0);
			// for BG
			CSVOption::setOption(CSVOptionConst::EXTRAMODE_OFF, 1);
			CSVOption::setOption(CSVOptionConst::EXTRAMODE_ON, 0);
			CSVOption::setOption(CSVOptionConst::PANEL_NOTSTART, 1);	// DEFAULT
			CSVOption::setOption(CSVOptionConst::IR_RANK_LOADCOMPLETE, 1);
			CSVOption::setOption(CSVOptionConst::IR_RIVAL_SCORE_COMPARING_OFF, 1);
			CSVOption::setOption(CSVOptionConst::IR_RIVAL_SCORE_COMPARING_ON, 0);
			//CSVOption::setOption(CSVOptionConst::IR_DISCONNECTED, 1);
			//CSVOption::setOption(999, 1);	// that shouldn't be drawn - for debug

			CSVSelectList::makeSelectArray(7, CSVSongDataDifficulty::ALL);
			CSVSelectList::Select(0);
			
			// load CSV
			if (!CSVReader::readCSVFile(GameSetting::scene.select, &csvData)) {
				return false;
			}

			// play sound
			StopSound();
			PlaySound(SOUND::SELECT);
			break;
		case GAMEMODE::DECIDE:
			// load CSV
			if (!CSVReader::readCSVFile(GameSetting::scene.decide, &csvData)) {
				return false;
			}

			// play sound
			StopSound();
			PlaySound(SOUND::DECIDE);
			break;
		case GAMEMODE::PLAY:
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
			CSVOption::setOption(912, 1);	// guage number ...??

			// set number
			CSVNumber::setNumber(CSVNumConst::BPM, 120);	// set BPM
			CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GUAGE, 84);	// set Guage

			// set bargraph
			CSVBargraph::setVal(CSVBargraphConst::PLAY_EXSCORE_1P, 0.1);
			CSVBargraph::setVal(CSVBargraphConst::PLAY_HIGHSCORE_1P_RESULT, 0.7);
			CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_CURRENT, 0.2);
			CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_RESULT, 0.8);
			
			// shutter test
			CSVSlider::setSliderValue(CSVSliderConst::SHUTTER_1P, 0.3);

			// load CSV
			if (!CSVReader::readCSVFile(GameSetting::scene.play, &csvData)) {
				return false;
			}

			// play sound
			//PlaySound(SOUND::);
			break;
		case GAMEMODE::RESULT:
			// clear or fail?
			CSVOption::setOption(CSVOptionConst::RESULT_CLEAR, 1);
			CSVOption::setOption(CSVOptionConst::RESULT_FAIL, 0);
			
			// load CSV
			if (!CSVReader::readCSVFile(GameSetting::scene.result, &csvData)) {
				return false;
			}

			// play sound
			StopSound();
			PlaySound(SOUND::CLEAR);
			break;
	}

	return true;
}

void GameManager::startScene() {
	CSVTimer::invalidateTime();

	// set timer
	CSVTimer::setTime(CSVTimerConst::MAIN);

	switch (GameMode) {
		case GAMEMODE::PLAY:
			// set timer
			//CSVTimer::setTime(CSVTimerConst::READY);
			//CSVTimer::setTime(CSVTimerConst::PLAYSTART, 1000);
			break;
	}
}

bool GameManager::loadScene(int mode) {
	if (mode >= 0) {
		// clear data
		ReleaseSkinResource();

		// load CSV
		if (!GameManager::setGameMode(mode)) {
			return false;
		}

		LoadSkinResource(dxGame);
	}

	// game just begun!
	GameManager::startScene();
	//CSVRenderer::SetnotedrawFunc(0);	// <- need to implement

	// need to implement event
	return true;
}

void GameManager::LoadSkinResource(DXGame *dxGame) {
	for (int i=0; i<csvData.images.size(); i++) {
		// TODO: *.* name should be preprocessed
		TCHAR absolutePath[256];
		if (CSVFile::GetPathFromSettings(csvData.images[i].c_str(), absolutePath)) {
			if (!images[i].LoadTexture(absolutePath, dxGame->GetD3D9Device(), 
				D3DCOLOR_XRGB(csvData.colorKey.r, csvData.colorKey.g, csvData.colorKey.b))) {
				OutputDebugString(L"Failed to load some texture - ");
				OutputDebugString(absolutePath);
				OutputDebugString(L"\n");
			}
		}
	}

	// init fonts
	for (int i=0; i<csvData.csvFont.size(); i++) {
		fonts[i].fontData = csvData.csvFont[i];
		CSVFile::GetAbsolutePath(fonts[i].fontData->fontPath, fonts[i].fontData->fontPath);
		CSVFile::GetAbsolutePath(fonts[i].fontData->fontTexturePath, fonts[i].fontData->fontTexturePath);
		if (!fonts[i].InitDXFont(dxGame->GetD3D9Device())) {
			OutputDebugString(L"Failed to load some fonts. use fallback.\n");
			wcscpy(fonts[i].fontData->fontPath, L"C:\\Windows\\Fonts\\Gulim.ttc");
			fonts[i].fontData->borderR = fonts[i].fontData->borderG = fonts[i].fontData->borderB = 0;
			fonts[i].fontData->borderWidth = 1;
			fonts[i].fontData->fontR = fonts[i].fontData->fontG = fonts[i].fontData->fontB = 255;
			fonts[i].InitDXFont(dxGame->GetD3D9Device());
		}
	}
}

void GameManager::ReleaseSkinResource() {
	// clear resources
	for (int i=0; i<256; i++) {
		if (images[i].isTextureLoaded())
			images[i].Release();
	}

	for (int i=0; i<256; i++) {
		fonts[i].Release();
	}

	// clear csv data
	csvData.Clear();
}

void GameManager::Release() {
	// cut off db
	sqlite3_close(sql);
}

CSVData* GameManager::getCSVData() {
	return &csvData;
}

DXTexture* GameManager::getTexture(int num) {
	return &images[num];
}

DXFont* GameManager::getFont(int num) {
	return &fonts[num];
}