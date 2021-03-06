#include "Scene.h"

class SceneCommon: public Scene {
public:
	void OnKeyDown(int key);
	void OnKeyUp(int key);
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnRButtonDown(int x, int y);
	void OnRButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseWheel(int delta);
	void InitalizeScene();
	void InvalidateScene();

	Scene* currentInput;
	bool receiveInput;
};