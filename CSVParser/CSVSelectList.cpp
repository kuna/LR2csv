#include "Stdafx.h"
#include "CSVSelectList.h"
#include "CSVNumber.h"
#include "CSVOption.h"
#include "CSVTimer.h"
#include "CSVConst.h"
#include "CSVBargraph.h"
#include "CSVSlider.h"
#include "CSVText.h"

#define DEFAULTMOVEDURATION 300
#define MINMOVEDURATION 60

double CSVSelectList::moveProgress = 0;
DWORD CSVSelectList::moveStartTime;
int CSVSelectList::currentIndex;
int CSVSelectList::currentSkinIndex;
int CSVSelectList::moveDuration = DEFAULTMOVEDURATION;	// init
// TODO: using function calling frequency, automatically controll moveDuration ...?
int CSVSelectList::moveDirection;
int nextMoveDuration = DEFAULTMOVEDURATION;

std::vector<CSVSelectData*> CSVSelectList::data;
std::vector<CSVSelectData*> CSVSelectList::songlist;
std::vector<CSVSelectHistory*> listHistory;
CSVSongDataDifficulty CSVSelectList::currentDiff;
int CSVSelectList::currentKey;


int CSVSelectList::getGroupInt(TCHAR *str) {
	int l = wcslen(str);
	int r = 0;
	for (int i=0; i<l; i++)
		r += str[i];
	return r;
}

void CSVSelectList::addCourse(std::wstring coursetitle, int clear) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	d->songData.title = coursetitle;
	d->songRecord.clear = clear;
	d->rank = 0;
	d->rival = 0;
	d->type = CSVSelectType::COURSE;

	data.push_back(d);
}

void CSVSelectList::addCourseFolder(std::wstring foldername) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	d->songData.title = foldername;
	d->songRecord.clear = 0;
	d->rank = 0;
	d->rival = 0;
	d->type = CSVSelectType::COURSEFOLDER;

	data.push_back(d);
}

void CSVSelectList::addNewFolder() {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	d->songData.title = L"NEW FOLDER";
	d->type = CSVSelectType::NEWFOLDER;

	data.push_back(d);
}

void CSVSelectList::addRivalFolder(std::wstring rival) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	d->songData.title = rival;
	d->songRecord.clear = 0;
	d->rank = 0;
	d->rival = 0;
	d->type = CSVSelectType::RIVALFOLDER;

	data.push_back(d);
}

void CSVSelectList::addFolder(std::wstring path, std::wstring foldername) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));
	
	d->songData.title = foldername;
	d->songData.path = path;
	d->songRecord.clear = 0;
	d->rank = 0;
	d->rival = 0;
	d->type = CSVSelectType::FOLDER;

	data.push_back(d);
}

void CSVSelectList::addCustomFolder(std::wstring foldername) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));
	
	d->songData.title = foldername;
	d->songRecord.clear = 0;
	d->rank = 0;
	d->rival = 0;
	d->type = CSVSelectType::CUSTOMFOLDER;

	data.push_back(d);
}

void CSVSelectList::addSong(CSVSongData songdata, CSVSongRecord songrecord) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	// make group data
	d->songData = songdata;
	d->songRecord = songrecord;
	d->rank = 0;
	d->rival = 0;
	d->type = CSVSelectType::SONG;

	data.push_back(d);
}

void CSVSelectList::addRank(CSVSongData songdata, int clear, int rank) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	d->songData = songdata;
	d->songRecord.clear = clear;
	d->rank = rank;
	d->rival = 0;
	d->type = CSVSelectType::SONG;

	data.push_back(d);
}

void CSVSelectList::addRival(CSVSongData songdata, int clear, int winlose) {
	CSVSelectData *d = (CSVSelectData*)malloc(sizeof(CSVSelectData));
	memset(d, 0, sizeof(CSVSelectData));

	d->songData = songdata;
	d->songRecord.clear = clear;
	d->rank = 0;
	d->rival = winlose;
	d->type = CSVSelectType::SONGRIVAL;

	data.push_back(d);
}

