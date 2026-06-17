#pragma once
#include "../Utilities/Math.h"

class Light {
public:
	int id;
	int type; // POINT, DIRECTIONAL, SPOT
	Vector3 position;
	Vector3 direction;
	Vector3 colorDiffuse;
	Vector3 colorSpecular;
	float specPower;
	float spotCutoff;
	float spotExponent;
};