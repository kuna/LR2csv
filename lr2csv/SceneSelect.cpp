#include "Stdafx.h"
#include "SceneSelect.h"
#include "GameManager.h"

void SceneSelect::OnKeyDown(int key) {
	if (key == DIK_UP) {
		CSVSelectList::MoveUp();
	} else if (key == DIK_DOWN) {
		CSVSelectList::MoveDown();
	}
}

void SceneSelect::OnKeyUp(int key) {
	if (key == DIK_RIGHT) {
		// select
		GameManager::SelectList();
	} else if (key == DIK_LEFT) {
		// TODO: go to previous one
		CSVSelectList::goPrevList();
	}
}

void SceneSelect::OnLButtonDown(int x, int y) {
	// ok
}

void SceneSelect::OnLButtonUp(int x, int y) {
}

void SceneSelect::OnRButtonDown(int x, int y) {
	// cancel
}

void SceneSelect::OnRButtonUp(int x, int y) {
}

void SceneSelect::OnMouseMove(int x, int y) {
}

void SceneSelect::OnMouseWheel(int delta) {
}

void SceneSelect::InitalizeScene() {
}

void SceneSelect::InvalidateScene() {
}