#pragma once
#include "mesh.hpp"
#include "opengl-helpers.hpp"
#include "tiny_obj_loader.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path);

std::string openFileDialog();

void loadObj(const std::string& path);

void loadMaterials(
  const std::string baseDir,
  const std::vector<tinyobj::material_t>& tinyMaterials
);

void loadMesh(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib);
