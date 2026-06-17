#pragma once
#include "stdafx.h"
#include "SceneObject.h"

class ReflectedObject : public SceneObject {
public:
	float reflectedAmmount = 0.5;
	ReflectedObject(SceneObject* so);
	void sendSpecificData(ESContext* esContext) override;
	void Update(float deltaTime) override;
};