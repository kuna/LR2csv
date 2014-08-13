#include "Stdafx.h"
#include "SceneDecide.h"
#include "GameManager.h"

void SceneDecide::OnKeyDown(int key) {
	// ESC: cancel, ELSE: okay
	if (key == VK_ESCAPE) {
		isCanceled = true;
	} else {
		isCanceled = false;
	}
	
	// call endScene
	GameManager::getCSVHandler()->endScene();
}

void SceneDecide::OnKeyUp(int key) {
}

void SceneDecide::OnLButtonDown(int x, int y) {
	// ok
}

void SceneDecide::OnLButtonUp(int x, int y) {
}

void SceneDecide::OnRButtonDown(int x, int y) {
	// cancel
}

void SceneDecide::OnRButtonUp(int x, int y) {
}

void SceneDecide::OnMouseMove(int x, int y) {
}

void SceneDecide::OnMouseWheel(int delta) {
}

void SceneDecide::InitalizeScene() {
	isCanceled = false;
}

void SceneDecide::InvalidateScene() {
}


bool SceneDecide::getIsCanceled() {
	return isCanceled;
}