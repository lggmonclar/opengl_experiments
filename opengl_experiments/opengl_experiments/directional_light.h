#pragma once
#include <math/math.h>

class DirectionalLight {
public:
	DirectionalLight(Vector3 pos, Vector3 dir);
	Vector3 position;
	Vector3 direction;
};