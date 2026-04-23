#include "keyboard.hpp"

void processKeyboard() {
	Camera& camera = Globals::camera;
	GLFWwindow* window = Globals::window;
	Time& time = Globals::time;

	float distance = camera.speed * time.dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.pos += distance * camera.front;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.pos -= distance * camera.front;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * distance;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * distance;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
