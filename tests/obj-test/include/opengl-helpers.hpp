#pragma once

#include "config.hpp"
#include "file.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "stb_image.h"

#include <GLFW/glfw3.h>

#include <iostream>

struct GPUbuffers {
  GLuint VAO, VBO, EBO, texture, program;
};

struct RGBA {
  float r, g, b, a;
};

struct DrawingContext {
  GLint colorLoc;
  GLsizei indexCount;
  GLenum mode;
  RGBA rgba;
};

void initialize();

GLFWwindow* getWindow(WindowConfig config);

int getStride(const std::vector<VertexAttribute>& vertexAttributes);

GPUbuffers& getEmptyGPUbuffers();

void loadAttributesIntoVAO(const std::vector<VertexAttribute>& vertexAttributes);
void loadVerticesIntoVBO(const std::vector<float>& vertices);
void loadIndicesIntoEBO(const std::vector<unsigned int>& indices);
void loadTexture(const std::string& path, const GLuint& buffer);

void terminate();

void clearWindow(
  GLclampf red = 0.08f,
  GLclampf green = 0.08f,
  GLclampf blue = 0.08f,
  GLclampf alpha = 1.0f
);

void drawMesh();
