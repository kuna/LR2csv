#include "Stdafx.h"
#include "ScenePlay.h"

bool isKeyPressed[20];
int keyArr[20] = {VK_SHIFT, 'Z', 'S', 'X', 'D', 'C', 'F', 'V', };
BMSKeyData* pressedLN[20];
int pressedJudge[20];

void ScenePlay::OnKeyDown(int key) {
	for (int i=0; i<8; i++) {
		if (keyArr[i]==key) {
			LanePress(i);
		}
	}
}

void ScenePlay::OnKeyUp(int key) {
	for (int i=0; i<8; i++) {
		if (keyArr[i] == key) {
			LaneUp(i);
		}
	}
}

void ScenePlay::OnLButtonDown(int x, int y) {
}

void ScenePlay::OnLButtonUp(int x, int y) {
}

void ScenePlay::OnRButtonDown(int x, int y) {
}

void ScenePlay::OnRButtonUp(int x, int y) {
}

void ScenePlay::OnMouseMove(int x, int y) {
}

void ScenePlay::OnMouseWheel(int delta) {
}

void ScenePlay::LanePress(int num) {
	if (!bmsData)
		return;
	if (isKeyPressed[num])
		return;
	isKeyPressed[num] = true;
	
	// press effect
	CSVTimer::deActiviateTimer(CSVTimerConst::KEYOFF_TIMER+num);
	CSVTimer::setTime(CSVTimerConst::KEYDOWN_TIMER+num);

	// check judgement
	BMSKeyData *note = getLastValidKey(num);
	if (note) {
		int curTime = CSVTimer::getTime(CSVTimerConst::PLAYSTART);
		int judge = checkJudge(curTime - note->getTime());	// TODO need to implement getTimeMilisec
		if (judge == PlayJudge::NOJUDGE)
			return;		// nothing happens.
		if (judge == PlayJudge::NOPOOR) {
			// guaue down and bad judge, but nothing else.
			processJudge(judge);
			return;
		}
		
		// over ÍöPoor, note is used.
		note->setAttr(1);

		// check if LN or not.
		if (note->is1PLNChannel()) {
			// if judge is under good, then LN failed.
			if (judge <= PlayJudge::GOOD) {
				processJudge(judge);
				BMSKeyData *notePair = bmsData->getPairLN(note);
				if (notePair)
					notePair->setAttr(2);
			} else {
				// store judge information.
				pressedLN[num] = note;
				pressedJudge[num] = judge;
			}
		} else {
			// bom effect
			if (judge >= PlayJudge::GREAT)
				CSVTimer::setTime(CSVTimerConst::BOMB_TIMER+num);

			// process judge
			processJudge(judge);
		}
	}

}

void ScenePlay::LaneUp(int num) {
	if (!bmsData)
		return;
	if (!isKeyPressed[num])
		return;
	isKeyPressed[num] = false;

	CSVTimer::setTime(CSVTimerConst::KEYOFF_TIMER+num);
	CSVTimer::deActiviateTimer(CSVTimerConst::KEYDOWN_TIMER+num);
	
	// check is LN pressed.
	if (pressedLN[num]) {
		// get LN
		BMSKeyData *note = getLastValidKey(num);

		// check time
		int curTime = CSVTimer::getTime(CSVTimerConst::PLAYSTART);
		int judge = checkJudge(curTime - note->getTime());

		// if judge is over BAD, then LN failed.
		if (judge <= PlayJudge::BAD) {
			processJudge(judge);
		} else {
			// set attr
			note->setAttr(1);	// 2: failed, 1: succeed. dont draw.

			// set combo & judge
			processJudge(pressedJudge[num]);

			// clear
			pressedJudge[num] = 0;
			pressedLN[num] = 0;
		}
	}
}

void ScenePlay::SetBMSData(BMSData *bmsData) {
	this->bmsData = bmsData;
}

