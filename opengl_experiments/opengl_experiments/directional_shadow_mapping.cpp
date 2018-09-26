#include "directional_shadow_mapping.h"

DirectionalShadowMappingScene::DirectionalShadowMappingScene(const unsigned int screenWidth, const unsigned int screenHeight): Scene(screenWidth, screenHeight) {
	GenerateDepthMap(true);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	modelsShader.setInt("shadowMap", 0);

	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
}

void DirectionalShadowMappingScene::Draw(Camera& camera, const Matrix4& P) {
	if (2 << DEPTH_MAP_EXPONENT != DEPTH_MAP_SIZE) {
		GenerateDepthMap();
	}

	//Directional Light render to depth
	if (rotateDirectionalLight) {
		dirLight.direction.x = sin(glfwGetTime());
		dirLight.direction.z = cos(glfwGetTime());
	}
	Matrix4 lightSpaceMatrix, dirLightV, dirLightP;
	dirLightP = Matrix4::orthographicProjection(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	dirLightV.lookAt(dirLight.position, dirLight.direction, Vector3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = dirLightV * dirLightP;

	directionalShadowDepthShader.use();
	directionalShadowDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, DEPTH_MAP_SIZE, DEPTH_MAP_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, directionalDepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawObjects(directionalShadowDepthShader, dirLightV, dirLightP);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Color buffer rendering
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelsShader.use();
	modelsShader.setBool("enablePCF", enablePCF);
	modelsShader.setVec3("viewPos", camera.position);
	modelsShader.setVec3("lightPos", dirLight.position);
	modelsShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, directionalDepthMapID);
	DrawObjects(modelsShader, camera.getViewMatrix(), P);

	//debug quad
	// render Depth map to quad for visual debugging
	// ---------------------------------------------
	if (showDepthMapDebug) {
		debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", 1.0f);
		debugDepthQuad.setFloat("far_plane", 7.5f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, directionalDepthMapID);
		RenderDebugQuad();
	}
}

void DirectionalShadowMappingScene::DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P) {
	//Floor
	Matrix4 M = Matrix4();
	shader.setVec3("color", Vector3(1.0f, 1.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	plane.Draw(shader);

	//Cubes
	M = Matrix4();
	M.translate(Vector3(0.0f, 1.5f, 0.0f)).scale(.5f);
	if (moveObjects) {
		M.translate(Vector3(0.0f, 0.5f + sin(glfwGetTime()), 0.0f));
	}
	shader.setVec3("color", Vector3(1.0f, 1.0f, 0.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);

	M = Matrix4();
	M.translate(Vector3(2.0f, 0.0f, 1.0f)).scale(.5f);
	if (moveObjects) {
		M.rotateY(glfwGetTime());
	}
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

void DirectionalShadowMappingScene::DrawGUIOptions() {
	ImGui::Text("Current map size : %d", 2 << DEPTH_MAP_EXPONENT);
	ImGui::SliderInt("Depth Exponent", &DEPTH_MAP_EXPONENT, 8, 12);
	ImGui::Checkbox("Enable soft shadows", &enablePCF);
	ImGui::Checkbox("Move objects", &moveObjects);
	ImGui::Checkbox("Rotate directional light", &rotateDirectionalLight);
	ImGui::Checkbox("Show depth map debug quad", &showDepthMapDebug);
}

void DirectionalShadowMappingScene::GenerateDepthMap(bool firstPass) {
	if (firstPass) {
		glGenTextures(1, &directionalDepthMapID);
		glGenFramebuffers(1, &directionalDepthMapFBO);
	}
	DEPTH_MAP_SIZE = 2 << DEPTH_MAP_EXPONENT;

	//Directional depth map
	glBindTexture(GL_TEXTURE_2D, directionalDepthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_SIZE, DEPTH_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, directionalDepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, directionalDepthMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

DirectionalShadowMappingScene::~DirectionalShadowMappingScene() {
	glDeleteFramebuffers(1, &directionalDepthMapFBO);
}

void DirectionalShadowMappingScene::RenderDebugQuad() {
	if (quadVAO == 0) {
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  -0.5f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f,  -0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  -1.0f, 0.0f, 1.0f, 0.0f,
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