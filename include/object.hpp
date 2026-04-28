#pragma once
#include "mesh.hpp"
#include "transform.hpp"

#include <iostream>

struct Object {
  std::string name;
  Mesh mesh;
  Transform transform;
  bool selected = false;
};
