#include "file.hpp"
#include "shader.hpp"
#include "opengl-helpers.hpp"

int main() {
	initializeGLFW();

	auto config = getConfig();
	auto window = getWindow(config.window);
	auto gpuBuffers = getEmptyGPUbuffers();
	
	loadAttributesIntoVAO(config.triangle.attributes);
	loadVerticesIntoVBO(config.triangle.vertices);
	loadIndicesIntoEBO(config.triangle.indices);

	gpuBuffers.program = getGPUprogram(
		config.triangle.shaders.vertex,
		config.triangle.shaders.fragment
	);

	while (!glfwWindowShouldClose(window.get())) {
		clearWindow();

		glUseProgram(gpuBuffers.program);
		glDrawElements(GL_TRIANGLES, config.triangle.indices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	terminate(gpuBuffers);
}
