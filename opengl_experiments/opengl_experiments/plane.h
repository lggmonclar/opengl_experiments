#pragma once
#include "mesh.h"
class Plane : public Mesh {
public:
	Plane() : Plane(vector<Texture>{}){}
	Plane(vector<Texture> textures) : Mesh(
		vector<Vertex>{
			Vertex(Vector3(25.0f, -0.5f, 25.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(25.0f, 0.0f)),
			Vertex(Vector3(-25.0f, -0.5f, 25.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector3(-25.0f, -0.5f, -25.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 25.0f)),

			Vertex(Vector3(25.0f, -0.5f, 25.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(25.0f, 0.0f)),
			Vertex(Vector3(-25.0f, -0.5f, -25.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 25.0f)),
			Vertex(Vector3(25.0f, -0.5f, -25.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(25.0f, 25.0f)),
		},
		vector<unsigned int>{0,1,2,3,4,5},
		textures){}
};