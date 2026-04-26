#include "globals.hpp"

namespace Globals {
  Mouse mouse;
  Time time;
  GLFWwindow* window = nullptr;
  Config config;

  Camera camera;
  ViewMode viewMode = ViewMode::Relative;

  GLuint program;
  DrawingContext drawingContext;
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
}
