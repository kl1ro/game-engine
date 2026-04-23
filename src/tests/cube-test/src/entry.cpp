#include "file.hpp"
#include "shader.hpp"
#include "opengl-helpers.hpp"
#include "keyboard.hpp"
#include "globals.hpp"

int main() {
	initializeGLFW();

	auto config     = getConfig();
	auto window     = getWindow(config.window, moveMouse);
	auto gpuBuffers = getEmptyGPUbuffers();

	glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

	Time& time = Globals::time;
	Camera& camera = Globals::camera;

	loadAttributesIntoVAO(config.triangle.attributes);
	loadVerticesIntoVBO(config.triangle.vertices);
	loadIndicesIntoEBO(config.triangle.indices);
	loadTexture(config.triangle.textureSrc, gpuBuffers.texture);

	gpuBuffers.program = getGPUprogram(
		config.triangle.shaders.vertex,
		config.triangle.shaders.fragment
	);

	while (!glfwWindowShouldClose(window)) {
		time.refresh();
		clearWindow();

		processKeyboard();

		glUseProgram(gpuBuffers.program);

		camera.refreshMVP();
		camera.uploadMVPtoGPU(gpuBuffers.program);

		GLint colorLoc = glGetUniformLocation(gpuBuffers.program, "color");

		glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, config.triangle.indices.size(), GL_UNSIGNED_INT, 0);

		glUniform4f(colorLoc, 0.2f, 0.2f, 0.2f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, config.triangle.indices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	terminate(gpuBuffers);
}
