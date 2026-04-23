#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "tiny_obj_loader.h"
#include "object.hpp"
#include "opengl-helpers.hpp"

std::string readFile(const std::string& path);
MeshData readMesh(const std::string& path);
