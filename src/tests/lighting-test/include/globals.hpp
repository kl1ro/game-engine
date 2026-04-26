#pragma once
#include <GLFW/glfw3.h>
#include "mouse.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "config.hpp"
#include "material.hpp"

namespace Globals {
	extern Mouse mouse;
	extern Camera camera;
	extern Time time;
	extern GLFWwindow* window;
	extern Config config;
	extern GLuint program;
	extern DrawingContext drawingContext;
	extern std::vector<Mesh> meshes;
	extern std::vector<Material> materials;
}
