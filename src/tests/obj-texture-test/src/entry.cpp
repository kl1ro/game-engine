#include "render.hpp"

int main() {
	initialize();
	loadObj(Globals::config.mesh);
	while (!glfwWindowShouldClose(Globals::window)) render();
	terminate();
}