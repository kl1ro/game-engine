#include "render.hpp"

int main() {
  initialize();

  Config& config = Globals::config;
  MeshData mesh = readMesh(config.mesh);

  loadAttributesIntoVAO(config.triangle.attributes);
  loadVerticesIntoVBO(mesh.vertices);
  loadIndicesIntoEBO(mesh.indices);

  while (!glfwWindowShouldClose(Globals::window)) render();

  terminate();
}
