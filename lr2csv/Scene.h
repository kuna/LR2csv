#pragma once

class Scene {
public:
	virtual void OnLButtonDown(int, int) = 0;
	virtual void OnLButtonUp(int, int) = 0;
	virtual void OnRButtonDown(int, int) = 0;
	virtual void OnRButtonUp(int, int) = 0;
	virtual void OnMouseMove(int, int) = 0;
	virtual void OnMouseWheel(int) = 0;
	virtual void OnKeyDown(int) = 0;
	virtual void OnKeyUp(int) = 0;
	virtual void InvalidateScene() = 0;
	virtual void InitalizeScene() = 0;
};