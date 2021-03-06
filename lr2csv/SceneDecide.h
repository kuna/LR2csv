#include "Scene.h"

class SceneDecide: public Scene {
public:
	// redeclaration
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

	void Init();
	bool getIsCanceled();
private:
	bool isCanceled;
};