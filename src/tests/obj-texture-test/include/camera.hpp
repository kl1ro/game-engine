#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "opengl-helpers.hpp"

struct Camera {
	glm::vec3 pos   = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	float yaw   = -90.0f;
	float pitch =  0.0f;
	float speed = 100.0f;

	void rotate(float xoffset, float yoffset);
	void refreshMVP();
	void uploadMVPtoGPU(GLuint program);
};
