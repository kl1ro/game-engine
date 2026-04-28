#pragma once
#include "camera.hpp"
#include "config.hpp"
#include "material.hpp"
#include "mouse.hpp"
#include "time.hpp"

#include <GLFW/glfw3.h>

namespace Globals {
  extern Mouse mouse;
  extern Camera camera;
  extern Time time;
  extern GLFWwindow* window;
  extern Config config;
  extern GLuint program;
  extern DrawingContext drawingContext;
  extern std::vector<Mesh> meshes;
  extern std::vector<Material> materials;
}
