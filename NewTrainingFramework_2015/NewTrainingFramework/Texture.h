#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <string>


struct TextureResource {
	std::string file;
	std::string type;
	int min_filter, mag_filter, wrap_s, wrap_t;
};

class Texture
{
public:
	TextureResource* tr;
	GLuint textureId;
	
	Texture();
	~Texture();
	void Load();
};
