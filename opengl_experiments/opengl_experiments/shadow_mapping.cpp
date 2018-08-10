#include "shadow_mapping.h"

void renderQuad();

ShadowMappingScene::ShadowMappingScene() {
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMapID);
	glBindTexture(GL_TEXTURE_2D, depthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	modelsShader.setInt("shadowMap", 0);


	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
}

void ShadowMappingScene::Draw(Camera& camera, const Matrix4& P) {
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	Matrix4 lightSpaceMatrix, lightV, lightP;
	lightP = Matrix4::orthographicProjection(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	lightV.lookAt(dirLight.position, dirLight.direction, Vector3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightV * lightP;

	shadowDepthShader.use();
	shadowDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawObjects(shadowDepthShader, lightV, lightP);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelsShader.use();
	modelsShader.setVec3("viewPos", camera.position);
	modelsShader.setVec3("lightPos", dirLight.position);
	modelsShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapID);
	DrawObjects(modelsShader, camera.getViewMatrix(), P);

	//debug quad
	// render Depth map to quad for visual debugging
	// ---------------------------------------------
	//debugDepthQuad.use();
	//debugDepthQuad.setFloat("near_plane", 1.0f);
	//debugDepthQuad.setFloat("far_plane", 7.5f);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMapID);
	//renderQuad();
}

void ShadowMappingScene::DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P) {
	Matrix4 M = Matrix4();
	shader.setVec3("color", Vector3(1.0f, 1.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	plane.Draw(shader);

	M = Matrix4();
	M.translate(Vector3(0.0f, 1.5f, 0.0f)).scale(.5f);
	shader.setVec3("color", Vector3(1.0f, 1.0f, 0.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);

	M = Matrix4();
	M.translate(Vector3(2.0f, 0.0f, 1.0f)).scale(.5f);
	shader.setVec3("color", Vector3(1.0f, 0.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);

	M = Matrix4();
	M.translate(Vector3(-1.0f, 0.0f, 2.0f)).rotateAbout(deg2rad(60.0f), Vector3(1.0f, 0.0f, 1.0f)).scale(.25f);
	shader.setVec3("color", Vector3(0.0f, 1.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);
}

void ShadowMappingScene::DrawGUIOptions() {
}

ShadowMappingScene::~ShadowMappingScene() {
	glDeleteFramebuffers(1, &depthMapFBO);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
	if (quadVAO == 0) {
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}