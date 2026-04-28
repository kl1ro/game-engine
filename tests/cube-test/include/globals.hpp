#pragma once
#include "camera.hpp"
#include "mouse.hpp"
#include "time.hpp"

#include <GLFW/glfw3.h>

namespace Globals {
  extern Mouse mouse;
  extern Camera camera;
  extern Time time;
  extern GLFWwindow* window;
}
