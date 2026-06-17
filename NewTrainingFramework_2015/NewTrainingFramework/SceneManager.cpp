#include "stdafx.h"
#include "SceneManager.h"
#include "SceneObject.h"
#include "ResourceManager.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Fire.h"
#include "ReflectedObject.h"

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

				camera->perspectiveMatrix.SetPerspective(camera->fov, Globals::screenWidth / Globals::screenHeight, camera->nearPlane, camera->farPlane);
				camera->updateAxes();
				camera->updateWorldView();
				cameras.push_back(camera);
			}
		}
		if (strcmp(node->name(), "lights") == 0) {
			for (rapidxml::xml_node<>* lightNode = node->first_node("light"); lightNode; lightNode = lightNode->next_sibling()) {
				Light* light = new Light();
				light->id = std::stoi(lightNode->first_attribute("id")->value());
				std::string type = lightNode->first_attribute("type")->value();

				if (type == "point") {
					light->type = 0;
					light->position.x = std::stof(lightNode->first_node("position")->first_node("x")->value());
					light->position.y = std::stof(lightNode->first_node("position")->first_node("y")->value());
					light->position.z = std::stof(lightNode->first_node("position")->first_node("z")->value());
				}
				else if (type == "directional") {
					light->type = 1;
					light->direction.x = std::stof(lightNode->first_node("direction")->first_node("x")->value());
					light->direction.y = std::stof(lightNode->first_node("direction")->first_node("y")->value());
					light->direction.z = std::stof(lightNode->first_node("direction")->first_node("z")->value());
				}
				else if (type == "spot") {
					light->type = 2;
					light->position.x = std::stof(lightNode->first_node("position")->first_node("x")->value());
					light->position.y = std::stof(lightNode->first_node("position")->first_node("y")->value());
					light->position.z = std::stof(lightNode->first_node("position")->first_node("z")->value());
					light->direction.x = std::stof(lightNode->first_node("direction")->first_node("x")->value());
					light->direction.y = std::stof(lightNode->first_node("direction")->first_node("y")->value());
					light->direction.z = std::stof(lightNode->first_node("direction")->first_node("z")->value());
					light->spotCutoff = std::stof(lightNode->first_node("spotCutoff")->value());
					light->spotExponent = std::stof(lightNode->first_node("spotExponent")->value());
				}

				light->colorDiffuse.x = std::stof(lightNode->first_node("diffuse")->first_node("r")->value());
				light->colorDiffuse.y = std::stof(lightNode->first_node("diffuse")->first_node("g")->value());
				light->colorDiffuse.z = std::stof(lightNode->first_node("diffuse")->first_node("b")->value());

				light->colorSpecular.x = std::stof(lightNode->first_node("specular")->first_node("r")->value());
				light->colorSpecular.y = std::stof(lightNode->first_node("specular")->first_node("g")->value());
				light->colorSpecular.z = std::stof(lightNode->first_node("specular")->first_node("b")->value());

				light->specPower = std::stof(lightNode->first_node("specPower")->value());

				lights.push_back(light);
			}
		}
		if (strcmp(node->name(), "fog") == 0)
		{
			rapidxml::xml_node<>* colorNode = node->first_node("color");
			fogColor.x = std::stof(colorNode->first_node("r")->value());
			fogColor.y = std::stof(colorNode->first_node("g")->value());
			fogColor.z = std::stof(colorNode->first_node("b")->value());

			smallRadius = std::stof(node->first_node("smallRadius")->value());
			largeRadius = std::stof(node->first_node("largeRadius")->value());
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
				else if (so->type == "fire")
				{
					so->model = rm->loadModel(std::stoi(object->first_node("model")->value()));
					Fire* fire = new Fire(so);
					objects.insert(std::pair<int, SceneObject*>(fire->id, fire));
				}
				else if (so->type == "reflected")
				{
					so->model = rm->loadModel(std::stoi(object->first_node("model")->value()));
					ReflectedObject* reflected = new ReflectedObject(so);

					float reflectedAmmount = std::stof(object->first_node("reflection")->value());
					reflected->reflectedAmmount = reflectedAmmount;

					objects.insert(std::pair<int, SceneObject*>(reflected->id, reflected));
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
