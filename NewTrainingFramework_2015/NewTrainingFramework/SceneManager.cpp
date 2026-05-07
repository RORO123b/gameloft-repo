#include "stdafx.h"
#include "SceneManager.h"
#include "SceneObject.h"
#include "ResourceManager.h"
#include "Terrain.h"
#include "SkyBox.h"

SceneManager* SceneManager::spInstance = NULL;

SceneManager::SceneManager()
{

}


void SceneManager::Init()
{
	totalTime = 0.0f;

	ResourceManager* rm = ResourceManager::getInstance();
	rapidxml::file<> file("../../XML/sceneManager.xml");
	char* buffer = new char[file.size() + 1];
	std::memcpy(buffer, file.data(), file.size());
	buffer[file.size()] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);
	rapidxml::xml_node<>* root = doc.first_node("sceneManager");

	///
	Camera* camera = new Camera();
	cameras.push_back(camera);

	///

	for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling())
	{
		if (strcmp(node->name(), "cameras") == 0)
		{
			for (rapidxml::xml_node<>* camNode = node->first_node("camera"); camNode; camNode = camNode->next_sibling())
			{
				Camera* camera = new Camera();

				camera->position.x = std::stof(camNode->first_node("position")->first_node("x")->value());
				camera->position.y = std::stof(camNode->first_node("position")->first_node("y")->value());
				camera->position.z = std::stof(camNode->first_node("position")->first_node("z")->value());

				camera->target.x = std::stof(camNode->first_node("target")->first_node("x")->value());
				camera->target.y = std::stof(camNode->first_node("target")->first_node("y")->value());
				camera->target.z = std::stof(camNode->first_node("target")->first_node("z")->value());

				camera->up.x = std::stof(camNode->first_node("up")->first_node("x")->value());
				camera->up.y = std::stof(camNode->first_node("up")->first_node("y")->value());
				camera->up.z = std::stof(camNode->first_node("up")->first_node("z")->value());

				camera->moveSpeed = std::stof(camNode->first_node("translationSpeed")->value());
				camera->rotateSpeed = std::stof(camNode->first_node("rotationSpeed")->value());
				camera->fov = std::stof(camNode->first_node("fov")->value());
				camera->nearPlane = std::stof(camNode->first_node("near")->value());
				camera->farPlane = std::stof(camNode->first_node("far")->value());

				camera->updateWorldView();
				cameras.push_back(camera);
			}
		}
		if (strcmp(node->name(), "objects") == 0)
		{
			for (rapidxml::xml_node<>* object = node->first_node("object"); object; object = object->next_sibling())
			{
				SceneObject* so = new SceneObject();
				so->id = std::stoi(object->first_attribute("id")->value());
				so->position.x = std::stof(object->first_node("position")->first_node("x")->value());
				so->position.y = std::stof(object->first_node("position")->first_node("y")->value());
				so->position.z = std::stof(object->first_node("position")->first_node("z")->value());
				so->rotation.x = std::stof(object->first_node("rotation")->first_node("x")->value());
				so->rotation.y = std::stof(object->first_node("rotation")->first_node("y")->value());
				so->rotation.z = std::stof(object->first_node("rotation")->first_node("z")->value());
				so->scale.x = std::stof(object->first_node("scale")->first_node("x")->value());
				so->scale.y = std::stof(object->first_node("scale")->first_node("y")->value());
				so->scale.z = std::stof(object->first_node("scale")->first_node("z")->value());
				so->type = object->first_node("type")->value();
				so->name = object->first_node("name")->value();
				// so->depth_test = std::stoi(object->first_node("depth_test")->value());

				so->shader = rm->loadShader(std::stoi(object->first_node("shader")->value()));

				int k = 0;
				for (rapidxml::xml_node<>* texture = object->first_node("textures")->first_node("texture"); texture; texture = texture->next_sibling())
				{
					so->textures.push_back(rm->loadTexture(std::stoi(texture->first_attribute("id")->value())));
					k++;
				}

				if (so->type == "normal")
				{
					so->model = rm->loadModel(std::stoi(object->first_node("model")->value()));

					objects.insert(std::pair<int, SceneObject*>(so->id, so));
				}
				else if (so->type == "terrain")
				{
					Terrain* terrain = new Terrain(so);

					rapidxml::xml_node<>* heightsNode = object->first_node("heights");
					float heightR = std::stof(heightsNode->first_node("r")->value());
					float heightG = std::stof(heightsNode->first_node("g")->value());
					float heightB = std::stof(heightsNode->first_node("b")->value());
					terrain->terrainHeights = Vector3(heightR, heightG, heightB);

					objects.insert(std::pair<int, SceneObject*>(terrain->id, terrain));
				}
				else if (so->type == "skybox")
				{
					so->model = rm->loadModel(std::stoi(object->first_node("model")->value()));

					SkyBox* skybox = new SkyBox(so);
					objects.insert(std::pair<int, SceneObject*>(skybox->id, skybox));
				}
			}
		}
	}

}

Camera* SceneManager::getActiveCamera()
{
	return cameras[activeCamera];
}

void SceneManager::Draw(ESContext* esContext)
{
	std::map<int, SceneObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		it->second->Draw(esContext);
		it++;
	}
}

void SceneManager::Update(float deltaTime)
{
	Camera* camera = getActiveCamera();
	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime)
	{
		totalTime = totalTime - Globals::frameTime;

		camera->deltaTime = deltaTime;
		camera->updateWorldView();
	}
	std::map<int, SceneObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		it->second->Update(deltaTime);
		it++;
	}
}

SceneManager* SceneManager::getInstance()
{
	if (!spInstance)
	{
		spInstance = new SceneManager();
	}
	return spInstance;

}
