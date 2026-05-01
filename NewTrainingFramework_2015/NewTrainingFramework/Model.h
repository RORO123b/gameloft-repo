#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <string>

struct ModelResource {
	std::string file;
};

class Model
{
public:
	ModelResource* mr;
	GLuint iboId, wiredIboId, vboId;
	GLsizei indexCount, wiredIndexCount;

	Model();
	~Model();
	void Load();
};
