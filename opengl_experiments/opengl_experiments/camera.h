#pragma once

#include <glad/glad.h>
#include "math/vector3.h"
#include "math/matrix4.h"
#include "math/math_helpers.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
	// Camera Attributes
	Matrix4 view;
	Vector3 position;
	Vector3 front;
	Vector3 up;
	Vector3 right;
	Vector3 worldUp;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// Constructor with vectors
	Camera(Vector3 pPos = Vector3(0.0f, 0.0f, 0.0f), Vector3 pUp = Vector3(0.0f, 1.0f, 0.0f), float pYaw = YAW, float pPitch = PITCH) : 
		front(Vector3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		position = pPos;
		worldUp = pUp;
		yaw = pYaw;
		pitch = pPitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float pYaw, float pPitch) : 
		front(Vector3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		position = Vector3(posX, posY, posZ);
		worldUp = Vector3(upX, upY, upZ);
		yaw = pYaw;
		pitch = pPitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	Matrix4 getViewMatrix() {
		return view.lookAt(position, position + front, up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(CameraMovement direction, float deltaTime) {
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yOffset) {
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yOffset;
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors() {
		// Calculate the new Front vector
		Vector3 newFront;
		newFront.x = cos(deg2rad(yaw)) * cos(deg2rad(pitch));
		newFront.y = sin(deg2rad(pitch));
		newFront.z = sin(deg2rad(yaw)) * cos(deg2rad(pitch));
		front = newFront.normalized();
		// Also re-calculate the Right and Up vector
		right = front.cross(worldUp).normalized();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = right.cross(front).normalized();
	}
};