BMSKeyData* ScenePlay::getLastValidKey(int num) {
	for (int i=0; i<bmsData->bmsdata.size(); i++) {
		BMSKeyData *note = bmsData->bmsdata[i];
		if ((note->is1PChannel() || note->is1PLNChannel()) && note->getAttr() == 0) {
			if (num < 0 || num == note->getKeyNum())
				return note;
		}
	}

	// not found
	return 0;
}

int ScenePlay::checkJudge(int diffTime) {
	if (diffTime < 0)
		diffTime *= -1;

	// get judge time
	int currentJudgeTime = PlayJudgeTime::EASY;
	if (bmsData->rank == 0)
		currentJudgeTime = PlayJudgeTime::VERYHARD;
	else if (bmsData->rank == 1)
		currentJudgeTime = PlayJudgeTime::HARD;
	else if (bmsData->rank == 2)
		currentJudgeTime = PlayJudgeTime::NORMAL;

	// and check judge for difftime.
	if (diffTime < GetJudgeTime(PlayJudge::PGREAT, currentJudgeTime))
		return PlayJudge::PGREAT;
	else if (diffTime < GetJudgeTime(PlayJudge::GREAT, currentJudgeTime))
		return PlayJudge::GREAT;
	else if (diffTime < GetJudgeTime(PlayJudge::GOOD, currentJudgeTime))
		return PlayJudge::GOOD;
	else if (diffTime < GetJudgeTime(PlayJudge::BAD, currentJudgeTime))
		return PlayJudge::BAD;
	else if (diffTime < GetJudgeTime(PlayJudge::POOR, currentJudgeTime))
		return PlayJudge::POOR;
	else if (diffTime < GetJudgeTime(PlayJudge::NOPOOR, currentJudgeTime))
		return PlayJudge::NOPOOR;
	else
		return PlayJudge::NOJUDGE;
}

void ScenePlay::processJudge(int judge) {
	// set combo
	if (judge >= PlayJudge::GOOD)
		combo++;
	else if (judge != PlayJudge::NOPOOR)
		combo = 0;	// ÍöPoor saves combo.

	// set combo & judge.
	CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_EMPTYPOOR, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_POOR, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_BAD, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_GOOD, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_GREAT, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_PGREAT, 0);
	switch (judge) {
	case PlayJudge::NOPOOR:
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_EMPTYPOOR, 1);
		break;
	case PlayJudge::POOR:
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_POOR, 1);
		break;
	case PlayJudge::BAD:
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_BAD, 1);
		break;
	case PlayJudge::GOOD:
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_GOOD, 1);
		break;
	case PlayJudge::GREAT:
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_GREAT, 1);
		break;
	case PlayJudge::PGREAT:
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_PGREAT, 1);
		break;
	}
	CSVNumber::setNumber(CSVNumConst::PLAYER_COMBO, combo);
	if (combo > CSVNumber::getNumber(CSVNumConst::PLAYER_MAXCOMBO))
		CSVNumber::setNumber(CSVNumConst::PLAYER_MAXCOMBO, combo);
	CSVTimer::setTime(CSVTimerConst::JUDGE_1P);
	
	// if combo == total note count, then fullcombo effect.
	if (bmsData->notecnt == combo)
		CSVTimer::setTime(CSVTimerConst::FULLCOMBO_1P);
}

int ScenePlay::GetJudgeTime(int judge, int judgetime) {
	switch (judge) {
	case PlayJudge::NOPOOR:
		return judgetime*10;
	case PlayJudge::POOR:
		return judgetime*8;
	case PlayJudge::BAD:
		return judgetime*6;
	case PlayJudge::GOOD:
		return judgetime*4;
	case PlayJudge::GREAT:
		return judgetime*2;
	case PlayJudge::PGREAT:
		return judgetime;
	}
	return 0;	// set judge!
}

void ScenePlay::InitalizeCSV() {
	// init maxcombo
	// init score
}