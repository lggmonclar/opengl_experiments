#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"
#include "plane.h"
#include "directional_light.h"

class ShadowMappingScene : public Scene {
public:
	ShadowMappingScene();
	void Draw(Camera& camera, const Matrix4& P);
	void DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P);
	void DrawGUIOptions();
	~ShadowMappingScene();
private:
	unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	bool enableShadows = false;
	bool enablePCF = false;
	bool enableShadowBias = false;
	bool enablePeterPanning = false;
	bool moveCubes = true;
	unsigned int depthMapFBO;
	unsigned int depthMapID;

	Shader shadowDepthShader{ "shaders/14.shadow_depth_map.vert", "shaders/14.shadow_depth_map.frag" };
	Shader modelsShader{ "shaders/14.shadow_mapped_phong.vert", "shaders/14.shadow_mapped_phong.frag" };
	Shader debugDepthQuad{ "shaders/14.debug_quad.vert", "shaders/14.debug_quad.frag" };
	//Model sphereModel{ "models/sphere/sphere.obj" };
	//Model nanosuitModel { "models/nanosuit/nanosuit.obj" };
	DirectionalLight dirLight{Vector3(-2.0f, 4.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f)};
	Cube cube;
	Plane plane;
};