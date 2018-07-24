#pragma once
#include <math.h>

struct Vector3 {
	float x, y, z;

	Vector3() = default;
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

	void print() {
		std::cout << "[" << x << ", " << y << ", " << z << "]";
		std::cout << std::endl;
	}

	operator float*() {
		return reinterpret_cast<float*>(this);
	}

	operator const float*() const {
		return reinterpret_cast<const float*>(this);
	}

	Vector3 operator +(const Vector3& v3) {
		return Vector3(x + v3.x, y + v3.y, z + v3.z);
	}

	Vector3 operator +(const Vector3& v3) const {
		return Vector3(x + v3.x, y + v3.y, z + v3.z);
	}

	Vector3 operator +=(const Vector3& v3) {
		x += v3.x;
		y += v3.y;
		z += v3.z;
		return *this;
	}

	Vector3 operator -() {
		return Vector3(-x, -y, -z);
	}

	Vector3 operator -(const Vector3& v3) {
		return Vector3(x - v3.x, y - v3.y, z - v3.z);
	}

	Vector3 operator -(const Vector3& v3) const {
		return Vector3(x - v3.x, y - v3.y, z - v3.z);
	}

	Vector3 operator -=(const Vector3& v3) {
		x -= v3.x;
		y -= v3.y;
		z -= v3.z;
		return *this;
	}

	Vector3 operator *(float s) {
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 operator *(float s) const {
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 operator *=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3 operator /(float s) {
		return Vector3(x / s, y / s, z / s);
	}

	Vector3 operator /(float s) const {
		return Vector3(x / s, y / s, z / s);
	}

	Vector3 operator /=(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	double dot(Vector3 &v3) {
		return x * v3.x + y * v3.y + z * v3.z;
	}

	double dot(const Vector3 &v3) const {
		return x * v3.x + y * v3.y + z * v3.z;
	}

	Vector3 cross(Vector3 &v3) {
		return Vector3(
			y * v3.z - z * v3.y,
			z * v3.x - x * v3.z,
			x * v3.y - y * v3.x
		);
	}

	Vector3 cross(Vector3 &v3) const {
		return Vector3(
			y * v3.z - z * v3.y,
			z * v3.x - x * v3.z,
			x * v3.y - y * v3.x
		);
	}

	double magnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	const double magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 normalized() {
		return *this / magnitude();
	}	
	
	const Vector3 normalized() const {
		return *this / magnitude();
	}
};