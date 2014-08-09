#include <vector>
#include <tchar.h>
#include <Windows.h>
#pragma once

// ALL / BEGINNER / HYPER... how to process it?
// index should be stayed...

enum CSVSelectType {
	SONG = 0,
	FOLDER = 1,
	CUSTOMFOLDER = 2,
	NEWFOLDER = 3,
	RIVALFOLDER = 4,
	SONGRIVAL = 5,
	COURSEFOLDER = 6,
	COURSEMAKE = 7,
	COURSE = 8,
	COURSERANDOM = 9
};

enum CSVSongDataDifficulty {
	BEGINNER = 1,
	NORMAL = 2,
	HYPER = 3,
	ANOTHER = 4,
	INSANE = 5,
	OTHER = 6,
	ALL = 0
};

namespace CSVSongDataJudge {
	enum CSVSongDataJudge {
		EASY = 3,
		NORMAL = 2,
		HARD = 1,
		VERYHARD = 0
	};
};

struct CSVSongData {
	std::wstring path;
	std::wstring title;
	std::wstring subtitle;
	std::wstring artist;
	std::wstring subartist;
	std::wstring genre;
	int parent;
	int folder;
	int level;
	CSVSongDataDifficulty difficulty;
	CSVSongDataJudge::CSVSongDataJudge judge;
	int key;
	int notecnt;
	int maxBPM;
	int minBPM;
	bool readme;
	bool longnote;

	int playCount;
	int exscore;
	double rate;
};

struct CSVSelectData {
	CSVSongData songData;
	int clear;
	int rank;			// 0: not rank mode / 1~...: rank
	int rival;			// 0: not rival / 1: win / 2: lose
	CSVSelectType type;	// refer CSVSelectType
};

struct CSVSelectHistory {
	std::vector<CSVSelectData*> data;
	int pos;
};

class CSVSelectList {
private:
	static void GetDirectory(TCHAR *path);

	static int moveDirection;
	static int makeCorrectIndex(int idx);
	static int getGroupInt(TCHAR *str);

	static CSVSongDataDifficulty currentDiff;
	static int currentKey;
public:
	// for drawing/indexing
	static double moveProgress;		// default: 1
	static DWORD moveStartTime;
	static int currentIndex;		// real selection
	static int currentSkinIndex;	// skin selection
	static int moveDuration;		// default 400ms?

	static void MoveUp();
	static void MoveDown();
	static double CalculateMoving();
	static CSVSelectData* getSongData(int idx);
	static CSVSelectData* getSelectedSongData();

	static std::vector<CSVSelectData*> data;		// cached list (storing)
	static std::vector<CSVSelectData*> songlist;	// current list
	
	static void addCourse(std::wstring coursetitle, int clear);
	static void addCourseFolder(std::wstring foldername);
	static void addNewFolder();
	static void addRivalFolder(std::wstring rival);
	static void addFolder(std::wstring path, std::wstring foldername);					// add folder.
	static void addCustomFolder(std::wstring foldername);
	static void addSong(CSVSongData songdata, int clear);
	static void addRank(CSVSongData songdata, int clear, int rank);
	static void addRival(CSVSongData songdata, int clear, int winlose);

	static void clearData(bool release=true);									// clear current data (release memory?)
	static void clearAllData();													// clear all data(memory free) including history
	static void makeSelectArray(int key, CSVSongDataDifficulty difficulty);
	static void makeSelectArray();
	static void Select(int songlistIdx);	// just focus

	static void checkSlider();

	static void SelectCurrent();			// enter focused one
	static void goPrevList();				// go to previous folder
	static void makeNewList();				// make new folder

	// TODO: just changing difficulty or key ... new method is necessary
	// -> implement in makeSelectArray ?
	// TODO: make new method of calling next difficulty (same -> difficulty)
};


