#pragma once
#include "vector3.h"

struct Matrix3 {
private:
	float v[15];

public:
	operator float*() {
		return v;
	}

	operator const float*() const {
		return v;
	}

	Matrix3() : 
		v { 
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		}
	{}

	Matrix3(float v00, float v01, float v02,
			float v10, float v11, float v12,
			float v20, float v21, float v22) : 
		v { 
			v00, v01, v02,
			v10, v11, v12,
			v20, v21, v22
		}
	{}

	Matrix3(const Vector3& i, const Vector3& j, const Vector3& k) : 
		v { 
			i.x, i.y, i.z,
			j.x, j.y, j.z,
			k.x, k.y, k.z
		}
	{}
};