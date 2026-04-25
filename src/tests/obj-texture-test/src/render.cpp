#include "render.hpp"
#include "globals.hpp"

void render() {
	Globals::time.refresh();

	clearWindow();
	processKeyboard();

	glUseProgram(Globals::program);

	Camera& camera = Globals::camera;
	camera.refreshMVP();
	camera.uploadMVPtoGPU(Globals::program);

	DrawingContext& ctx = Globals::drawingContext;
	ctx.mode = GL_FILL;
	drawMesh();

	ctx.mode = GL_LINE;
	ctx.rgba = { 0.2f, 0.2f, 0.2f, 1.0f };
	drawMesh();

	glfwSwapBuffers(Globals::window);
	glfwPollEvents();
}
