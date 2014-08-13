#include "Stdafx.h"
#include "SceneResult.h"
#include "GameManager.h"

void SceneResult::OnKeyDown(int key) {
}

void SceneResult::OnKeyUp(int key) {
	// press any key to quit scene.
	GameManager::getCSVHandler()->endScene();
}

void SceneResult::OnLButtonDown(int x, int y) {
	// press any key to quit scene.
	GameManager::getCSVHandler()->endScene();
}

void SceneResult::OnLButtonUp(int x, int y) {
}

void SceneResult::OnRButtonDown(int x, int y) {
	// press any key to quit scene.
	GameManager::getCSVHandler()->endScene();
}

void SceneResult::OnRButtonUp(int x, int y) {
}

void SceneResult::OnMouseMove(int x, int y) {
}

void SceneResult::OnMouseWheel(int delta) {
}

void SceneResult::InitalizeScene() {
	// get result from scenePlay

	// set DJ level
	// TODO: 2P
	int exscore = CSVNumber::getNumber(CSVNumConst::PLAYER_1P_EXSCORE);
	int prev_exscore = 0;	// TODO
	double rate = exscore / ((double)CSVNumber::getNumber(CSVNumConst::PLAYER_1P_TOTALNOTES)*2);
	double prev_rate = prev_exscore / ((double)CSVNumber::getNumber(CSVNumConst::PLAYER_1P_TOTALNOTES)*2);
	
	CSVOption::setOption(CSVOptionConst::RESULT_1P_AAA, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_AA, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_A, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_B, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_C, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_D, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_E, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_1P_F, 0);
	if (rate > 8/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_AAA, 1);
	else if (rate > 7/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_AA, 1);
	else if (rate > 6/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_A, 1);
	else if (rate > 5/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_B, 1);
	else if (rate > 4/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_C, 1);
	else if (rate > 3/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_D, 1);
	else if (rate > 2/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_1P_E, 1);
	else
		CSVOption::setOption(CSVOptionConst::RESULT_1P_F, 1);
	
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_AAA, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_AA, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_A, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_B, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_C, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_D, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_E, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_AFTER_F, 0);
	if (rate > 8/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_AAA, 1);
	else if (rate > 7/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_AA, 1);
	else if (rate > 6/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_A, 1);
	else if (rate > 5/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_B, 1);
	else if (rate > 4/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_C, 1);
	else if (rate > 3/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_D, 1);
	else if (rate > 2/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_E, 1);
	else
		CSVOption::setOption(CSVOptionConst::RESULT_AFTER_F, 1);

	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_AAA, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_AA, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_A, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_B, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_C, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_D, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_E, 0);
	CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_F, 0);
	if (prev_rate > 8/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_AAA, 1);
	else if (prev_rate > 7/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_AA, 1);
	else if (prev_rate > 6/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_A, 1);
	else if (prev_rate > 5/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_B, 1);
	else if (prev_rate > 4/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_C, 1);
	else if (prev_rate > 3/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_D, 1);
	else if (prev_rate > 2/9.0)
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_E, 1);
	else
		CSVOption::setOption(CSVOptionConst::RESULT_BEFORE_F, 1);

	// set combo, great, rate, score, etc.
	CSVNumber::setNumber(CSVNumConst::RESULT_MAXCOMBO, CSVNumber::getNumber(CSVNumConst::PLAYER_1P_MAXCOMBO));
	CSVNumber::setNumber(CSVNumConst::RESULT_MAXCOMBO_BEFORE, 0);	// TODO
	CSVNumber::setNumber(CSVNumConst::RESULT_MAXCOMBO_DIFF, 
		CSVNumber::getNumber(CSVNumConst::RESULT_MAXCOMBO) - CSVNumber::getNumber(CSVNumConst::RESULT_MAXCOMBO_BEFORE));
	if (CSVNumber::getNumber(CSVNumConst::RESULT_MAXCOMBO_DIFF) > 0)
		CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_MAXCOMBO, 1);
	else
		CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_MAXCOMBO, 0);
	CSVNumber::setNumber(CSVNumConst::RESULT_EXSCORE, CSVNumber::getNumber(CSVNumConst::PLAYER_1P_EXSCORE));
	CSVNumber::setNumber(CSVNumConst::RESULT_EXSCORE_BEFORE, prev_exscore);	// TODO
	CSVNumber::setNumber(CSVNumConst::RESULT_EXSCORE_DIFF, 
		CSVNumber::getNumber(CSVNumConst::RESULT_EXSCORE) - CSVNumber::getNumber(CSVNumConst::RESULT_EXSCORE_BEFORE));
	if (CSVNumber::getNumber(CSVNumConst::RESULT_EXSCORE_DIFF) > 0)
		CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_SCORE, 1);
	else
		CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_SCORE, 0);
	CSVNumber::setNumber(CSVNumConst::RESULT_MINBP, 
		CSVNumber::getNumber(CSVNumConst::PLAYER_1P_BAD) + CSVNumber::getNumber(CSVNumConst::PLAYER_1P_POOR));
	CSVNumber::setNumber(CSVNumConst::RESULT_MINBP_BEFORE, 0);		// TODO
	CSVNumber::setNumber(CSVNumConst::RESULT_MINBP_DIFF, 
		CSVNumber::getNumber(CSVNumConst::RESULT_MINBP) - CSVNumber::getNumber(CSVNumConst::RESULT_MINBP_BEFORE));
	if (CSVNumber::getNumber(CSVNumConst::RESULT_MINBP_DIFF) < 0)
		CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_BP, 1);
	else
		CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_BP, 0);

	// TODO: updated IR rank
	// TODO: updated trial
	CSVOption::setOption(CSVOptionConst::RESULT_UPDATE_TRIAL, 1);
}

void SceneResult::InvalidateScene() {
}