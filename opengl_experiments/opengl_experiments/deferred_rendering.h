#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"
#include "plane.h"

class DeferredRenderingScene : public Scene {
public:
	DeferredRenderingScene(const unsigned int screenWidth, const unsigned int screenHeight);
	void Draw(Camera& camera, const Matrix4& P);
	void DrawGUIOptions();
	~DeferredRenderingScene();
private:
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	bool useForwardRendering = false;

	Shader gBufferShader{ "shaders/19.g_buffer.vert", "shaders/19.g_buffer.frag" };
	Shader deferredLightingShader{ "shaders/19.deferred_lighting.vert", "shaders/19.deferred_lighting.frag" };
	Shader pointLightShader{ "shaders/1.diffuse_color.vert", "shaders/1.diffuse_color.frag" };

	Model sphereModel{ "models/sphere/sphere.obj" };
	Texture diffTexture{ "container2.png", "textures"};
	Texture specularTexture{ "container2_specular.png", "textures"};
	Cube cubeModel;
	Plane planeModel;
	void RenderQuad();
	void DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P);
	void DrawLights(const Shader &shader, const Matrix4& V, const Matrix4& P);

	std::vector<Vector3> lightPositions;
	std::vector<Vector3> lightColors;
};