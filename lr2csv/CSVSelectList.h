#include <vector>
#include <tchar.h>
#include <Windows.h>
#pragma once

// ALL / BEGINNER / HYPER... how to process it?
// index should be stayed...

enum CSVSelectDataConst {
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

enum CSVSelectDataDifficulty {
	BEGINNER = 0,
	NORMAL = 1,
	HYPER = 2,
	ANOTHER = 3,
	INSANE = 4,
	OTHER = 5,
	ALL = -1
};

namespace CSVSelectDataJudge {
	enum CSVSelectDataJudge {
		EASY = 0,
		NORMAL = 1,
		HARD = 2,
		VERYHARD = 3
	};
};

class CSVSelectData;	// redeclaration

class CSVSelectSong {
public:
	CSVSelectData *parent;

	int level;
	int key;
	TCHAR path[256];
	int noteCnt;
	int maxBPM;
	int minBPM;
	int difficulty;
	int judge;
};

class CSVSelectData {
public:
	CSVSelectDataConst index;			// course or what?
	TCHAR dir[256];						// identifier - song directory or something...
	TCHAR readme[256];					// readme file exists?
	std::vector<CSVSelectSong*> songs;	// how many songs exists in this fldr (vector?)
};

class CSVSelectList {
private:
	static void GetDirectory(TCHAR *path);

	static int moveDirection;
	static int makeCorrectIndex(int idx);
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
	static CSVSelectSong* getSongData(int idx);
	static CSVSelectSong* getSelectedSongData();

	// for storing
	static std::vector<CSVSelectData*> data;
	static std::vector<CSVSelectSong*> songlist;

	static void addData(CSVSelectDataConst index, TCHAR *path);
	static void clearData();
	static void makeSelectArray(int key, CSVSelectDataDifficulty difficulty); 
	static CSVSelectData* getSelectDataFromSong(CSVSelectSong *song);
	static void Select(int songlistIdx);

	static void checkSlider();

	// TODO: just changing difficulty or key ... new method is necessary
	// -> implement in makeSelectArray ?
	// TODO: make new method of calling next difficulty (same -> difficulty)
};


