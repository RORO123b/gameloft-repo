#pragma once
#include "Camera.h"
#include <map>
#include <vector>
#ifdef new
#pragma message("WARNING: 'new' is a macro here!")
#endif
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "Light.h"
#include "Globals.h"


class SceneObject;

class SceneManager
{
private:
	static SceneManager* spInstance;
	SceneManager();
public:
	Camera myCamera;

	void InitWindow();
	void Init();
	static SceneManager* getInstance();
	void Draw(ESContext* esContext);
	void Update(float deltaTime);
	~SceneManager();

	std::map<int, SceneObject*> objects;
	std::vector<Camera*> cameras;
	std::vector<Light*> lights;

	Vector3 ambientColor = Vector3(0.2f, 0.2f, 0.2f);
	float ambientRatio = 0.5f;

	Camera* getActiveCamera();
	int activeCamera = 0;
	float totalTime;
	Vector3 fogColor;
	float smallRadius, largeRadius;
};
