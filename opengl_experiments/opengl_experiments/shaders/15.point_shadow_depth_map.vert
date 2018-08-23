#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 FragPos;

uniform mat4 model;
uniform mat4 mvp;

void main() {
	FragPos = model * vec4(aPos, 1.0);
    gl_Position = mvp * vec4(aPos, 1.0);
}