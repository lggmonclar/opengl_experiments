#pragma once
#include <iostream>
#include <iomanip>
#include "vector3.h"
#include "vector4.h"

struct Matrix4 {
private:
	float v[16];

public:
	operator float*() {
		return v;
	}

	operator const float*() const {
		return v;
	}

	Matrix4() :
		v{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}
	{}

	Matrix4(float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23,
		float v30, float v31, float v32, float v33) :
		v{
		v00, v01, v02, v03,
		v10, v11, v12, v13,
		v20, v21, v22, v23,
		v30, v31, v32, v33,
	}
	{}

	Matrix4(const Vector4& i, const Vector4& j, const Vector4& k, const Vector4 &l) :
		v{
		i.x, i.y, i.z, i.w,
		j.x, j.y, j.z, j.w,
		k.x, k.y, k.z, k.w,
		l.x, l.y, l.z, l.w,
	}
	{}

	void printMatrix() {
		const char separator = ' ';

		for (int i = 0; i < 16; i++) {
			if (i % 4 == 0) {
				std::cout << std::endl;
			}
			std::cout << std::left << std::setw(10) << std::setfill(separator) << v[i];
		}
		std::cout << std::endl;
	}

	const float& operator ()(int i, int j) const {
		return v[i * 4 + j];
	}

	float& operator ()(int i, int j) {
		return v[i * 4 + j];
	}

	Matrix4 operator *(const Matrix4& N) {
		Matrix4 M = *this;
		return Matrix4(
			M(0, 0) * N(0, 0) + M(0, 1) * N(1, 0) + M(0, 2) * N(2, 0) + M(0, 3) * N(3, 0),
			M(0, 0) * N(0, 1) + M(0, 1) * N(1, 1) + M(0, 2) * N(2, 1) + M(0, 3) * N(3, 1),
			M(0, 0) * N(0, 2) + M(0, 1) * N(1, 2) + M(0, 2) * N(2, 2) + M(0, 3) * N(3, 2),
			M(0, 0) * N(0, 3) + M(0, 1) * N(1, 3) + M(0, 2) * N(2, 3) + M(0, 3) * N(3, 3),

			M(1, 0) * N(0, 0) + M(1, 1) * N(1, 0) + M(1, 2) * N(2, 0) + M(1, 3) * N(3, 0),
			M(1, 0) * N(0, 1) + M(1, 1) * N(1, 1) + M(1, 2) * N(2, 1) + M(1, 3) * N(3, 1),
			M(1, 0) * N(0, 2) + M(1, 1) * N(1, 2) + M(1, 2) * N(2, 2) + M(1, 3) * N(3, 2),
			M(1, 0) * N(0, 3) + M(1, 1) * N(1, 3) + M(1, 2) * N(2, 3) + M(1, 3) * N(3, 3),

			M(2, 0) * N(0, 0) + M(2, 1) * N(1, 0) + M(2, 2) * N(2, 0) + M(2, 3) * N(3, 0),
			M(2, 0) * N(0, 1) + M(2, 1) * N(1, 1) + M(2, 2) * N(2, 1) + M(2, 3) * N(3, 1),
			M(2, 0) * N(0, 2) + M(2, 1) * N(1, 2) + M(2, 2) * N(2, 2) + M(2, 3) * N(3, 2),
			M(2, 0) * N(0, 3) + M(2, 1) * N(1, 3) + M(2, 2) * N(2, 3) + M(2, 3) * N(3, 3),

			M(3, 0) * N(0, 0) + M(3, 1) * N(1, 0) + M(3, 2) * N(2, 0) + M(3, 3) * N(3, 0),
			M(3, 0) * N(0, 1) + M(3, 1) * N(1, 1) + M(3, 2) * N(2, 1) + M(3, 3) * N(3, 1),
			M(3, 0) * N(0, 2) + M(3, 1) * N(1, 2) + M(3, 2) * N(2, 2) + M(3, 3) * N(3, 2),
			M(3, 0) * N(0, 3) + M(3, 1) * N(1, 3) + M(3, 2) * N(2, 3) + M(3, 3) * N(3, 3)
		);
	}

	Matrix4& operator *=(const Matrix4& N) {
		Matrix4 M = *this;

		v[0] = M(0, 0) * N(0, 0) + M(0, 1) * N(1, 0) + M(0, 2) * N(2, 0) + M(0, 3) * N(3, 0);
		v[1] = M(0, 0) * N(0, 1) + M(0, 1) * N(1, 1) + M(0, 2) * N(2, 1) + M(0, 3) * N(3, 1);
		v[2] = M(0, 0) * N(0, 2) + M(0, 1) * N(1, 2) + M(0, 2) * N(2, 2) + M(0, 3) * N(3, 2);
		v[3] = M(0, 0) * N(0, 3) + M(0, 1) * N(1, 3) + M(0, 2) * N(2, 3) + M(0, 3) * N(3, 3);

		v[4] = M(1, 0) * N(0, 0) + M(1, 1) * N(1, 0) + M(1, 2) * N(2, 0) + M(1, 3) * N(3, 0);
		v[5] = M(1, 0) * N(0, 1) + M(1, 1) * N(1, 1) + M(1, 2) * N(2, 1) + M(1, 3) * N(3, 1);
		v[6] = M(1, 0) * N(0, 2) + M(1, 1) * N(1, 2) + M(1, 2) * N(2, 2) + M(1, 3) * N(3, 2);
		v[7] = M(1, 0) * N(0, 3) + M(1, 1) * N(1, 3) + M(1, 2) * N(2, 3) + M(1, 3) * N(3, 3);

		v[8] = M(2, 0) * N(0, 0) + M(2, 1) * N(1, 0) + M(2, 2) * N(2, 0) + M(2, 3) * N(3, 0);
		v[9] = M(2, 0) * N(0, 1) + M(2, 1) * N(1, 1) + M(2, 2) * N(2, 1) + M(2, 3) * N(3, 1);
		v[10] = M(2, 0) * N(0, 2) + M(2, 1) * N(1, 2) + M(2, 2) * N(2, 2) + M(2, 3) * N(3, 2);
		v[11] = M(2, 0) * N(0, 3) + M(2, 1) * N(1, 3) + M(2, 2) * N(2, 3) + M(2, 3) * N(3, 3);

		v[12] = M(3, 0) * N(0, 0) + M(3, 1) * N(1, 0) + M(3, 2) * N(2, 0) + M(3, 3) * N(3, 0);
		v[13] = M(3, 0) * N(0, 1) + M(3, 1) * N(1, 1) + M(3, 2) * N(2, 1) + M(3, 3) * N(3, 1);
		v[14] = M(3, 0) * N(0, 2) + M(3, 1) * N(1, 2) + M(3, 2) * N(2, 2) + M(3, 3) * N(3, 2);
		v[15] = M(3, 0) * N(0, 3) + M(3, 1) * N(1, 3) + M(3, 2) * N(2, 3) + M(3, 3) * N(3, 3);

		return *this;
	}

