#define TINYOBJLOADER_IMPLEMENTATION
#include "file.hpp"

#include "globals.hpp"

std::string readFile(const std::string& path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path << "\n";
    std::exit(1);
  }

  std::stringstream buf;
  buf << file.rdbuf();
  return buf.str();
}

MeshData readMesh(const std::string& path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  bool success =
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true);

  if (!err.empty())
    std::cerr << "OBJ error: " << err << std::endl;
  if (!success)
    throw std::runtime_error("Failed to load OBJ: " + path);

  MeshData mesh;

  for (auto& shape : shapes) {
    for (auto& index : shape.mesh.indices) {
      float vx = attrib.vertices[3 * index.vertex_index + 0];
      float vy = attrib.vertices[3 * index.vertex_index + 1];
      float vz = attrib.vertices[3 * index.vertex_index + 2];
      float u = index.texcoord_index >= 0 ? attrib.texcoords[2 * index.texcoord_index + 0] : 0.0f;
      float v = index.texcoord_index >= 0 ? attrib.texcoords[2 * index.texcoord_index + 1] : 0.0f;

      mesh.vertices.insert(mesh.vertices.end(), {vx, vy, vz, u, v});
      mesh.indices.push_back(mesh.indices.size());
    }
  }

  DrawingContext& ctx = Globals::drawingContext;
  ctx.indexCount = mesh.indices.size();

  return mesh;
}
