#include "file.hpp"
#include "shader.hpp"
#include "opengl-helpers.hpp"

int main() {
	initializeGLFW();

	auto config = getConfig();
	auto window = getWindow(config.window);
	auto gpuBuffers = getGPUbuffers(config.triangle.attributes);
	
	loadVerticesIntoVBO(config.triangle.vertices);

	auto program = getGPUprogram(config.triangle.shaders.vertex, config.triangle.shaders.fragment);

	while (!glfwWindowShouldClose(window)) {
		clearWindow();

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	terminate(gpuBuffers, program);
}