void CSVSelectList::clearData(bool release) {
	// first delete all songlist to prevent drawing
	songlist.clear();

	// remove data (memory release)
	if (release) {
		for (int i=0; i<data.size(); i++) {
			delete data[i];
		}
	}
	data.clear();
}

void CSVSelectList::clearAllData() {
}

void CSVSelectList::makeSelectArray(int key, CSVSongDataDifficulty difficulty) {
	currentKey = key;
	currentDiff = difficulty;
	makeSelectArray();
}

void CSVSelectList::makeSelectArray() {
	// make songlist
	// to show in song list
	for (int i=0; i<data.size(); i++) {
		if (currentDiff == CSVSongDataDifficulty::ALL
			|| data[i]->songData.difficulty
			|| data[i]->songData.difficulty == currentDiff) {
			if (currentKey == 0 ||
				currentKey == data[i]->songData.key ||
				data[i]->songData.key == 0)
			songlist.push_back(data[i]);
		}
	}
}

void CSVSelectList::GetDirectory(TCHAR *path) {
	int idx = wcslen(path)-1;

	for (; idx>=0 && path[idx] != L'\\'; idx--) {
		path[idx] = 0;
	}
}

void CSVSelectList::Select(int songlistIdx) {
	// if there's no data ...?
	if (songlist.size() == 0)
		return;

	// wrong number will be converted automatically
	songlistIdx = makeCorrectIndex(songlistIdx);

	currentIndex = songlistIdx;
	CSVSelectData* current = songlist[songlistIdx];
	
	OutputDebugString(L"SONG CHANGED - ");
	OutputDebugString(current->songData.title.c_str());
	OutputDebugString(L"\n");

	// change slider
	CSVSlider::setSliderValue(CSVSliderConst::SELECT_SLIDER, (double)songlistIdx/songlist.size());

	// change timer
	CSVTimer::setTime(CSVTimerConst::SONGSELECT_CHANGE);

	// get all song difficulty (which in same folder)
	CSVSelectData* songs_diff[10];
	int songs_cnt[10];
	memset(&songs_diff, 0, sizeof(CSVSelectData*)*10);
	memset(&songs_cnt, 0, sizeof(int)*10);
	for (int i=0; i<data.size(); i++) {
		if (data[i]->type == CSVSelectType::SONG && data[i]->songData.folder == current->songData.folder) {
			songs_diff[data[i]->songData.difficulty] = data[i];
			songs_cnt[data[i]->songData.difficulty]++;
		}
	}
	if (current->type == CSVSelectType::SONG) {
		songs_diff[current->songData.difficulty] = current;
	}

	// set text
	CSVText::SetText(CSVTextConst::CURRENT_SONG_TITLE, current->songData.title);
	CSVText::SetText(CSVTextConst::CURRENT_SONG_SUBTITLE, current->songData.subtitle);
	CSVText::SetText(CSVTextConst::CURRENT_SONG_TITLE_SUBTITLE, current->songData.title + current->songData.subtitle);
	CSVText::SetText(CSVTextConst::CURRENT_SONG_ARTIST, current->songData.artist);
	CSVText::SetText(CSVTextConst::CURRENT_SONG_SUBARTIST, current->songData.subartist);
	CSVText::SetText(CSVTextConst::CURRENT_SONG_GENRE, current->songData.genre);
	CSVText::SetText(CSVTextConst::CURRENT_SONG_LEVEL, std::to_wstring((_ULONGLONG)current->songData.level));
	CSVText::SetText(CSVTextConst::CURRENT_SONG_DIFFICULTY, std::to_wstring((_ULONGLONG)current->songData.difficulty));


	// set number
	CSVNumber::setNumber(CSVNumConst::BMS_BPM, current->songData.minBPM);
	CSVNumber::setNumber(CSVNumConst::MINBPM, current->songData.minBPM);
	CSVNumber::setNumber(CSVNumConst::MAXBPM, current->songData.maxBPM);
	if (songs_diff[CSVSongDataDifficulty::BEGINNER] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_BEGINNER_LEVEL, songs_diff[CSVSongDataDifficulty::BEGINNER]->songData.level);
	if (songs_diff[CSVSongDataDifficulty::NORMAL] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_NORMAL_LEVEL, songs_diff[CSVSongDataDifficulty::NORMAL]->songData.level);
	if (songs_diff[CSVSongDataDifficulty::HYPER] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_HYPER_LEVEL, songs_diff[CSVSongDataDifficulty::HYPER]->songData.level);
	if (songs_diff[CSVSongDataDifficulty::ANOTHER] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_ANOTHER_LEVEL, songs_diff[CSVSongDataDifficulty::ANOTHER]->songData.level);
	if (songs_diff[CSVSongDataDifficulty::INSANE] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_INSANE_LEVEL, songs_diff[CSVSongDataDifficulty::INSANE]->songData.level);

	// set bargraph
#define LIMIT(x) ((x>1)?1:(x))
	if (songs_diff[CSVSongDataDifficulty::BEGINNER] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_BEGINNER_LEVEL, LIMIT(songs_diff[CSVSongDataDifficulty::BEGINNER]->songData.level/12.0));
	if (songs_diff[CSVSongDataDifficulty::NORMAL] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_NORMAL_LEVEL, LIMIT(songs_diff[CSVSongDataDifficulty::NORMAL]->songData.level/12.0));
	if (songs_diff[CSVSongDataDifficulty::HYPER] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_HYPER_LEVEL, LIMIT(songs_diff[CSVSongDataDifficulty::HYPER]->songData.level/12.0));
	if (songs_diff[CSVSongDataDifficulty::ANOTHER] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_ANOTHER_LEVEL, LIMIT(songs_diff[CSVSongDataDifficulty::ANOTHER]->songData.level/12.0));
	if (songs_diff[CSVSongDataDifficulty::INSANE] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_INSANE_LEVEL, LIMIT(songs_diff[CSVSongDataDifficulty::INSANE]->songData.level/12.0));
#undef LIMIT
	// TODO
	CSVBargraph::setVal(CSVBargraphConst::SELECT_PGREAT_PER_TOTALNOTE, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_GREAT_PER_TOTALNOTE, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_GOOD_PER_TOTALNOTE, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_BAD_PER_TOTALNOTE, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_POOR_PER_TOTALNOTE, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_MAXCOMBO_PER_TOTALNOTE, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_SCORE_PER_TOTAL, 0.1);
	CSVBargraph::setVal(CSVBargraphConst::SELECT_EXSCORE_PER_TOTAL, current->songRecord.rate);

	// change OPs
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_SONG, (current->type == CSVSelectType::SONG));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_COURSE, (current->type == CSVSelectType::COURSE));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_FOLDER, (current->type == CSVSelectType::FOLDER || current->type == CSVSelectType::COURSEFOLDER));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_NEWCOURSE, (current->type == CSVSelectType::COURSEMAKE));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_PLAYABLE, (current->type == CSVSelectType::SONG));	// TODO

	CSVOption::setOption(CSVOptionConst::SELECT_READMETXT_EXIST, current->songData.readme);
	CSVOption::setOption(CSVOptionConst::SELECT_READMETXT_NOTEXIST, !current->songData.readme);
	CSVOption::setOption(CSVOptionConst::SELECT_BPMCHANGE_EXIST, (current->songData.minBPM != current->songData.maxBPM));
	CSVOption::setOption(CSVOptionConst::SELECT_BPMCHANGE_NOTEXIST, (current->songData.minBPM == current->songData.maxBPM));
	if (current->type == CSVSelectType::SONG) {
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_F, current->songRecord.rate<1.0/9*2);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_E, current->songRecord.rate>=1.0/9*2 && current->songRecord.rate<1.0/9*3);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_D, current->songRecord.rate>=1.0/9*3 && current->songRecord.rate<1.0/9*4);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_C, current->songRecord.rate>=1.0/9*4 && current->songRecord.rate<1.0/9*5);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_B, current->songRecord.rate>=1.0/9*5 && current->songRecord.rate<1.0/9*6);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_A, current->songRecord.rate>=1.0/9*6 && current->songRecord.rate<1.0/9*7);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_AA, current->songRecord.rate>=1.0/9*7 && current->songRecord.rate<1.0/9*8);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_AAA, current->songRecord.rate>=1.0/9*8);
		CSVOption::setOption(CSVOptionConst::SELECT_NOTPLAYED, current->songRecord.clear == CSVSongRecordClear::NOTPLAY);
		CSVOption::setOption(CSVOptionConst::SELECT_FAILED, current->songRecord.clear == CSVSongRecordClear::FAILED);
		CSVOption::setOption(CSVOptionConst::SELECT_EASYCLEARED, current->songRecord.clear == CSVSongRecordClear::EASYCLEAR);
		CSVOption::setOption(CSVOptionConst::SELECT_GROOVECLEARED, current->songRecord.clear == CSVSongRecordClear::NORMALCLEAR);
		CSVOption::setOption(CSVOptionConst::SELECT_HARDCLEARED, current->songRecord.clear == CSVSongRecordClear::HARDCLEAR);
		CSVOption::setOption(CSVOptionConst::SELECT_FULLCOMBO, current->songRecord.clear == CSVSongRecordClear::FULLCOMBO);
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_EASY, (current->songData.judge == CSVSongDataJudge::EASY));
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_NORMAL, (current->songData.judge == CSVSongDataJudge::NORMAL));
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_HARD, (current->songData.judge == CSVSongDataJudge::HARD));
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_VERYHARD, (current->songData.judge == CSVSongDataJudge::VERYHARD));
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_NONE, (current->songData.difficulty == CSVSongDataDifficulty::OTHER));
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_EASY, (current->songData.difficulty == CSVSongDataDifficulty::BEGINNER));
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_NORMAL, (current->songData.difficulty == CSVSongDataDifficulty::NORMAL));
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_HYPER, (current->songData.difficulty == CSVSongDataDifficulty::HYPER));
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_ANOTHER, (current->songData.difficulty == CSVSongDataDifficulty::ANOTHER));
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_INSANE, (current->songData.difficulty == CSVSongDataDifficulty::INSANE));
	} else {
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_F, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_E, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_D, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_C, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_B, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_A, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_AA, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_RANK_AAA, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_NOTPLAYED, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_FAILED, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_EASYCLEARED, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_GROOVECLEARED, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_HARDCLEARED, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_FULLCOMBO, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_EASY, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_NORMAL, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_HARD, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_VERYHARD, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_NONE, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_EASY, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_NORMAL, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_HYPER, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_ANOTHER, 0);
		CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_INSANE, 0);
	}
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_5KEY, (current->songData.key==5));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_7KEY, (current->songData.key==7));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_9KEY, (current->songData.key==9));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_10KEY, (current->songData.key==10));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_14KEY, (current->songData.key==14));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_5KEY, (current->songData.key==5));		// 
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_7KEY, (current->songData.key==7));		// 
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_9KEY, (current->songData.key==9));		// 
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_10KEY, (current->songData.key==10));	// 
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_14KEY, (current->songData.key==14));	// 
	
	CSVOption::setOption(CSVOptionConst::SELECT_BGA_EXIST, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_BGA_NOTEXIST, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_LN_EXIST, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_LN_NOTEXIST, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_LEVEL_UNDER, 1);
	CSVOption::setOption(CSVOptionConst::SELECT_LEVEL_OVER, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_REPLAY_EXIST, 0);
	CSVOption::setOption(CSVOptionConst::SELECT_REPLAY_NOTEXIST, 1);
	
	CSVOption::setOption(CSVOptionConst::LOAD_COMPLETE, 1);		// TODO ...? preview?
	CSVOption::setOption(CSVOptionConst::SELECT_REPLAY_NOTEXIST, 1);

	// about folder
