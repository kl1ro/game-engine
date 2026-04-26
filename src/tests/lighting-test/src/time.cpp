#include "time.hpp"

void Time::refresh() {
  currentFrame = glfwGetTime();
  dt = currentFrame - lastFrame;
  lastFrame = currentFrame;
  fps = 1 / dt;
}
