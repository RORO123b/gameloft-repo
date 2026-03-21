#pragma once
#include "stdafx.h"
#include "../Utilities/Math.h"
#include "../Utilities/TGA.h"
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
	Texture() {
		tr = new TextureResource();
	}
	~Texture() {
		delete tr;
	}
	void Load() {
		int width, height, bpp;
		char* pixelArray = LoadTGA("../../NewResourcesPacket/Textures/Croco.tga", &width, &height, &bpp);

		if (pixelArray != NULL)
		{
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			GLenum format = (bpp == 24) ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixelArray);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);

			delete[] pixelArray;
		}
	}
};