#define MACRO_NOTEXIST(x) (current->type==CSVSelectType::SONG&&songs_cnt[(x)]==0)
#define MACRO_EXIST(x) (current->type==CSVSelectType::SONG&&songs_cnt[(x)]!=0)
#define MACRO_ONEEXIST(x) (current->type==CSVSelectType::SONG&&songs_cnt[(x)]==1)
#define MACRO_SEVERALEXIST(x) (current->type==CSVSelectType::SONG&&songs_cnt[(x)]>1)
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_NOTEXIST, MACRO_NOTEXIST(CSVSongDataDifficulty::BEGINNER));
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_EXIST, MACRO_EXIST(CSVSongDataDifficulty::BEGINNER));
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_ONE_EXIST, MACRO_ONEEXIST(CSVSongDataDifficulty::BEGINNER));
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_SEVERAL_EXIST, MACRO_SEVERALEXIST(CSVSongDataDifficulty::BEGINNER));
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_NOTEXIST, MACRO_NOTEXIST(CSVSongDataDifficulty::NORMAL));
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_EXIST, MACRO_EXIST(CSVSongDataDifficulty::NORMAL));
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_ONE_EXIST, MACRO_ONEEXIST(CSVSongDataDifficulty::NORMAL));
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_SEVERAL_EXIST, MACRO_SEVERALEXIST(CSVSongDataDifficulty::NORMAL));
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_NOTEXIST, MACRO_NOTEXIST(CSVSongDataDifficulty::HYPER));
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_EXIST, MACRO_EXIST(CSVSongDataDifficulty::HYPER));
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_ONE_EXIST, MACRO_ONEEXIST(CSVSongDataDifficulty::HYPER));
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_SEVERAL_EXIST, MACRO_SEVERALEXIST(CSVSongDataDifficulty::HYPER));
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_NOTEXIST, MACRO_NOTEXIST(CSVSongDataDifficulty::ANOTHER));
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_EXIST, MACRO_EXIST(CSVSongDataDifficulty::ANOTHER));
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_ONE_EXIST, MACRO_ONEEXIST(CSVSongDataDifficulty::ANOTHER));
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_SEVERAL_EXIST, MACRO_SEVERALEXIST(CSVSongDataDifficulty::ANOTHER));
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_NOTEXIST, MACRO_NOTEXIST(CSVSongDataDifficulty::INSANE));
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_EXIST, MACRO_EXIST(CSVSongDataDifficulty::INSANE));
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_ONE_EXIST, MACRO_ONEEXIST(CSVSongDataDifficulty::INSANE));
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_SEVERAL_EXIST, MACRO_SEVERALEXIST(CSVSongDataDifficulty::INSANE));
#undef MACRO_NOTEXIST
#undef MACRO_EXIST
#undef MACRO_ONEEXIST
#undef MACRO_SEVERALEXIST
#define MACRO_A(x) (songs_diff[(x)]&&songs_diff[(x)]->songData.level<=12)
#define MACRO_B(x) (songs_diff[(x)]&&songs_diff[(x)]->songData.level>12)
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_UNDER, MACRO_A(CSVSongDataDifficulty::BEGINNER));		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_OVER, MACRO_B(CSVSongDataDifficulty::BEGINNER));		// 
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_UNDER, MACRO_A(CSVSongDataDifficulty::NORMAL));		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_OVER, MACRO_B(CSVSongDataDifficulty::NORMAL));		// 
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_UNDER, MACRO_A(CSVSongDataDifficulty::HYPER));		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_OVER, MACRO_B(CSVSongDataDifficulty::HYPER));			// 
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_UNDER, MACRO_A(CSVSongDataDifficulty::ANOTHER));		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_OVER, MACRO_B(CSVSongDataDifficulty::ANOTHER));		// 
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_UNDER, MACRO_A(CSVSongDataDifficulty::INSANE));		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_OVER, MACRO_B(CSVSongDataDifficulty::INSANE));		// 
#undef MACRO_A
#undef MACRO_B
}

