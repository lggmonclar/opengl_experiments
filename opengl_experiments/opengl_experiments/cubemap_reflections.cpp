#include "cubemap_reflections.h"

CubemapReflectionsScene::CubemapReflectionsScene() {

}

void CubemapReflectionsScene::Draw(const Matrix4& V, const Matrix4& P) {
	lightBoxShader.use();
	Matrix4 lightM;
	lightM.scale(0.2f, 0.2f, 0.2f).translate(pointLightPosition).rotateY(glfwGetTime());
	lightBoxShader.setMat4("mvp", lightM * V * P);
	cube.Draw(lightBoxShader);

	Matrix4 M;
	M.scale(0.2f, 0.2f, 0.2f).translate(0.0f, -1.75f, 0.0f);
	// render boxes
	modelShader.use();
	modelShader.setVec3("viewPos", V.translation());

	modelShader.setMat4("model", M);
	modelShader.setMat4("mvp", (M * V * P));

	modelShader.setInt("skybox", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextureID());
	ourModel.Draw(modelShader);


	skybox.Draw(V, P);
}

void CubemapReflectionsScene::DrawGUIOptions() {
	ImGui::TextWrapped("Sup.");
}
