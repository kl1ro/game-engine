#pragma once
#include "mesh.hpp"
#include "opengl-helpers.hpp"
#include "scene.hpp"
#include "tiny_gltf.h"
#include "tiny_obj_loader.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path);

std::string openFileDialog(const std::string& filter = "*");

void loadObj(const std::string& path);

void loadMaterials(
  const std::string baseDir,
  const std::vector<tinyobj::material_t>& tinyMaterials
);

void loadMesh(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib);

static GLuint loadGLTFTexture(const tinygltf::Model& model, int textureIndex);
static int loadGLTFMaterials(const tinygltf::Model& model);
static const float* getBufferData(const tinygltf::Model& model, int accessorIndex);
static const unsigned short* getIndexData(const tinygltf::Model& model, int accessorIndex);
static Mesh loadGLTFMesh(const tinygltf::Model& model, int meshIndex, int materialOffset);
static Transform nodeToTransform(const tinygltf::Node& node);
static Object loadNode(const tinygltf::Model& model, int nodeIndex, int materialOffset);
Scene loadScene(const std::string& path);
