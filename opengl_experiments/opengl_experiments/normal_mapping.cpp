#include "normal_mapping.h"

void NormalMappingScene::Draw(Camera & camera, const Matrix4 & P) {
	Matrix4 V = camera.getViewMatrix();
	// render normal-mapped quad
	Matrix4 M;
	diffuseColorShader.use();
	lightPos.x = cos(glfwGetTime() * 2.0f);
	lightPos.y = sin(glfwGetTime() * 2.0f);
	M.scale(0.05f).translate(lightPos);
	diffuseColorShader.setVec3("color", Vector3(1.0f, 1.0f, 1.0f));
	diffuseColorShader.setMat4("mvp", M * V * P);
	pointLightModel.Draw(diffuseColorShader);

	M = Matrix4();
	normalMapShader.use();
	normalMapShader.setMat4("model", M);
	normalMapShader.setMat4("mvp", M * V * P);
	normalMapShader.setVec3("color", 0.5f, 0.5f, 0.5f);
	normalMapShader.setVec3("viewPos", camera.position);
	normalMapShader.setVec3("lightPos", lightPos);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, normalMap.id);
	RenderQuad();
}

void NormalMappingScene::DrawGUIOptions() {
}

void NormalMappingScene::RenderQuad() {
	if (quadVAO == 0) {
		// positions
		Vector3 pos1(-1.0f, 1.0f, 0.0f);
		Vector3 pos2(-1.0f, -1.0f, 0.0f);
		Vector3 pos3(1.0f, -1.0f, 0.0f);
		Vector3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		Vector2 uv1(0.0f, 1.0f);
		Vector2 uv2(0.0f, 0.0f);
		Vector2 uv3(1.0f, 0.0f);
		Vector2 uv4(1.0f, 1.0f);
		// normal vector
		Vector3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		Vector3 tangent1, bitangent1;
		Vector3 tangent2, bitangent2;
		// triangle 1
		// ----------
		Vector3 edge1 = pos2 - pos1;
		Vector3 edge2 = pos3 - pos1;
		Vector2 deltaUV1 = uv2 - uv1;
		Vector2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = tangent1.normalized();

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = bitangent1.normalized();

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = tangent2.normalized();


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = bitangent2.normalized();


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
