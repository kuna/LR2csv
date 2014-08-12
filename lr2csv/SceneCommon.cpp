#include "Stdafx.h"
#include "SceneCommon.h"
#include "GameManager.h"

void SceneCommon::OnKeyDown(int key) {
	if (currentInput)
		currentInput->OnKeyDown(key);
}

void SceneCommon::OnKeyUp(int key) {
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

	if (currentInput)
		currentInput->OnKeyUp(key);
}

void SceneCommon::OnLButtonDown(int x, int y) {
	CSVButton::Click(x, y);
	CSVSlider::mouseDown(x, y);

	if (currentInput)
		currentInput->OnLButtonDown(x, y);
}

void SceneCommon::OnLButtonUp(int x, int y) {
	CSVSlider::mouseUp();
	
	if (currentInput)
		currentInput->OnLButtonUp(x, y);
}

void SceneCommon::OnRButtonDown(int x, int y) {
	// cancel event
	// like shutter close, parent folder, cancel song.
	if (currentInput)
		currentInput->OnRButtonDown(x, y);
}

void SceneCommon::OnRButtonUp(int x, int y) {
	if (currentInput)
		currentInput->OnRButtonUp(x, y);
}

void SceneCommon::OnMouseMove(int x, int y) {
	CSVRenderer::SetCurPos(x, y);
	if (CSVSlider::mouseMove(x, y) == CSVSliderConst::SELECT_SLIDER) {
		CSVSelectList::checkSlider();
	}
	
	if (currentInput)
		currentInput->OnMouseMove(x, y);
}

void SceneCommon::OnMouseWheel(int wheel) {
	if (wheel > 0) {
		CSVSelectList::MoveUp();
	} else if (wheel < 0) {
		CSVSelectList::MoveDown();
	}
	
	if (currentInput)
		currentInput->OnMouseWheel(wheel);
}