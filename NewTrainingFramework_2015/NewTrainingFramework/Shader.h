#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <string>

#define MAX_TEXTURES 5

struct ShaderResource {
	std::string fileVS, fileFS;
};

class Shader
{
public:
	ShaderResource* sr;
	GLuint program, vertexShader, fragmentShader;
	GLint positionAttribute, colorAttribute, matrixUniform, MVP;
	GLint textureUniform[MAX_TEXTURES], uvAttribute, uv2Attribute;
	GLint heightUniform, uvOffsetUniform;
	GLint fogColorUniform, smallRadiusUniform, largeRadiusUniform;
	GLint cameraPosUniform, modelMatrixUniform, normalAttribute;
	GLint reflectionUniform;

	Shader();
	~Shader();
	void Load();
};
