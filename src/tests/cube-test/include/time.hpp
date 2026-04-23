#pragma once
#include <GLFW/glfw3.h>

struct Time {
	float currentFrame = 0.0f;
	float lastFrame = 0.0f;
	float dt = 0.0f;
	float fps = 0.0f;
	
	void refresh();
};
