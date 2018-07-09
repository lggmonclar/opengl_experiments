#pragma once
#include <math.h>

struct Vector2 {
	float x, y;

	Vector2() = default;
	Vector2(float a, float b) : x(a), y(b) {}

	operator float*() {
		return reinterpret_cast<float*>(this);
	}

	operator const float*() const {
		return reinterpret_cast<const float*>(this);
	}

	Vector2 operator +(const Vector2& v2) {
		return Vector2(x + v2.x, y + v2.y);
	}

	Vector2 operator +=(const Vector2& v2) {
		x += v2.x;
		y += v2.y;
		return *this;
	}

	Vector2 operator -(const Vector2& v2) {
		return Vector2(x - v2.x, y - v2.y);
	}

	Vector2 operator -=(const Vector2& v2) {
		x -= v2.x;
		y -= v2.y;
		return *this;
	}

	Vector2 operator *(float s) {
		return Vector2(x * s, y * s);
	}

	Vector2 operator *=(float s) {
		x *= s;
		y *= s;
		return *this;
	}

	Vector2 operator /(float s) {
		return Vector2(x / s, y / s);
	}

	Vector2 operator /=(float s) {
		x /= s;
		y /= s;
		return *this;
	}

	float dot(Vector2 &v4) {
		return x * v4.x + y * v4.y;
	}

	float magnitude() {
		return sqrt(x * x + y * y);
	}

	Vector2 normalized() {
		return *this / magnitude();
	}
};