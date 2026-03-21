#pragma once
#include <map>
#include "Shader.cpp"
#include "Texture.cpp"
#include "Model.cpp"

class ResourceManager
{
private:
    static ResourceManager* spInstance;
    ResourceManager();

public:
    void Init();
    static ResourceManager* getInstance();
    void freeResources();
    ~ResourceManager();
    
    std::map<int, ShaderResource*> shaderResources;
    std::map<int, TextureResource*> textureResources;
	std::map<int, ModelResource*> modelResources;

    std::map<int, Shader*> shaders;
    std::map<int, Texture*> textures;
    std::map<int, Model*> models;

    Model* loadModel(int id);
    Texture* loadTexture(int id);
    Shader* loadShader(int id);
};


