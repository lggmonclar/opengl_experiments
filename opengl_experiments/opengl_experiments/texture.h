#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <stb_image.h>

class Texture {
public:
	unsigned int id;
	std::string type;
	std::string path;
	Texture(const char * path, const std::string & directory);
	~Texture();
};
