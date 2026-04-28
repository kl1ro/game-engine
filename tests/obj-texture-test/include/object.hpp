#pragma once
#include "vector"
#include "vertex-buffers.hpp"

struct Submesh {
  VertexBuffers buffers;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  int materialIndex;
};

struct Mesh {
  std::vector<Submesh> submeshes;
};
