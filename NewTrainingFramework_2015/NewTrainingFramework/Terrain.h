#pragma once
#include "stdafx.h"
#include "SceneObject.h"

class Model;
class Shader;

class Terrain : public SceneObject {
public:
	const int nrCells = 100, dimCell = 10, offsetY = -100;
	virtual float getKspec() const override { return 0.1f; }
	virtual float getKdiff() const override { return 1.0f; }
	Vector3 terrainHeights;
	Vector2 uvOffset;
	void generateModel();
	void sendSpecificData(ESContext* esContext) override;
	void Update(float deltaTime) override;
	Terrain(SceneObject* so);
};

