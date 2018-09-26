#include "deferred_rendering.h"

DeferredRenderingScene::DeferredRenderingScene(const unsigned int screenWidth, const unsigned int screenHeight) : Scene(screenWidth, screenHeight) {
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// lighting info
	// -------------
	srand(13);
	for (unsigned int i = 0; i < 100; i++) {
		// calculate slightly random offsets
		float xPos = (-5.0f + i/10) * 2.0f;
		float yPos = ((rand() % 100) / 100.0) * 2.0 + 0.1f;
		float zPos = (-5.0f + i%10) * 2.0f;
		lightPositions.push_back(Vector3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		lightColors.push_back(Vector3(rColor, gColor, bColor));
	}

	// shader configuration
	// --------------------
	deferredLightingShader.use();
	deferredLightingShader.setInt("gPosition", 0);
	deferredLightingShader.setInt("gNormal", 1);
	deferredLightingShader.setInt("gAlbedoSpec", 2);

	//set sphere textures
	gBufferShader.use();
	gBufferShader.setInt("texture_diffuse1", 0);
	gBufferShader.setInt("texture_specular1", 1);
}

void DeferredRenderingScene::Draw(Camera& camera, const Matrix4& P) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 1. geometry pass: render scene's geometry/color data into gbuffer
	// -----------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Matrix4 view = camera.getViewMatrix();
	gBufferShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffTexture.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTexture.id);
	DrawObjects(gBufferShader, view, P);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
	// -----------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	deferredLightingShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	// send light relevant uniforms
	for (unsigned int i = 0; i < lightPositions.size(); i++) {
		deferredLightingShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
		deferredLightingShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		// update attenuation parameters and calculate radius
		const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const float linear = 0.4;
		const float quadratic = 1.2;
		deferredLightingShader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
		deferredLightingShader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		// then calculate radius of light volume/sphere
		const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].x, lightColors[i].y), lightColors[i].z);
		float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
		deferredLightingShader.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
	}
	deferredLightingShader.setVec3("viewPos", camera.position);
	// finally render quad
	RenderQuad();

	// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
	// ----------------------------------------------------------------------------------
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
											   // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
											   // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
											   // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	glBlitFramebuffer(0, 0, viewportWidth, viewportHeight, 0, 0, viewportWidth, viewportHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. render lights on top of scene
	// --------------------------------
	pointLightShader.use();
	DrawLights(pointLightShader, view, P);
}

void DeferredRenderingScene::DrawObjects(const Shader &shader, const Matrix4& V, const Matrix4& P) {
	//Floor
	Matrix4 M = Matrix4();
	M.scale(.5f);
	shader.setVec3("color", Vector3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", M);
	shader.setMat4("view", V);
	shader.setMat4("projection", P);
	planeModel.Draw(shader);

	//Spheres
	for (int i = -10; i < 10; ++i) {
		for (int j = -10; j < 10; ++j) {
			M = Matrix4();
			M.scale(0.1f).translate(i*1.2f, 0.4f + (sin(glfwGetTime() + i) + cos(glfwGetTime() + j)) / 4.0f, j*1.2f);
			shader.setMat4("model", M);
			sphereModel.Draw(shader);
		}
	}
}

void DeferredRenderingScene::DrawLights(const Shader &shader, const Matrix4& V, const Matrix4& P) {
	//Cube point lights
	for (unsigned int i = 0; i < lightPositions.size(); i++) {
		Matrix4 M = Matrix4();
		M.scale(0.1f).translate(lightPositions[i]);
		pointLightShader.setMat4("model", M);
		pointLightShader.setMat4("mvp", M * V * P);
		pointLightShader.setVec3("color", lightColors[i]);
		cubeModel.Draw(pointLightShader);
	}
}

void DeferredRenderingScene::DrawGUIOptions() {
	ImGui::Checkbox("Use forward rendering", &useForwardRendering);
}

void DeferredRenderingScene::RenderQuad() {
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

DeferredRenderingScene::~DeferredRenderingScene() {}