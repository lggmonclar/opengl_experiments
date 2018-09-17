#include "omnidirectional_shadow_mapping.h"

OmnidirectionalShadowMappingScene::OmnidirectionalShadowMappingScene() {
	GenerateDepthMap(true);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	modelsShader.setInt("shadowMap", 0);

	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
}

void OmnidirectionalShadowMappingScene::Draw(Camera& camera, const Matrix4& P) {
	if (2 << DEPTH_MAP_EXPONENT != DEPTH_MAP_SIZE) {
		GenerateDepthMap();
	}

	//Point light render to depth
	glViewport(0, 0, DEPTH_MAP_SIZE, DEPTH_MAP_SIZE);
	Matrix4 pointLightP = Matrix4::perspectiveProjection(deg2rad(90.0f), 1.0f, 0.5f, 100.0f);

	Vector3 directionVectors[6] = {
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(-1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f, -1.0f)
	};
	Vector3 upVectors[6] = {
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f)
	};
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	pointShadowDepthShader.use();
	pointShadowDepthShader.setFloat("farPlane", 100.0f);
	pointShadowDepthShader.setVec3("lightPos", pointLightPosition);
	for (int i = 0; i < 6; ++i) {
		Matrix4 pointLightV;
		pointLightV.lookAt(pointLightPosition, pointLightPosition + directionVectors[i], upVectors[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeDepthMapID, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawObjects(pointShadowDepthShader, pointLightV, pointLightP);
	}

	//Color buffer rendering
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelsShader.use();
	modelsShader.setBool("enablePCF", enablePCF);
	modelsShader.setVec3("viewPos", camera.position);
	modelsShader.setVec3("lightPos", pointLightPosition);
	modelsShader.setFloat("farPlane", 100.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepthMapID);
	DrawObjects(modelsShader, camera.getViewMatrix(), P);

	pointLightModelShader.use();
	DrawPointLights(pointLightModelShader, camera.getViewMatrix(), P);

	//debug quad
	// render Depth map to quad for visual debugging
	// ---------------------------------------------
	if (showDepthMapDebug) {
		debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", 1.0f);
		debugDepthQuad.setFloat("far_plane", 7.5f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeDepthMapID);
		RenderDebugQuad();
	}
}

void OmnidirectionalShadowMappingScene::DrawPointLights(const Shader &shader, const Matrix4& V, const Matrix4& P) {
	//Point Lights
	Matrix4 M = Matrix4();
	M.scale(.05f).translate(pointLightPosition);
	shader.setVec3("color", 1.0f, 1.0f, 1.0f);
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	pointLightModel.Draw(shader);
}

void OmnidirectionalShadowMappingScene::DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P) {
	//Floor
	Matrix4 M = Matrix4();
	shader.setVec3("color", Vector3(1.0f, 1.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	plane.Draw(shader);

	//Cubes
	M = Matrix4();
	M.rotateAbout(deg2rad(glfwGetTime() * 200), Vector3(1.0f, 0.0f, 1.0f)).scale(.5f).translate(Vector3(-0.0f, 0.5f, -1.0f));
	if (moveObjects) {
		M.rotateY(glfwGetTime() * 1.2);
	}
	shader.setVec3("color", Vector3(1.0f, 1.0f, 0.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);

	M = Matrix4();
	M.rotateAbout(deg2rad(glfwGetTime() * 100), Vector3(1.0f, 0.0f, 1.0f)).scale(.5f).translate(Vector3(2.0f, 0.0f, 1.0f));
	if (moveObjects) {
		M.rotateY(glfwGetTime());
	}
	shader.setVec3("color", Vector3(1.0f, 0.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);

	M = Matrix4();
	M.rotateAbout(deg2rad(glfwGetTime()*50), Vector3(1.0f, 0.0f, 1.0f)).scale(.25f).translate(Vector3(-1.0f, .5f, 2.0f));
	if (moveObjects) {
		M.rotateY(glfwGetTime() * 0.5);
	}
	shader.setVec3("color", Vector3(0.0f, 1.0f, 1.0f));
	shader.setMat4("mvp", M * V * P);
	shader.setMat4("model", M);
	cube.Draw(shader);
}

void OmnidirectionalShadowMappingScene::DrawGUIOptions() {
	ImGui::Text("Current map size : %d", 2 << DEPTH_MAP_EXPONENT);
	ImGui::SliderInt("Depth Exponent", &DEPTH_MAP_EXPONENT, 8, 11);
	ImGui::Checkbox("Enable soft shadows", &enablePCF);
	ImGui::Checkbox("Move objects", &moveObjects);
}

void OmnidirectionalShadowMappingScene::GenerateDepthMap(bool firstPass) {
	if (firstPass) {
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &cubeDepthMapID);
	}
	DEPTH_MAP_SIZE = 2 << DEPTH_MAP_EXPONENT;


	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepthMapID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (unsigned int j = 0; j < 6; ++j)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT,
			DEPTH_MAP_SIZE, DEPTH_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeDepthMapID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OmnidirectionalShadowMappingScene::~OmnidirectionalShadowMappingScene() {
	glDeleteFramebuffers(1, &depthMapFBO);
}

void OmnidirectionalShadowMappingScene::RenderDebugQuad() {
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