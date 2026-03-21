#pragma once
#include "stdafx.h"
#include "../Utilities/Math.h"
#include <string>
#include <vector>
#include "Vertex.h"

void ReadNfg(const char* filename, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices)
{
	FILE* f = fopen(filename, "r");

	int numVertices = 0;
	fscanf(f, "NrVertices: %d", &numVertices);

	for (int i = 0; i < numVertices; ++i)
	{
		Vertex v;
		int index;

		fscanf(f, " %d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",
			&index,
			&v.pos.x, &v.pos.y, &v.pos.z,
			&v.norm.x, &v.norm.y, &v.norm.z,
			&v.binorm.x, &v.binorm.y, &v.binorm.z,
			&v.tgt.x, &v.tgt.y, &v.tgt.z,
			&v.uv.x, &v.uv.y
		);

		v.color = Vector3(1.0f, 1.0f, 1.0f);

		vertices.push_back(v);
	}

	int numTriangles = 0;
	fscanf(f, " NrIndices: %d", &numTriangles);
	numTriangles /= 3;

	for (int i = 0; i < numTriangles; ++i)
	{
		int index;
		unsigned short idx1, idx2, idx3;

		fscanf(f, " %d. %hu, %hu, %hu", &index, &idx1, &idx2, &idx3);

		indices.push_back(idx1);
		indices.push_back(idx2);
		indices.push_back(idx3);
	}

	fclose(f);
}


struct ModelResource {
	std::string file;
};
class Model
{
public:
	ModelResource* mr;
	GLuint iboId, wiredIboId, vboId;
	GLsizei indexCount, wiredIndexCount;

	Model() {
		mr = new ModelResource();
	}
	~Model() {
		delete mr;
	}

	void Load() {
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		ReadNfg(mr->file.c_str(), vertices, indices);

		this->indexCount = vertices.size();
		int vertexCount = indices.size();

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};