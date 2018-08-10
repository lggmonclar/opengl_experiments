#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"

class CubemapReflectionsScene : public Scene {
public:
	CubemapReflectionsScene();
	void Draw(Camera& camera, const Matrix4& P);
	void DrawGUIOptions();
	~CubemapReflectionsScene();
private:
	struct BoxData {
		BoxData() = default;
		BoxData(Vector3 pos, Vector3 col) : position(pos), color(col) {}
		Vector3 position;
		Vector3 color;
	};
	bool useDynamicCubemap = false;
	bool rotateCubes = true;
	unsigned int reflectionFBOs[6];
	unsigned int dynamicCubemapID;

	Shader modelShader{ "shaders/13.reflections.vert", "shaders/13.reflections.frag" };
	Shader boxShader{ "shaders/7.light_box.vert", "shaders/7.light_box.frag" };
	BoxData boxData[5] = {
		BoxData {
			Vector3(1.2f, 1.2f, 0.0f),
			Vector3(1.0f, 1.0f, 1.0f)
		},
		BoxData{
			Vector3(0.8f, 0.8f, 1.0f),
			Vector3(1.0f, 0.0f, 0.0f)
		},
		BoxData{
			Vector3(0.2f, 0.2f, 2.0f),
			Vector3(0.0f, 1.0f, 0.0f)
		},
		BoxData{
			Vector3(-0.4f, -0.4f, 3.0f),
			Vector3(0.0f, 0.0f, 1.0f)
		},
		BoxData{
			Vector3(-1.0f, -1.0f, 4.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		}
	};
	Skybox skybox{
		"textures/skyboxes/1/right.jpg",
		"textures/skyboxes/1/left.jpg",
		"textures/skyboxes/1/top.jpg",
		"textures/skyboxes/1/bottom.jpg",
		"textures/skyboxes/1/front.jpg",
		"textures/skyboxes/1/back.jpg" };
	Model sphereModel{ "models/sphere/sphere.obj" };
	Vector3 modelPos{0.0f, 0.0f, 0.0f};
	Cube cube;
};