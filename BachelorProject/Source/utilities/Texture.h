#pragma once

#include <string>
#include <gl/glew.h>
#include <stb_image/stb_image.h>
#include <Logger.h>

class Texture2D {
public:
	Texture2D(std::string path) : path{ path } {}
	void init(bool genMimMaps);
	void useAs(int num);
private:
	std::string path;
	unsigned int id;
};