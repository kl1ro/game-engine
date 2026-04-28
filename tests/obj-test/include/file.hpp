#pragma once
#include "object.hpp"
#include "opengl-helpers.hpp"
#include "tiny_obj_loader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path);
MeshData readMesh(const std::string& path);
