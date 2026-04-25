#include "render.hpp"

void render() {
	Globals::time.refresh();

	clearWindow();
	processKeyboard();

	GPUbuffers& gpuBuffers = Globals::gpuBuffers;
	glUseProgram(gpuBuffers.program);

	Camera& camera = Globals::camera;
	camera.refreshMVP();
	camera.uploadMVPtoGPU(gpuBuffers.program);

	DrawingContext& ctx = Globals::drawingContext;
	ctx.mode = GL_FILL;
	ctx.rgba = { 0.5f, 0.5f, 0.5f, 1.0f };
	drawMesh();

	ctx.mode = GL_LINE;
	ctx.rgba = { 0.2f, 0.2f, 0.2f, 1.0f };
	drawMesh();

	glfwSwapBuffers(Globals::window);
	glfwPollEvents();
}
