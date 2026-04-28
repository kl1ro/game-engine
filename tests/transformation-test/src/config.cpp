#include "config.hpp"

Config getConfig(std::string path) {
  std::ifstream file(path);
  nlohmann::json j;
  file >> j;

  Config cfg;

  cfg.window.width = j["window"].value("width", 800);
  cfg.window.height = j["window"].value("height", 600);
  cfg.window.title = j["window"].value("title", "OpenGL Triangle");

  cfg.triangle.vertices = j["triangle"]["vertices"].get<std::vector<float>>();
  cfg.triangle.indices = j["triangle"]["indices"].get<std::vector<unsigned int>>();
  cfg.triangle.attributes = j["triangle"]["attributes"].get<std::vector<VertexAttribute>>();
  cfg.triangle.shaders.vertex = j["triangle"]["shaders"].value("vertex", "shaders/vertex.glsl");
  cfg.triangle.shaders.fragment =
    j["triangle"]["shaders"].value("fragment", "shaders/fragment.glsl");
  cfg.triangle.textureSrc = j["triangle"].value("textureSrc", "assets/texture.png");

  return cfg;
}
