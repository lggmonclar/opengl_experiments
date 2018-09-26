#pragma once
#include "imgui/imgui.h"
#include <math/math.h>
#include "camera.h"

class Scene {
public:
	Scene() = default;
	Scene(const unsigned int screenWidth, const unsigned int screenHeight) : viewportWidth(screenWidth), viewportHeight(screenHeight) {};
	virtual void Draw(Camera& camera, const Matrix4& P) = 0;
	virtual void DrawGUIOptions() = 0;
	void setViewport(unsigned int width, unsigned int height);
protected:
	unsigned int viewportWidth, viewportHeight;
};