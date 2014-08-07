#include "CSVSelectList.h"
#include "CSVNumber.h"
#include "CSVOption.h"
#include "CSVTimer.h"
#include "CSVConst.h"
#include "CSVBargraph.h"
#include "CSVSlider.h"

#define DEFAULTMOVEDURATION 300
#define MINMOVEDURATION 100

double CSVSelectList::moveProgress = 0;
DWORD CSVSelectList::moveStartTime;
int CSVSelectList::currentIndex;
int CSVSelectList::currentSkinIndex;
int CSVSelectList::moveDuration = DEFAULTMOVEDURATION;	// init
// TODO: using function calling frequency, automatically controll moveDuration ...?
int CSVSelectList::moveDirection;
int nextMoveDuration = DEFAULTMOVEDURATION;

std::vector<CSVSelectData*> CSVSelectList::data;
std::vector<CSVSelectSong*> CSVSelectList::songlist;

void CSVSelectList::addData(CSVSelectDataConst index, TCHAR *path) {
	// create 
	CSVSelectSong *ssdata = new CSVSelectSong();
	memset(ssdata, 0, sizeof(CSVSelectSong));

	ssdata->minBPM = 120;
	ssdata->maxBPM = 120;
	ssdata->key = 7;
	ssdata->difficulty = rand()%2+1;				// TODO
	ssdata->judge = CSVSelectDataJudge::EASY;		// TODO
	wcscpy(ssdata->path, path);
	ssdata->noteCnt = 1234;				// TODO
	ssdata->level = rand()%10+2;		// TODO

	// first search all data
	// if anything dir(name) exists
	TCHAR dir[256];
	wcscpy(dir, path);
	GetDirectory(dir);

	bool find = false;
	for (int i=0; i<data.size(); i++) {
		if (wcscmp(data[i]->dir, dir) == 0) {
			ssdata->parent = data[i];
			data[i]->songs.push_back(ssdata);
			find = true;
			break;
		}
	}

	// if no group exists, create one
	if (!find) {
		CSVSelectData* sdata = new CSVSelectData();
		memset(sdata, 0, sizeof(CSVSelectData));

		// fill data
		wcscpy(sdata->dir, dir);
		sdata->index = index;
		sdata->readme[0] = 0;	// TODO: scan Directory for *.txt

		ssdata->parent = sdata;
		sdata->songs.push_back(ssdata);
		data.push_back(sdata);
	}
}

void CSVSelectList::clearData() {
	// first delete all songlist to prevent drawing
	songlist.clear();

	// remove data
	for (int i=0; i<data.size(); i++) {
		for (int j=0; j<data[i]->songs.size(); j++) {
			delete data[i]->songs[j];
		}
		delete data[i];
	}
	data.clear();
}

void CSVSelectList::makeSelectArray(int key, CSVSelectDataDifficulty difficulty) {
	// make songlist
	// to show in song list
	
	for (int i=0; i<data.size(); i++) {
		for (int j=0; j<data[i]->songs.size(); j++) {
			if (data[i]->songs[j]->key == key) {
				if (difficulty == CSVSelectDataDifficulty::ALL
					|| data[i]->songs[j]->difficulty == difficulty) {

					songlist.push_back(data[i]->songs[j]);
				}
			}
		}
	}
}

