#pragma once
#include "camera.hpp"
#include "config.hpp"
#include "material.hpp"
#include "mouse.hpp"
#include "scene.hpp"
#include "time.hpp"

#include <GLFW/glfw3.h>

namespace Globals {
  extern Mouse mouse;
  extern Time time;
  extern GLFWwindow* window;
  extern Config config;

  extern Camera camera;
  extern ViewMode viewMode;
  extern DisplayMode displayMode;

  extern GLuint program;
  extern DrawingContext drawingContext;

  extern Scene scene;
  extern std::vector<Material> materials;
}
