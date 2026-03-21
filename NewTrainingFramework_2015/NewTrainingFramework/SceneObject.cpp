#pragma once
#include "../Utilities/Math.h"
#include "Model.cpp"
#include "Shader.cpp"
#include "Texture.cpp"
#include <vector>

class SceneObject
{
public:
	int id;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Model* model;
	Shader* shader;
	std::vector<Texture*> textures;
	int depthTest;

};