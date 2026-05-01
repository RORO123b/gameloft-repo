#include "stdafx.h"
#include "Shader.h"
#include "../Utilities/Math.h"

Shader::Shader() {
	sr = new ShaderResource();
}

Shader::~Shader() {
	delete sr;
	glDeleteProgram(program);
}

void Shader::Load()
{
	char* charFileVS = new char;
	char* charFileFS = new char;
	charFileVS = (char*)(sr->fileVS.c_str());
	charFileFS = (char*)(sr->fileFS.c_str());

	vertexShader = esLoadShader(GL_VERTEX_SHADER, charFileVS);

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, charFileFS);

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	matrixUniform = glGetUniformLocation(program, "u_rotation");
	positionAttribute = glGetAttribLocation(program, "a_posL");
	heightUniform = glGetUniformLocation(program, "u_height");
	colorAttribute = glGetAttribLocation(program, "a_color");
	uv2Attribute = glGetAttribLocation(program, "a_uv2");
	uvAttribute = glGetAttribLocation(program, "a_uv");

	smallRadiusUniform = glGetUniformLocation(program, "u_smallRadius");
	largeRadiusUniform = glGetUniformLocation(program, "u_largeRadius");
	cameraPosUniform = glGetUniformLocation(program, "u_cameraPos");
	fogColorUniform = glGetUniformLocation(program, "u_fogColor");

	modelMatrixUniform = glGetUniformLocation(program, "u_model");
	normalAttribute = glGetAttribLocation(program, "u_normal");

	reflectionUniform = glGetUniformLocation(program, "u_reflection");

	for (int i = 0; i < MAX_TEXTURES; i++) {
		std::string uniformName = "u_texture_" + std::to_string(i);
		textureUniform[i] = glGetUniformLocation(program, uniformName.c_str());
	}

	MVP = glGetUniformLocation(program, "uMVP");
	uvOffsetUniform = glGetUniformLocation(program, "u_uvOffset");
}