void CSVSelectList::goPrevList() {
	if (listHistory.size() > 0) {
		int l = listHistory.size()-1;

		// release current lists
		clearData();

		// move previous list and make new list
		data = listHistory[l]->data;
		makeSelectArray();

		// select & recalculate index
		Select(listHistory[l]->pos);
		currentSkinIndex = currentIndex;

		// release & pop
		free(listHistory[l]);
		listHistory.pop_back();
	}
}

void CSVSelectList::makeNewList() {
	// add previous one
	CSVSelectHistory *h = (CSVSelectHistory*)malloc(sizeof(CSVSelectHistory));
	memset(h, 0, sizeof(CSVSelectHistory));
	h->data = data;
	h->pos = currentIndex;
	listHistory.push_back(h);

	// select 0 & recalculate index
	Select(0);
	currentSkinIndex = currentIndex;

	// clear current
	clearData(false);
}

void CSVSelectList::MoveUp() {
	// check if moving
	if (moveDirection != 0) {
		// new move duration
		moveDuration = GetTickCount() - moveStartTime;
		if (moveDuration < MINMOVEDURATION)
			moveDuration = MINMOVEDURATION;
		return;
	}

	moveDirection = 1;
	moveDuration = DEFAULTMOVEDURATION;
	moveStartTime = GetTickCount();

	// move index change, and duration
	Select(currentIndex-1);
	moveProgress = 0;
}

