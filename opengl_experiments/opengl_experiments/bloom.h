#pragma once
#include "scene.h"
#include "shader.h"
#include "skybox.h"
#include "model.h"
#include "cube.h"
#include "plane.h"
#include "directional_light.h"

class BloomScene : public Scene {
public:
	BloomScene(const unsigned int screenWidth, const unsigned int screenHeight);
	void Draw(Camera& camera, const Matrix4& P);
	void DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P);
	void DrawGUIOptions();
	~BloomScene();
private:
	unsigned int hdrFBO;
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	unsigned int colorBuffers[2];
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	bool enableBloom = true;
	bool showHDRColorBufferDebug = false;
	float exposure = 1.0f;
	float blurWeightCoeff = 1.0f;

	Shader shader{ "shaders/18.bloom.vert", "shaders/18.bloom.frag" };
	Shader shaderLight{"shaders/18.bloom.vert", "shaders/1.diffuse_color.frag"};
	Shader shaderBlur{ "shaders/18.blur.vert", "shaders/18.blur.frag" };
	Shader shaderBloomFinal{ "shaders/18.bloom_final.vert", "shaders/18.bloom_final.frag" };

	Cube cube;
	Plane plane;
	void RenderPostProcessingQuad();
	void RenderHDRDebugQuad();

	std::vector<Vector3> lightPositions{ 
		Vector3{0.0f, 1.5f, 1.5f},
		Vector3{ -4.0f, 0.5f, -3.0f },
		Vector3{ 3.0f, 0.5f, 1.0f },
		Vector3{ -.8f, 2.4f, -1.0f }
	};
	std::vector<Vector3> lightColors {
		Vector3{ 2.0f, 2.0f, 2.0f },
		Vector3{ 1.5f, 0.0f, 0.0f },
		Vector3{ 0.0f, 0.0f, 1.5f },
		Vector3{ 0.0f, 1.5f, 0.0f }
	};
};