#pragma once
#include "imgui/imgui.h"
#include <math/math.h>
class Scene {
public:
	virtual void Draw(const Matrix4& V, const Matrix4& P) = 0;
	virtual void DrawGUIOptions() = 0;
private:
};