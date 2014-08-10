#include "Stdafx.h"
#include "SceneCommon.h"
#include "GameManager.h"

void SceneCommon::OnKeyDown(int key) {
	if (key == VK_F5) {
		// refresh
	}

	if (key == 'Z') {
		// timer
		CSVTimer::deActiviateTimer(CSVTimerConst::KEYOFF_TIMER);
		CSVTimer::setTime(CSVTimerConst::KEYDOWN_TIMER);
	} else if (key == VK_UP) {
		CSVSelectList::MoveUp();
	} else if (key == VK_DOWN) {
		CSVSelectList::MoveDown();
	}
}

void SceneCommon::OnKeyUp(int key) {
	if (key == 'Z') {
		// key input test
		CSVTimer::deActiviateTimer(CSVTimerConst::KEYDOWN_TIMER);
		CSVTimer::setTime(CSVTimerConst::KEYOFF_TIMER);
		CSVTimer::setTime(CSVTimerConst::BOMB_TIMER);
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

	if (key == VK_RIGHT) {
		// select
		GameManager::SelectList();
	} else if (key == VK_LEFT) {
		// TODO: go to previous one
		CSVSelectList::goPrevList();
	}
	
	if (key == VK_F1) {
		GameManager::loadScene(GAMEMODE::SELECT);
		GameManager::startScene();
	} else if (key == VK_F2) {
		GameManager::loadScene(GAMEMODE::DECIDE);
		GameManager::startScene();
	} else if (key == VK_F3) {
		GameManager::loadScene(GAMEMODE::PLAY);
		GameManager::startScene();
	} else if (key == VK_F4) {
		GameManager::loadScene(GAMEMODE::RESULT);
		GameManager::startScene();
	} else if (key == VK_F5) {
		// refresh, without reloading resources
		GameManager::setGameMode(GameManager::GameMode);
		GameManager::startScene();
	}
}

void SceneCommon::OnLButtonDown(int x, int y) {
	CSVButton::Click(x, y);
	CSVSlider::mouseDown(x, y);
}

void SceneCommon::OnLButtonUp(int x, int y) {
	CSVSlider::mouseUp();
}

void SceneCommon::OnRButtonDown(int x, int y) {
	// cancel event
	// like shutter close, parent folder, cancel song.
}

void SceneCommon::OnRButtonUp(int x, int y) {
}

void SceneCommon::OnMouseMove(int x, int y) {
	CSVRenderer::SetCurPos(x, y);
	if (CSVSlider::mouseMove(x, y) == CSVSliderConst::SELECT_SLIDER) {
		CSVSelectList::checkSlider();
	}
}

void SceneCommon::OnMouseWheel(int wheel) {
	if (wheel > 0) {
		CSVSelectList::MoveUp();
	} else if (wheel < 0) {
		CSVSelectList::MoveDown();
	}
}