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

	// shutter control
	if (key == VK_LEFT) {
		double shutter = CSVSlider::getSliderValue(CSVSliderConst::SHUTTER_1P);
		shutter -= 0.1;
		if (shutter < 0)
			shutter = 0;
		CSVSlider::setSliderValue(CSVSliderConst::SHUTTER_1P, shutter);
	} else if (key == VK_RIGHT) {
		double shutter = CSVSlider::getSliderValue(CSVSliderConst::SHUTTER_1P);
		shutter += 0.1;
		if (shutter > 1)
			shutter = 1;
		CSVSlider::setSliderValue(CSVSliderConst::SHUTTER_1P, shutter);
	}
}

void ScenePlay::OnKeyUp(int key) {
	for (int i=0; i<8; i++) {
		if (keyArr[i] == key) {
			LaneUp(i);
		}
	}

	
	if (key == 'Q') {
		// combo test
		CSVTimer::setTime(CSVTimerConst::JUDGE_1P);
	}
	if (key == 'W') {
		// fullcombo test
		CSVTimer::setTime(CSVTimerConst::FULLCOMBO_1P);
	}
	if (key == 'R') {
		// rhythm timer test
		CSVTimer::setTime(CSVTimerConst::RHYTHM_TIMER);
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
		CSVNumber::increaseNumber(CSVNumConst::PLAYER_1P_POOR);
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_EMPTYPOOR, 1);
		fillGuage(-2);	// TODO
		break;
	case PlayJudge::POOR:
		CSVNumber::increaseNumber(CSVNumConst::PLAYER_1P_POOR);
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_POOR, 1);
		fillGuage(-4);	// TODO
		leftnote--;
		break;
	case PlayJudge::BAD:
		CSVNumber::increaseNumber(CSVNumConst::PLAYER_1P_BAD);
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_BAD, 1);
		fillGuage(-6);	// TODO
		leftnote--;
		break;
	case PlayJudge::GOOD:
		CSVNumber::increaseNumber(CSVNumConst::PLAYER_1P_GOOD);
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_GOOD, 1);
		fillGuage(10);	// TODO
		leftnote--;
		break;
	case PlayJudge::GREAT:
		CSVNumber::increaseNumber(CSVNumConst::PLAYER_1P_GREAT);
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_GREAT, 1);
		fillGuage(20);	// TODO
		exScore++;
		leftnote--;
		break;
	case PlayJudge::PGREAT:
		CSVNumber::increaseNumber(CSVNumConst::PLAYER_1P_PGREAT);
		CSVOption::setOption(CSVOptionConst::PLAY_1P_JUDGE_PGREAT, 1);
		fillGuage(20);	// TODO
		exScore+=2;
		leftnote--;
		break;
	}
	// TODO: change it to 1P/2P Combo
	CSVNumber::setNumber(CSVNumConst::PLAYER_COMBO, combo);
	if (combo > CSVNumber::getNumber(CSVNumConst::PLAYER_MAXCOMBO))
		CSVNumber::setNumber(CSVNumConst::PLAYER_MAXCOMBO, combo);
	CSVTimer::setTime(CSVTimerConst::JUDGE_1P);

	// calculate current rate, target exscore, score.
	int ex_total = bmsData->notecnt * 2;
	int ex_target_current = (bmsData->notecnt-leftnote)*targetRate*2;
	int prev_exscore = 0;				// TODO
	double rate_current = (double)exScore / ex_total * 100;

	// set data to CSVNumber
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_EXSCORE, exScore);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_EXSCORE_DIFF, exScore - ex_target_current);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_SCORE, 200000 * rate_current);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_MAXCOMBO, CSVNumber::getNumber(CSVNumConst::PLAYER_COMBO));
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_NOWCOMBO, CSVNumber::getNumber(CSVNumConst::PLAYER_MAXCOMBO));
	//CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GREAT, 0);
	//CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GOOD, 0);
	//CSVNumber::setNumber(CSVNumConst::PLAYER_1P_BAD, 0);
	//CSVNumber::setNumber(CSVNumConst::PLAYER_1P_POOR, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_RATE, rate_current);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_RATE_DECIMAL, (int)(rate_current * 100) % 100);
	
	// set Bargraph
	CSVBargraph::setVal(CSVBargraphConst::PLAY_EXSCORE_1P, (double)exScore / ex_total);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_HIGHSCORE_1P_RESULT, (double)exScore / ex_total);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_CURRENT, (double)ex_target_current / ex_total);
	
	// this number is related to graph.
	int high_ex = (ex_target_current>exScore)?ex_target_current:exScore;
	int nextExscore = ex_total;
	if (rate_current > 8/9.0) {
		// stay there
	} else if (rate_current > 7/9.0) {
		nextExscore *= 8/9.0;
	} else if (rate_current > 6/9.0) {
		nextExscore *= 7/9.0;
	} else if (rate_current > 5/9.0) {
		nextExscore *= 6/9.0;
	} else if (rate_current > 4/9.0) {
		nextExscore *= 5/9.0;
	} else if (rate_current > 3/9.0) {
		nextExscore *= 4/9.0;
	} else if (rate_current > 2/9.0) {
		nextExscore *= 3/9.0;
	} else {
		nextExscore *= 2/9.0;
	}
	double rate_high = (double)high_ex / ex_total * 100;
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_HIGHER, high_ex);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_TARGET, ex_target_current);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_HIGHER_DIFF, high_ex - exScore);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_TARGET_DIFF, high_ex - ex_target_current);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_NEXT_DIFF, nextExscore - exScore);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_HIGHER_RATE, rate_high);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_HIGHER_RATE_DECIMAL, (int)(rate_high * 100) % 100);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_TARGET_RATE, targetRate);
	CSVNumber::setNumber(CSVNumConst::PLAY_SCORE_TARGET_RATE_DECIMAL, (int)(targetRate * 100) % 100);
	
	// set dst option
	setDSTOption();

	// if last note? (song end)
	if (!leftnote) {
		// last note
		CSVTimer::setTime(CSVTimerConst::LASTNOTE_1P);

		// if combo == total note count, then fullcombo effect.
		if (bmsData->notecnt == combo)
			CSVTimer::setTime(CSVTimerConst::FULLCOMBO_1P);

		// song end, prepare to end scene.
	}
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

