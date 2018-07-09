#pragma once
#include <math.h>

struct Vector4 {
	float x, y, z, w;

	Vector4() = default;
	Vector4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}

	operator float*() {
		return reinterpret_cast<float*>(this);
	}

	operator const float*() const {
		return reinterpret_cast<const float*>(this);
	}

	Vector4 operator +(const Vector4& v4) {
		return Vector4(x + v4.x, y + v4.y, z + v4.z, w + v4.w);
	}

	Vector4 operator +=(const Vector4& v4) {
		x += v4.x;
		y += v4.y;
		z += v4.z;
		w += v4.w;
		return *this;
	}

	Vector4 operator -(const Vector4& v4) {
		return Vector4(x - v4.x, y - v4.y, z - v4.z, w - v4.w);
	}

	Vector4 operator -=(const Vector4& v4) {
		x -= v4.x;
		y -= v4.y;
		z -= v4.z;
		w -= v4.w;
		return *this;
	}

	Vector4 operator *(float s) {
		return Vector4(x * s, y * s, z * s, w * s);
	}

	Vector4 operator *=(float s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	Vector4 operator /(float s) {
		return Vector4(x / s, y / s, z / s, w / s);
	}

	Vector4 operator /=(float s) {
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	float dot(Vector4 &v4) {
		return x * v4.x + y * v4.y + z * v4.z + w * v4.w;
	}

	float magnitude() {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	Vector4 normalized() {
		return *this / magnitude();
	}
};