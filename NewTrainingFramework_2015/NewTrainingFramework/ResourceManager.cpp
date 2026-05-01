#include "stdafx.h"
#include "ResourceManager.h"
//// 1. Include standard memory and exception headers first
//#include <memory>
//#include <exception>
//#include <stdexcept>
//
//// 2. This macro tells RapidXML to use standard C++ exceptions 
//// instead of trying to define its own weird memory handlers.
//#define RAPIDXML_NO_EXCEPTIONS
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

ResourceManager* ResourceManager::spInstance = NULL;


void ResourceManager::Init()
{
	rapidxml::file<> file("../../XML/resourceManager.xml");
	char* buffer = new char[file.size() + 1];
	std::memcpy(buffer, file.data(), file.size());
	buffer[file.size()] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);
	rapidxml::xml_node<>* root = doc.first_node("resourceManager");
	char Res[] = "../../NewResourcesPacket";
	for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
		if (strcmp(node->name(), "models") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("folder"); folder; folder = folder->next_sibling())
			{
				std::string path = folder->first_attribute("path")->value();
				rapidxml::xml_node<>* model = folder->first_node("model");
				while (model)
				{
					ModelResource* mr = new ModelResource();
					mr->file = Res + path.substr(path.find('/')) + model->first_node("file")->value();
					modelResources.insert(std::pair<int, ModelResource*>(std::stoi(model->first_attribute("id")->value()), mr));
					model = model->next_sibling();
				}
			}
		}
		else if (strcmp(node->name(), "textures") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("folder"); folder; folder = folder->next_sibling())
			{
				std::string path = folder->first_attribute("path")->value();
				rapidxml::xml_node<>* texture = folder->first_node("texture");
				while (texture)
				{
					TextureResource* tr = new TextureResource();

					tr->file = Res + path.substr(path.find('/')) + texture->first_node("file")->value();

					if (strcmp(texture->first_node("min_filter")->value(), "LINEAR") == 0)
					{
						tr->min_filter = GL_LINEAR;
					}
					else if (strcmp(texture->first_node("min_filter")->value(), "NEAREST") == 0)
					{
						tr->min_filter = GL_NEAREST;
					}

					if (strcmp(texture->first_node("mag_filter")->value(), "LINEAR") == 0)
					{
						tr->mag_filter = GL_LINEAR;
					}
					else if (strcmp(texture->first_node("mag_filter")->value(), "NEAREST") == 0)
					{
						tr->mag_filter = GL_NEAREST;
					}

					if (strcmp(texture->first_node("wrap_s")->value(), "CLAMP_TO_EDGE") == 0)
					{
						tr->wrap_s = GL_CLAMP_TO_EDGE;
					}
					else if (strcmp(texture->first_node("wrap_s")->value(), "REPEAT") == 0)
					{
						tr->wrap_s = GL_REPEAT;
					}

					if (strcmp(texture->first_node("wrap_t")->value(), "CLAMP_TO_EDGE") == 0)
					{
						tr->wrap_t = GL_CLAMP_TO_EDGE;
					}
					else if (strcmp(texture->first_node("wrap_t")->value(), "REPEAT") == 0)
					{
						tr->wrap_t = GL_REPEAT;
					}

					if (texture->first_node("type"))
					{
						tr->type = texture->first_node("type")->value();
					}

					textureResources.insert(std::pair<int, TextureResource*>(std::stoi(texture->first_attribute("id")->value()), tr));
					texture = texture->next_sibling();
				}
			}
		}
		else if (strcmp(node->name(), "shaders") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("folder"); folder; folder = folder->next_sibling())
			{
				std::string path = folder->first_attribute("path")->value();
				rapidxml::xml_node<>* shader = folder->first_node("shader");
				while (shader)
				{
					ShaderResource* sr = new ShaderResource();
					sr->fileVS = Res + path.substr(path.find('/')) + shader->first_node("vs")->value();
					sr->fileFS = Res + path.substr(path.find('/')) + shader->first_node("fs")->value();
					shaderResources.insert(std::pair<int, ShaderResource*>(std::stoi(shader->first_attribute("id")->value()), sr));

					shader = shader->next_sibling();
				}
			}
		}
	}
	delete[] buffer;
}

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
