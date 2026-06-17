#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include <ctime>

class Fire : public SceneObject {
public:
	float u_DispMax;
	float u_Time;
	clock_t lastClock;
	float speedFactor;
	Fire(SceneObject* so);
	void Update(float deltaTime) override;
	void sendSpecificData(ESContext* esContext) override;
};