#include "file.hpp"

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