	Vector4 operator *(const Vector4& v) {
		Matrix4 M = *this;
		return Vector4(
			M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z + M(0, 3) * v.w,
			M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z + M(1, 3) * v.w,
			M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z + M(2, 3) * v.w,
			M(3, 0) * v.x + M(3, 1) * v.y + M(3, 2) * v.z + M(3, 3) * v.w
		);
	}

	Matrix4& scale(float x, float y, float z) {
		return *this *= Matrix4(
			x, 0.0f, 0.0f, 0.0f,
			0.0f, y, 0.0f, 0.0f,
			0.0f, 0.0f, z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Matrix4& scale(float val) {
		return scale(val, val, val);
	}

	Matrix4& scale(const Vector3& v) {
		return scale(v.x, v.y, v.z);
	}

	Matrix4& reflectX() {
		return scale(-1.0f, 1.0f, 1.0f);
	}

	Matrix4& reflectY() {
		return scale(1.0f, -1.0f, 1.0f);
	}

	Matrix4& reflectZ() {
		return scale(1.0f, 1.0f, -1.0f);
	}

	Matrix4& rotateX(float ang) {
		float c = cos(ang);
		float s = sin(ang);

		return *this *= Matrix4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, c, -s, 0.0f,
			0.0f, s, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Matrix4& rotateY(float ang) {
		float c = cos(ang);
		float s = sin(ang);

		return *this *= Matrix4(
			c, 0.0f, s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-s, 0.0f, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Matrix4& rotateZ(float ang) {
		float c = cos(ang);
		float s = sin(ang);

		*this *= Matrix4(
			c, -s, 0.0f, 0.0f,
			s, c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return *this;
	}

	Matrix4& rotateAbout(float ang, const Vector3& axis) {
		float c = cos(ang);
		float s = sin(ang);
		Vector3 ax = axis.normalized();

		return *this *= Matrix4(
			ax.x * ax.x * (1 - c) + c, ax.x * ax.y * (1 - c) + ax.z * s, ax.x * ax.z * (1 - c) - ax.y * s, 0.0F,

			ax.x * ax.y * (1 - c) - ax.z * s, ax.y * ax.y * (1 - c) + c, ax.y * ax.z * (1 - c) + ax.x * s, 0.0F,

			ax.x * ax.z * (1 - c) + ax.y * s, ax.y * ax.z * (1 - c) - ax.x * s, ax.z * ax.z * (1 - c) + c, 0.0F,

			0.0F, 0.0F, 0.0F, 1.0F
		);
	}

	Matrix4& translate(float x, float y, float z) {
		return *this *= Matrix4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f
		);
	}

	Matrix4& translate(const Vector3& v) {
		return translate(v.x, v.y, v.z);
	}

	Matrix4& projectOrtographically() {

	}

	static Matrix4 perspectiveProjection(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
		float tanHalfFov = tan(fieldOfView / 2.0f);

		float v11 = 1.0f / (aspectRatio * tanHalfFov);
		float v22 = 1.0f / tanHalfFov;
		float v33 = -(farPlane + nearPlane) / (farPlane - nearPlane);
		float v34 = -1.0f;
		float v43 = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);

		return Matrix4(
			v11, 0.0f, 0.0f, 0.0f,
			0.0f, v22, 0.0f, 0.0f,
			0.0f, 0.0f, v33, v34,
			0.0f, 0.0f, v43, 0.0f
		);
	}

	Matrix4& lookAt(const Vector3 &position, const Vector3 &target, const Vector3 &worldUp) {
		Vector3 zAxis = (position - target).normalized();
		Vector3 xAxis = worldUp.cross(zAxis).normalized();
		Vector3 yAxis = zAxis.cross(xAxis);

		v[0] = xAxis.x;
		v[1] = yAxis.x;
		v[2] = zAxis.x;

		v[4] = xAxis.y;
		v[5] = yAxis.y;
		v[6] = zAxis.y;

		v[8] = xAxis.z;
		v[9] = yAxis.z;
		v[10] = zAxis.z;

		v[12] = -(xAxis.dot(position));
		v[13] = -(yAxis.dot(position));
		v[14] = -(zAxis.dot(position));

		return *this;
	}

	Vector3 translation() {
		return Vector3(v[12], v[13], v[14]);
	}

	Vector3 translation() const {
		return Vector3(v[12], v[13], v[14]);
	}
};