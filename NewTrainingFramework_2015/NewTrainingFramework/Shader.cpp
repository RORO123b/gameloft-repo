#pragma once
#include "stdafx.h"
#include "../Utilities/Math.h"
#include <string>

struct ShaderResource {
	std::string fileVS, fileFS;
};

class Shader
{
public:
	ShaderResource* sr;
	GLuint program, vertexShader, fragmentShader;
	GLint a_posL, a_color, uMVP, v_uv;
	Shader() {
		sr = new ShaderResource();
	}
	~Shader() {
		delete sr;
		glDeleteProgram(program);
	}
	void Load() {
		vertexShader = esLoadShader(GL_VERTEX_SHADER, (char*)sr->fileVS.c_str());
		fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, (char*)sr->fileFS.c_str());

		program = esLoadProgram(vertexShader, fragmentShader);

		a_posL = glGetAttribLocation(program, "a_posL");
		a_color = glGetAttribLocation(program, "a_color");
		uMVP = glGetUniformLocation(program, "uMVP");
		v_uv = glGetAttribLocation(program, "v_uv");
	}
};