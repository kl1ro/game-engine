#include "globals.hpp"

namespace Globals {
	Mouse mouse;
	Camera camera;
	Time time;
	GLFWwindow* window = nullptr;
	Config config;
	GLuint program;
	DrawingContext drawingContext;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
}
