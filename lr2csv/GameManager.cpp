#include "GameManager.h"
#include "CSVOption.h"
#include "CSVTimer.h"
#include "CSVConst.h"
#include "CSVNumber.h"
#include "CSVBargraph.h"
#include "CSVSlider.h"
#include "CSVSelectList.h"
#include "CSVReader.h"
#include "CSVFile.h"
#include "GameSetting.h"

// for using FMOD
#pragma comment(lib, "fmod_vc.lib")
#include <fmod.hpp>

int GameManager::GameMode;
DXGame *GameManager::dxGame;

// fmod
FMOD::System *fmod_system;
FMOD::Sound *sounds[256];
FMOD::Channel *channel = 0;

// resources
DXTexture images[256];
DXFont fonts[256];
CSVData csvData;
CSVData csvSoundData;

void GameManager::InitGame(DXGame *dxGame_) {
	dxGame = dxGame_;

	// init FMOD Soundsystem
	FMOD::System_Create(&fmod_system);
	fmod_system->init(32, FMOD_INIT_NORMAL, 0);
}

std::string w2s(std::wstring w) {
	return std::string(w.begin(), w.end());
}

std::wstring getPath(std::wstring w) {
	TCHAR absolutePath[256];
	CSVFile::GetPathFromSettings(w.c_str(), absolutePath);
	return absolutePath;
}

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

bool GameManager::setGameMode(int mode) {
	GameMode = mode;
	switch (mode) {
		case GAMEMODE::SELECT:
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
			// for BG
			CSVOption::setOption(CSVOptionConst::EXTRAMODE_OFF, 1);
			CSVOption::setOption(CSVOptionConst::EXTRAMODE_ON, 0);
			CSVOption::setOption(CSVOptionConst::PANEL_NOTSTART, 1);	// DEFAULT
			CSVOption::setOption(CSVOptionConst::IR_RANK_LOADCOMPLETE, 1);
			//CSVOption::setOption(CSVOptionConst::IR_DISCONNECTED, 1);
			//CSVOption::setOption(999, 1);	// that shouldn't be drawn - for debug

			// add some songs...
			CSVSelectList::clearData();
			CSVSelectList::addData(CSVSelectDataConst::SONG, L"test\\test.bms");
			CSVSelectList::addData(CSVSelectDataConst::SONG, L"test\\test2.bms");
			CSVSelectList::addData(CSVSelectDataConst::SONG, L"123\\test.bms");

			CSVSelectList::makeSelectArray(7, CSVSelectDataDifficulty::ALL);
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
			CSVTimer::setTime(CSVTimerConst::READY);
			CSVTimer::setTime(CSVTimerConst::PLAYSTART, 1000);
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
}

void GameManager::LoadSkinResource(DXGame *dxGame) {
	for (int i=0; i<csvData.images.size(); i++) {
		// TODO: *.* name should be preprocessed
		TCHAR absolutePath[256];
		if (CSVFile::GetPathFromSettings(csvData.images[i].c_str(), absolutePath)) {
			if (!images[i].LoadTexture(absolutePath, dxGame->GetD3D9Device())) {
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

CSVData* GameManager::getCSVData() {
	return &csvData;
}

DXTexture* GameManager::getTexture(int num) {
	return &images[num];
}

DXFont* GameManager::getFont(int num) {
	return &fonts[num];
}