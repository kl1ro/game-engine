#pragma once
#include "config.hpp"
#include "file.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include "vertex-buffers.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

struct RGBA {
  float r, g, b, a;
};

struct DrawingContext {
  RGBA rgba;
  GLenum mode;
  GLsizei indexCount;

  GLint colorLoc;
  GLint displayModeLoc;
  GLint modelLoc;
  GLint viewLoc;
  GLint projectionLoc;
};

void initialize();

GLFWwindow* getWindow(WindowConfig config);

int getStride(const std::vector<VertexAttribute>& vertexAttributes);

VertexBuffers getEmptyVertexBuffers();

void loadAttributesIntoVAO(const std::vector<VertexAttribute>& vertexAttributes);
void loadVerticesIntoVBO(const std::vector<float>& vertices);
void loadIndicesIntoEBO(const std::vector<unsigned int>& indices);
void loadTexture(const std::string& path, const GLuint& buffer);
void loadLightsToGPU();
void terminate();
