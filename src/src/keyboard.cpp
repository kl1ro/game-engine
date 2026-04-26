#include "keyboard.hpp"

void processKeyPress() {
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

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.pos += distance * camera.up;

  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    camera.pos -= distance * camera.up;
}

void onKeyPress(GLFWwindow*, int key, int, int action, int) {
  if (action != GLFW_PRESS)
    return;

  if (key == GLFW_KEY_V)
    toggleViewMode();

  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(Globals::window, true);
}