void CSVSelectList::MoveDown() {
	// chek if moving
	if (moveDirection != 0) {
		// new move duration
		moveDuration = GetTickCount() - moveStartTime;
		if (moveDuration < MINMOVEDURATION)
			moveDuration = MINMOVEDURATION;
		return;
	}

	moveDirection = 2;
	moveDuration = DEFAULTMOVEDURATION;
	moveStartTime = GetTickCount();

	// move index change, and duration
	Select(currentIndex+1);
	moveProgress = 1;
	currentSkinIndex = currentIndex;
}

double CSVSelectList::CalculateMoving() {
	int eclipsedTime = GetTickCount() - moveStartTime;

	if (moveDirection == 1) {
		// moving up
		moveProgress = (double)eclipsedTime / moveDuration;
		if (moveProgress > 1) {
			// finish moving
			moveProgress = 0;
			moveDirection = 0;
			currentSkinIndex = currentIndex;
		}
	} else if (moveDirection == 2) {
		// moving down 
		moveProgress = 1-(double)eclipsedTime / moveDuration;
		if (moveProgress < 0) {
			// finish moving
			moveProgress = 0;
			moveDirection = 0;
		}
	}

	return moveProgress;
}

CSVSelectData* CSVSelectList::getSongData(int idx) {
	// this method will provide information
	// necessary for drawing bars.

	// if there's no data ...?
	if (songlist.size() == 0)
		return NULL;

	// check idx
	// TODO: recalculate offset
	return songlist[makeCorrectIndex(idx + currentSkinIndex)];
}

CSVSelectData* CSVSelectList::getSelectedSongData() {
	// if there's no data ...?
	if (songlist.size() == 0)
		return NULL;

	return songlist[currentIndex];
}

int CSVSelectList::makeCorrectIndex(int idx) {
	while (idx < 0)
		idx += songlist.size();
	idx %= songlist.size();
	return idx;
}

void CSVSelectList::checkSlider() {
	int p = songlist.size() * CSVSlider::getSliderValue(CSVSliderConst::SELECT_SLIDER);
	Select(p);
	currentSkinIndex = p;
}