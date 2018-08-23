#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"
#include "plane.h"
#include "directional_light.h"

class DirectionalShadowMappingScene : public Scene {
public:
	DirectionalShadowMappingScene();
	void Draw(Camera& camera, const Matrix4& P);
	void DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P);
	void DrawGUIOptions();
	~DirectionalShadowMappingScene();
private:
	int DEPTH_MAP_EXPONENT = 10;
	unsigned int DEPTH_MAP_SIZE;

	unsigned int directionalDepthMapFBO;
	unsigned int directionalDepthMapID;
	unsigned int quadVAO;
	unsigned int quadVBO;

	bool enablePCF = true;
	bool moveObjects = false;
	bool showDepthMapDebug = false;
	bool rotateDirectionalLight = false;

	Shader directionalShadowDepthShader{ "shaders/14.directional_shadow_depth_map.vert", "shaders/14.directional_shadow_depth_map.frag" };
	Shader modelsShader{ "shaders/14.shadow_mapped_phong.vert", "shaders/14.shadow_mapped_phong.frag" };
	Shader debugDepthQuad{ "shaders/14.debug_quad.vert", "shaders/14.debug_quad.frag" };
	DirectionalLight dirLight{Vector3(-2.0f, 4.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f)};
	Cube cube;
	Plane plane;

	void GenerateDepthMap(bool firstPass = false);
	void RenderDebugQuad();
};