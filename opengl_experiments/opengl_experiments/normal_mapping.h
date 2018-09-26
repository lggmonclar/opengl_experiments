#pragma once
#include "scene.h";
#include "model.h";

class NormalMappingScene : public Scene {
public:
	NormalMappingScene(const unsigned int screenWidth, const unsigned int screenHeight) : Scene(screenWidth, screenHeight) {};
	void Draw(Camera& camera, const Matrix4& P);
	void DrawGUIOptions();
private:
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void RenderQuad();
	Model pointLightModel{ "models/sphere/sphere.obj" };
	Shader normalMapShader{ "shaders/16.normal_map.vert", "shaders/16.normal_map.frag" };
	Shader diffuseColorShader{ "shaders/1.diffuse_color.vert", "shaders/1.diffuse_color.frag" };
	Texture normalMap{"normal_map.png", "textures"};
	Vector3 lightPos{ 0.0, 0.0, 4.0 };
};