#include "cubemap_reflections.h"

CubemapReflectionsScene::CubemapReflectionsScene() {
	glGenFramebuffers(6, reflectionFBOs);

	glGenTextures(1, &dynamicCubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubemapID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


		glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBOs[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dynamicCubemapID, 0);
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 512);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubemapReflectionsScene::Draw(Camera& camera, const Matrix4& P) {
	if (useDynamicCubemap) {
		glViewport(0, 0, 512, 512);
		Matrix4 perspective = Matrix4::perspectiveProjection(deg2rad(90.0f), 1.0f, 0.5f, 100.0f);
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
		for (int i = 0; i < 6; ++i) {
			glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBOs[i]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Matrix4 view;
			view.lookAt(modelPos, modelPos + directionVectors[i], upVectors[i]);

			boxShader.use();
			for (int i = 0; i < 5; i++) {
				boxShader.setVec3("color", boxData[i].color);
				Matrix4 boxM;
				boxM.scale(0.2f, 0.2f, 0.2f).translate(boxData[i].position);
				if (rotateCubes) {
					boxM.rotateY(glfwGetTime() / (i + 1));
				}
				boxShader.setMat4("mvp", boxM * view * perspective);
				cube.Draw(boxShader);
			}

			skybox.Draw(view, perspective);
		}
		glViewport(0, 0, 1366, 768);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	Matrix4 V = camera.getViewMatrix();
	boxShader.use();
	for (int i = 0; i < 5; i++) {
		boxShader.setVec3("color", boxData[i].color);
		Matrix4 boxM;
		boxM.scale(0.2f, 0.2f, 0.2f).translate(boxData[i].position);
		if (rotateCubes) {
			boxM.rotateY(glfwGetTime() / (i + 1));
		}
		boxShader.setMat4("mvp", boxM * V * P);
		cube.Draw(boxShader);
	}

	Matrix4 M;
	M.scale(Vector3(0.2f, 0.2f, 0.2f)).translate(modelPos);
	// render boxes
	modelShader.use();
	modelShader.setVec3("viewPos", camera.position);

	modelShader.setMat4("model", M);
	modelShader.setMat4("mvp", (M * V * P));

	modelShader.setInt("cubemap", 0);
	glActiveTexture(GL_TEXTURE0);
	if (useDynamicCubemap) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicCubemapID);
	} 
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextureID());
	}
	sphereModel.Draw(modelShader);

	skybox.Draw(V, P);
}

void CubemapReflectionsScene::DrawGUIOptions() {
	ImGui::Checkbox("Use dynamic cubemapping", &useDynamicCubemap);
	ImGui::Checkbox("Rotate cubes", &rotateCubes);
}

CubemapReflectionsScene::~CubemapReflectionsScene() {
	glDeleteFramebuffers(1, reflectionFBOs);
}
