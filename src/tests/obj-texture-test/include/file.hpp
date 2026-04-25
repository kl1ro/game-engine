#pragma once
#define GL_GLEXT_PROTOTYPES
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"
#include "object.hpp"
#include "opengl-helpers.hpp"

std::string readFile(const std::string& path);
void loadObj(const std::string& path);
