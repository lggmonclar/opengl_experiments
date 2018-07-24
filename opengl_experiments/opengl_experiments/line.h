#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "math/vector3.h"
#include "math/matrix4.h"

class Line {
private:
	Vector3 startPos;
	Vector3 endPos;
	unsigned int VAO, VBO;
	Shader lineShader = Shader("shaders/line.vert", "shaders/line.frag");
public:
	Line(Vector3 _startPos, Vector3 _endPos, Vector3 _color) : startPos(_startPos), endPos(_endPos) {
		float vertices[] = {
			_startPos.x, _startPos.y, _startPos.z,
			_endPos.x, _endPos.y, _endPos.z
		};
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		lineShader.use();
		lineShader.setVec3("lineColor", _color);
	}

	void draw(Matrix4 view, Matrix4 projection) {
		glBindVertexArray(VAO);
		lineShader.use();
		lineShader.setMat4("mvp", view * projection);
		glDrawArrays(GL_LINES, 0, 2);
	}

	~Line() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
};