void ScenePlay::fillGuage(int num) {
	int curGuage = CSVNumber::getNumber(CSVNumConst::PLAYER_1P_GUAGE);
	curGuage += num;
	if (curGuage < 0)
		curGuage = 0;
	if (curGuage > 100)
		curGuage = 100;
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GUAGE, curGuage);

	// TODO: if hard guage && guage is 0, then set failed signal!
}

void ScenePlay::setDSTOption() {
	// call after EXscore changed...
	// TODO: 2P?
	int ex_total = bmsData->notecnt * 2;
	double rate = (double)exScore / ex_total;

	// 1. rating
	CSVOption::setOption(CSVOptionConst::PLAY_1P_AAA, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_AA, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_A, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_B, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_C, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_D, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_E, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_F, 0);
	if (rate > 8/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_AAA, 1);
	else if (rate > 7/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_AA, 1);
	else if (rate > 6/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_A, 1);
	else if (rate > 5/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_B, 1);
	else if (rate > 4/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_C, 1);
	else if (rate > 3/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_D, 1);
	else if (rate > 2/9.0)
		CSVOption::setOption(CSVOptionConst::PLAY_1P_E, 1);
	else 
		CSVOption::setOption(CSVOptionConst::PLAY_1P_F, 1);

	// 2. guage
	int guage = CSVNumber::getNumber(CSVNumConst::PLAYER_1P_GUAGE);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_100, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_99, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_89, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_79, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_69, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_59, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_49, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_39, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_29, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_19, 0);
	CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_10, 0);
	if (guage == 100) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_100, 1);
	} else if (guage >= 90) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_99, 1);
	} else if (guage >= 80) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_89, 1);
	} else if (guage >= 70) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_79, 1);
	} else if (guage >= 60) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_69, 1);
	} else if (guage >= 50) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_59, 1);
	} else if (guage >= 40) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_49, 1);
	} else if (guage >= 30) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_39, 1);
	} else if (guage >= 20) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_29, 1);
	} else if (guage >= 10) {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_19, 1);
	} else {
		CSVOption::setOption(CSVOptionConst::PLAY_1P_GUAGE_10, 1);
	}
}

void ScenePlay::InvalidateScene() {
	if (bmsData) {
		// refresh time
		int eclipsedTime = CSVTimer::getTime(CSVTimerConst::PLAYSTART);
		int time_min = eclipsedTime/1000/60;
		int time_sec = eclipsedTime/1000%60;
		CSVNumber::setNumber(CSVNumConst::BMS_MINUTE, time_min);
		CSVNumber::setNumber(CSVNumConst::BMS_SECOND, time_sec);
		int remainTime = bmsData->time - eclipsedTime;
		if (remainTime < 0)
			remainTime = 0;
		time_min = remainTime/1000/60;
		time_sec = eclipsedTime/1000%60;
		CSVNumber::setNumber(CSVNumConst::BMS_REMAIN_MINUTE, time_min);
		CSVNumber::setNumber(CSVNumConst::BMS_REMAIN_SECOND, time_sec);

		// set current song progress
		CSVSlider::setSliderValue(CSVSliderConst::SONG_PROGRESS, 
			(double)eclipsedTime / bmsData->time / 1000);
	}

	// TODO: check poor bga
	//if (CSVOption::getOption(CSVOptionConst::PLAY_1P_POORBGA_ON) && poorTime) {
	//}
}

void ScenePlay::InitalizeScene() {
	// off timer
	CSVTimer::deActiviateTimer(CSVTimerConst::LASTNOTE_1P);
	CSVTimer::deActiviateTimer(CSVTimerConst::RHYTHM_TIMER);

	// init total note
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_TOTALNOTES, bmsData->notecnt);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_TOTALRATE, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_TOTALRATE_DECIMAL, 0);

	// init score
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_EXSCORE, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_EXSCORE_DIFF, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_SCORE, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_MAXCOMBO, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_NOWCOMBO, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_PGREAT, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GREAT, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GOOD, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_BAD, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_POOR, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_RATE, 0);
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_RATE_DECIMAL, 0);

	// set number
	CSVNumber::setNumber(CSVNumConst::BMS_BPM, bmsData->BPM);	// set BPM
	CSVNumber::setNumber(CSVNumConst::PLAYER_1P_GUAGE, 20);		// set Guage (normal)

	// set bargraph
	CSVBargraph::setVal(CSVBargraphConst::PLAY_EXSCORE_1P, 0);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_HIGHSCORE_1P_RESULT, 0);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_CURRENT, 0);
	CSVBargraph::setVal(CSVBargraphConst::PLAY_TARGET_EXSCORE_2P_RESULT, targetRate);

	// set target .. etc...
	targetRate = 0.66;
	exScore = 0;

	// set left note
	leftnote=bmsData->notecnt;
}