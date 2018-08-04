#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Control.h"
#include <iostream>


#define PI 3.14159265359

#define MouseSensitivity 0.1f

/*
	FIX!!! rasterizing and ray casting do not line up!
	FOV does not work for ray casting
*/

class Camera {
public:
	Camera(int w, int h, glm::vec3 p, float fieldOfView, float near, float far, float rSpeed, float mSpeed) {
		position = p;
		dir = glm::vec3(0.0f, 0.0f, -1.0f);
		u = glm::vec3(1.0f, 0.0f, 0.0f);
		v = glm::vec3(0.0f, 1.0f, 0.0f);
		pitch = -90.0f;
		yaw = 0.0f;
		roationSpeed = rSpeed;
		movementSpeed = mSpeed;
		fov = fieldOfView;
		nearPlane = near;
		farPlane = far;
		screenWidth = w;
		screenHeight = h;
	}

	void update() {

		glm::vec4 mouse = getMouse();

		yaw = mouse.z * MouseSensitivity;
		pitch = mouse.w * MouseSensitivity;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		//move position if keys were pressed
		if (getKey(GLFW_KEY_W))
			position += movementSpeed * dir;
		if (getKey(GLFW_KEY_S))
			position -= movementSpeed * dir;
		if (getKey(GLFW_KEY_A))
			position -= glm::normalize(glm::cross(dir, v)) * movementSpeed;
		if (getKey(GLFW_KEY_D))
			position += glm::normalize(glm::cross(dir, v)) * movementSpeed;
		if (getKey(GLFW_KEY_Q))
			position += glm::vec3(0.0, 1.0, 0.0) * movementSpeed;
		if (getKey(GLFW_KEY_E))
			position += glm::vec3(0.0, -1.0, 0.0) * movementSpeed;
		//update camera vectors
		dir = glm::vec3(
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		);
		dir = glm::vec3(1.0 / tan(0.5*fov*(PI / 180))) * dir;
		u = glm::normalize(glm::cross(dir, glm::vec3(0.0f,1.0f,0.0f)));
		v = glm::normalize(glm::cross(u, dir));


	}

	glm::mat4 getViewMatrix() { return glm::lookAt(position, position + dir, v); }
	glm::mat4 getProjMatrix() { return glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, nearPlane, farPlane); }
	glm::vec3 getDir() { return dir; }
	glm::vec3 getU() { return u; }
	glm::vec3 getV() { return v; }
	glm::vec3 getPosition() { return position; }

	~Camera() {}

private:
	glm::vec3 position;
	glm::vec3 dir;
	glm::vec3 u;
	glm::vec3 v;
	float pitch;
	float yaw;
	float roationSpeed;
	float movementSpeed;
	float fov;
	float nearPlane;
	float farPlane;
	int screenWidth;
	int screenHeight;
};

#endif //Camera.h

