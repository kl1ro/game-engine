#include "file.hpp"
#include "shader.hpp"
#include "opengl-helpers.hpp"

int main() {
	initializeGLFW();

	auto config = getConfig();
	auto window = getWindow(config.window);
	auto gpuBuffers = getGPUbuffers(config.triangle.attributes, config.triangle.shaders);
	
	loadVerticesIntoVBO(config.triangle.vertices);

	while (!glfwWindowShouldClose(window.get())) {
		clearWindow();

		glUseProgram(gpuBuffers.program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	terminate(gpuBuffers);
}
