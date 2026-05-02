#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <string>
#include <vector>
#include "Vertex.h"
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
	void ReadNfg(const char* filename, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);
};
