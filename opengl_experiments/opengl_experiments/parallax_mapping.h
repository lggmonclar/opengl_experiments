#pragma once
#include "scene.h";
#include "model.h";

class ParallaxMappingScene : public Scene {
public:
	ParallaxMappingScene();
	void Draw(Camera& camera, const Matrix4& P);
	void DrawGUIOptions();
private:
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	float heightScale = 0.1f;
	void RenderQuad();
	Model pointLightModel{ "models/sphere/sphere.obj" };
	Shader parallaxMapShader { "shaders/17.parallax_map.vert", "shaders/17.parallax_map.frag" };
	Shader diffuseColorShader { "shaders/1.diffuse_color.vert", "shaders/1.diffuse_color.frag" };
	Texture diffuseMap{ "brick_wall.jpg", "textures/parallax_brick" };
	Texture normalMap{ "brick_wall_normal.jpg", "textures/parallax_brick" };
	Texture parallaxMap{ "brick_wall_disp.jpg", "textures/parallax_brick" };
	Vector3 lightPos{ 5.0, 5.0, 10.0 };
};