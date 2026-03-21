#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::spInstance = NULL;


ResourceManager* ResourceManager::getInstance()
{
    if (!spInstance)
    {
        spInstance = new ResourceManager();
    }
    return spInstance;

}

Model* ResourceManager::loadModel(int id) {
    if (this->models.count(id)) {
        return models[id];
    }
    
    Model* newModel = new Model();
    newModel->mr = modelResources[id];
    newModel->Load();
    models.insert({ id, newModel });
    return newModel;
}

Texture* ResourceManager::loadTexture(int id) {
    if (this->textures.count(id))
        return textures[id];

    Texture* newTexture = new Texture();
    newTexture->tr = textureResources[id];
    newTexture->Load();
    textures.insert({ id, newTexture });
    return newTexture;
}

Shader* ResourceManager::loadShader(int id) {
    if (this->shaders.count(id))
        return shaders[id];

    Shader* newshader = new Shader();
    newshader->sr = shaderResources[id];
    newshader->Load();
    shaders.insert({ id, newshader });
    return newshader;
}
