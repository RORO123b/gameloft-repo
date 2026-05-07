#include "stdafx.h"
#include "Texture.h"
#include "../Utilities/Math.h"
#include "../Utilities/TGA.h"

Texture::Texture() {
	tr = new TextureResource();
}

Texture::~Texture() {
	delete tr;
}

void Texture::Load()
{
	int width, height, bpp;
	char* pixelArray;

	pixelArray = LoadTGA((tr->file).c_str(), &width, &height, &bpp);

	GLenum textureType = (tr->type == "cube") ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	glGenTextures(1, &textureId);
	glBindTexture(textureType, textureId);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, tr->wrap_t);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, tr->mag_filter);

	if (textureType == GL_TEXTURE_CUBE_MAP) {
		int bytesPerPixel = bpp / 8;
		int faceWidth = width / 4;
		int faceHeight = height / 3;

		int xOffsets[6] = { 2, 0, 1, 1, 1, 3 };
		int yOffsets[6] = { 1, 1, 0, 2, 1, 1 };

		for (int face = 0; face < 6; face++) {
			int xOffset = xOffsets[face] * faceWidth;
			int yOffset = yOffsets[face] * faceHeight;
			char* subBuffer = new char[faceWidth * faceHeight * bytesPerPixel];
			for (int row = 0; row < faceHeight; row++) {
				memcpy(
					subBuffer + row * faceWidth * bytesPerPixel,
					pixelArray + ((yOffset + row) * width + xOffset) * bytesPerPixel,
					faceWidth * bytesPerPixel
				);
			}
			GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
			GLenum format = (bytesPerPixel == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(faceTarget, 0, format, faceWidth,
				faceHeight, 0, format, GL_UNSIGNED_BYTE, subBuffer);
			delete[] subBuffer;
		}
	}
	else {
		if (bpp == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);
	}
}
