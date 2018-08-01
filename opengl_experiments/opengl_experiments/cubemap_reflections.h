#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"

class CubemapReflectionsScene : public Scene {
public:
	CubemapReflectionsScene();
	void Draw(const Matrix4& V, const Matrix4& P);
	void DrawGUIOptions();
private:
	Shader modelShader{ "shaders/13.reflections.vert", "shaders/13.reflections.frag" };
	Shader lightBoxShader{ "shaders/7.light_box.vert", "shaders/7.light_box.frag" };
	Skybox skybox{
		"textures/skyboxes/1/right.jpg",
		"textures/skyboxes/1/left.jpg",
		"textures/skyboxes/1/top.jpg",
		"textures/skyboxes/1/bottom.jpg",
		"textures/skyboxes/1/front.jpg",
		"textures/skyboxes/1/back.jpg" };
	Model ourModel{ "models/nanosuit.obj" };
	Vector3 pointLightPosition{ 1.2f, 1.2f, 0.0f };
	Cube cube;
};