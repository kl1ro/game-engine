#pragma once
#include "mouse.hpp"
#include "camera.hpp"
#include "time.hpp"
#include <GLFW/glfw3.h>

namespace Globals {
	extern Mouse mouse;
	extern Camera camera;
	extern Time time;
	extern GLFWwindow* window;
}
