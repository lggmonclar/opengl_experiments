#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"
#include "plane.h"
#include "directional_light.h"

class OmnidirectionalShadowMappingScene : public Scene {
public:
	OmnidirectionalShadowMappingScene();
	void Draw(Camera& camera, const Matrix4& P);
	void DrawPointLights(const Shader & shader, const Matrix4 & V, const Matrix4 & P);
	void DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P);
	void DrawGUIOptions();
	~OmnidirectionalShadowMappingScene();
private:
	int DEPTH_MAP_EXPONENT = 11;
	unsigned int DEPTH_MAP_SIZE;

	unsigned int depthMapFBO;
	unsigned int cubeDepthMapID;
	unsigned int quadVAO;
	unsigned int quadVBO;

	bool enablePCF = true;
	bool moveObjects = false;
	bool showDepthMapDebug = false;

	Shader pointShadowDepthShader{ "shaders/15.point_shadow_depth_map.vert", "shaders/15.point_shadow_depth_map.frag" };
	Shader modelsShader{ "shaders/15.omni_shadow_mapped_phong.vert", "shaders/15.omni_shadow_mapped_phong.frag" };
	Shader pointLightModelShader { "shaders/1.diffuse_color.vert", "shaders/1.diffuse_color.frag" };
	Shader debugDepthQuad{ "shaders/14.debug_quad.vert", "shaders/14.debug_quad.frag" };
	Cube cube;
	Plane plane;
	Model pointLightModel { "models/sphere/sphere.obj" };
	Vector3 pointLightPosition{Vector3(0.0f, 2.0f, 0.0)};

	void GenerateDepthMap(bool firstPass = false);
	void RenderDebugQuad();
};