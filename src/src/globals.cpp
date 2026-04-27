#include "globals.hpp"

namespace Globals {
  Mouse mouse;
  Time time;
  GLFWwindow* window = nullptr;
  Config config;

  Camera camera;
  ViewMode viewMode = ViewMode::Relative;
  DisplayMode displayMode = DisplayMode::Wireframe;

  GLuint program;
  DrawingContext drawingContext;
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
}