CSVSelectData* CSVSelectList::getSelectDataFromSong(CSVSelectSong *song) {
	// update: use backpointer
	return song->parent;
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
	CSVSelectSong* current = songlist[songlistIdx];
	CSVSelectData* currentGroup = getSelectDataFromSong(current);
	
	OutputDebugString(L"SONG CHANGED - ");
	OutputDebugString(current->path);
	OutputDebugString(L"\n");

	// change slider
	CSVSlider::setSliderValue(CSVSliderConst::SELECT_SLIDER, (double)songlistIdx/songlist.size());

	// change timer
	CSVTimer::setTime(CSVTimerConst::SONGSELECT_CHANGE);

	// get all song difficulty
	CSVSelectSong* songs_diff[10];
	int songs_cnt[10];
	memset(&songs_diff, 0, sizeof(CSVSelectSong*)*10);
	memset(&songs_cnt, 0, sizeof(int)*10);
	for (int i=0; i<currentGroup->songs.size(); i++) {
		songs_diff[currentGroup->songs[i]->difficulty] = currentGroup->songs[i];
		songs_cnt[currentGroup->songs[i]->difficulty]++;
	}
	songs_diff[current->difficulty] = current;

	// set number
	CSVNumber::setNumber(CSVNumConst::BPM, current->minBPM);
	CSVNumber::setNumber(CSVNumConst::MINBPM, current->minBPM);
	CSVNumber::setNumber(CSVNumConst::MAXBPM, current->maxBPM);
	if (songs_diff[CSVSelectDataDifficulty::BEGINNER] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_BEGINNER_LEVEL, songs_diff[CSVSelectDataDifficulty::BEGINNER]->level);
	if (songs_diff[CSVSelectDataDifficulty::NORMAL] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_NORMAL_LEVEL, songs_diff[CSVSelectDataDifficulty::NORMAL]->level);
	if (songs_diff[CSVSelectDataDifficulty::HYPER] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_HYPER_LEVEL, songs_diff[CSVSelectDataDifficulty::HYPER]->level);
	if (songs_diff[CSVSelectDataDifficulty::ANOTHER] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_ANOTHER_LEVEL, songs_diff[CSVSelectDataDifficulty::ANOTHER]->level);
	if (songs_diff[CSVSelectDataDifficulty::INSANE] > 0)
		CSVNumber::setNumber(CSVNumConst::SONG_INSANE_LEVEL, songs_diff[CSVSelectDataDifficulty::INSANE]->level);

	// set bargraph
#define LIMIT(x) ((x>1)?1:(x))
	if (songs_diff[CSVSelectDataDifficulty::BEGINNER] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_BEGINNER_LEVEL, LIMIT(songs_diff[CSVSelectDataDifficulty::BEGINNER]->level/12.0));
	if (songs_diff[CSVSelectDataDifficulty::NORMAL] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_NORMAL_LEVEL, LIMIT(songs_diff[CSVSelectDataDifficulty::NORMAL]->level/12.0));
	if (songs_diff[CSVSelectDataDifficulty::HYPER] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_HYPER_LEVEL, LIMIT(songs_diff[CSVSelectDataDifficulty::HYPER]->level/12.0));
	if (songs_diff[CSVSelectDataDifficulty::ANOTHER] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_ANOTHER_LEVEL, LIMIT(songs_diff[CSVSelectDataDifficulty::ANOTHER]->level/12.0));
	if (songs_diff[CSVSelectDataDifficulty::INSANE] > 0)
		CSVBargraph::setVal(CSVBargraphConst::FOLDER_INSANE_LEVEL, LIMIT(songs_diff[CSVSelectDataDifficulty::INSANE]->level/12.0));
#undef LIMIT

	// change OPs
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_SONG, (currentGroup->index == CSVSelectDataConst::SONG));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_COURSE, (currentGroup->index == CSVSelectDataConst::FOLDER));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_FOLDER, (currentGroup->index == CSVSelectDataConst::COURSE));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_NEWCOURSE, (currentGroup->index == CSVSelectDataConst::COURSEMAKE));
	CSVOption::setOption(CSVOptionConst::SELECT_BAR_PLAYABLE, (currentGroup->index == CSVSelectDataConst::SONG));	// TODO

	CSVOption::setOption(CSVOptionConst::SELECT_READMETXT_EXIST, (currentGroup->readme[0] != 0));
	CSVOption::setOption(CSVOptionConst::SELECT_READMETXT_NOTEXIST, (currentGroup->readme[0] == 0));
	CSVOption::setOption(CSVOptionConst::SELECT_BPMCHANGE_EXIST, (current->minBPM != current->maxBPM));
	CSVOption::setOption(CSVOptionConst::SELECT_BPMCHANGE_NOTEXIST, (current->minBPM == current->maxBPM));
	CSVOption::setOption(CSVOptionConst::SELECT_NOTPLAYED, 1);	// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_RANK_AA, 1);	// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_5KEY, (current->key==5));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_7KEY, (current->key==7));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_9KEY, (current->key==9));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_10KEY, (current->key==10));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_ORIGNAL_14KEY, (current->key==14));
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_5KEY, (current->key==5));		// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_7KEY, (current->key==7));		// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_9KEY, (current->key==9));		// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_10KEY, (current->key==10));	// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_KEY_PLAY_14KEY, (current->key==14));	// TODO
	CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_NONE, (current->difficulty == CSVSelectDataDifficulty::OTHER));
	CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_EASY, (current->difficulty == CSVSelectDataDifficulty::BEGINNER));
	CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_NORMAL, (current->difficulty == CSVSelectDataDifficulty::NORMAL));
	CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_HYPER, (current->difficulty == CSVSelectDataDifficulty::HYPER));
	CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_ANOTHER, (current->difficulty == CSVSelectDataDifficulty::ANOTHER));
	CSVOption::setOption(CSVOptionConst::SELECT_DIFFICULTY_INSANE, (current->difficulty == CSVSelectDataDifficulty::INSANE));
	CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_EASY, (current->judge == CSVSelectDataJudge::EASY));
	CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_NORMAL, (current->judge == CSVSelectDataJudge::NORMAL));
	CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_HARD, (current->judge == CSVSelectDataJudge::HARD));
	CSVOption::setOption(CSVOptionConst::SELECT_JUDGE_VERYHARD, (current->judge == CSVSelectDataJudge::VERYHARD));
	
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
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_NOTEXIST, songs_cnt[CSVSelectDataDifficulty::BEGINNER]==0);
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_EXIST, songs_cnt[CSVSelectDataDifficulty::BEGINNER]!=0);
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_ONE_EXIST, songs_cnt[CSVSelectDataDifficulty::BEGINNER]==1);
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_SEVERAL_EXIST, songs_cnt[CSVSelectDataDifficulty::BEGINNER]>1);
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_NOTEXIST, songs_cnt[CSVSelectDataDifficulty::NORMAL]==0);
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_EXIST, songs_cnt[CSVSelectDataDifficulty::NORMAL]!=0);
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_ONE_EXIST, songs_cnt[CSVSelectDataDifficulty::NORMAL]==1);
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_SEVERAL_EXIST, songs_cnt[CSVSelectDataDifficulty::NORMAL]>1);
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_NOTEXIST, songs_cnt[CSVSelectDataDifficulty::HYPER]==0);
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_EXIST, songs_cnt[CSVSelectDataDifficulty::HYPER]!=0);
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_ONE_EXIST, songs_cnt[CSVSelectDataDifficulty::HYPER]==1);
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_SEVERAL_EXIST, songs_cnt[CSVSelectDataDifficulty::HYPER]>1);
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_NOTEXIST, songs_cnt[CSVSelectDataDifficulty::ANOTHER]==0);
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_EXIST, songs_cnt[CSVSelectDataDifficulty::ANOTHER]!=0);
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_ONE_EXIST, songs_cnt[CSVSelectDataDifficulty::ANOTHER]==1);
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_SEVERAL_EXIST, songs_cnt[CSVSelectDataDifficulty::ANOTHER]>1);
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_NOTEXIST, songs_cnt[CSVSelectDataDifficulty::INSANE]==0);
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_EXIST, songs_cnt[CSVSelectDataDifficulty::INSANE]!=0);
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_ONE_EXIST, songs_cnt[CSVSelectDataDifficulty::INSANE]==1);
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_SEVERAL_EXIST, songs_cnt[CSVSelectDataDifficulty::INSANE]>1);
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_UNDER, 1);		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_BEGINNER_OVER, 0);		// 
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_UNDER, 1);		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_NORMAL_OVER, 0);		// 
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_UNDER, 1);		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_HYPER_OVER, 0);			// 
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_UNDER, 1);		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_ANOTHER_OVER, 0);		// 
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_UNDER, 1);		// TODO
	CSVOption::setOption(CSVOptionConst::FOLDER_INSANE_OVER, 0);		// 
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

	if (currentSkinIndex < 0)
		currentSkinIndex += songlist.size();
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
	currentSkinIndex++;
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
			currentSkinIndex--;
		}
	} else if (moveDirection == 2) {
		// moving down 
		moveProgress = 1-(double)eclipsedTime / moveDuration;
		if (moveProgress < 0) {
			// finish moving
			moveProgress = 0;
			moveDirection = 0;
	
			if (currentSkinIndex >= songlist.size())
				currentSkinIndex -= songlist.size();
		}
	}

	return moveProgress;
}

CSVSelectSong* CSVSelectList::getSongData(int idx) {
	// this method will provide information
	// necessary for drawing bars.

	// if there's no data ...?
	if (songlist.size() == 0)
		return NULL;

	// check idx
	// TODO: recalculate offset
	return songlist[makeCorrectIndex(idx + currentSkinIndex)];
}

CSVSelectSong* CSVSelectList::getSelectedSongData() {
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