#pragma once
#include "stdafx.h"
#include "../Utilities/Math.h"
#include <vector>
#include <string>

class Model;
class Shader;
class Texture;

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
    std::string type;
    std::string name;
    int depth_test;

    virtual float getKspec() const;
    virtual float getKdiff() const;

    void Draw(ESContext* esContext);
    void sendCommonData(ESContext* esContext);
    void sendSpecificData(ESContext* esContext);
    void Update(float deltaTime);
};