#pragma once
#include "../Utilities/utilities.h"
#include <vector>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <string>


class SceneObject
{
public:

	int id;
	Model* model;
	Shader* shader;
	std::vector<Texture*> textures;
	Vector3 position, rotation, scale;
	std::string type, name;
	bool depth_test;
	
	virtual float getKspec() const;
	virtual float getKdiff() const;
	void Draw(ESContext* esContext);

	virtual void sendCommonData(ESContext* esContext);

	virtual void sendSpecificData(ESContext* esContext);

	virtual void Update(float deltaTime);

	SceneObject() {};

	virtual ~SceneObject() {};
};
