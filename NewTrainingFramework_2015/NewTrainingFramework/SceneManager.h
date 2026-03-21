#pragma once
#include "Camera.h"
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
	void Draw();
	void Update(float deltaTime);
	~SceneManager()
};
