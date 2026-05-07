#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "Camera.h"
#include "SceneManager.h"

class SkyBox : public SceneObject
{
public:
	void Update(float deltaTime) override;
	SkyBox(SceneObject* so);
};

