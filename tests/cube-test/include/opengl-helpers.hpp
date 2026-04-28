#pragma once

#include "config.hpp"
#include "shader.hpp"
#include "stb_image.h"

#include <GLFW/glfw3.h>

#include <iostream>

struct GPUBuffers {
  GLuint VAO, VBO, EBO, texture, program;
};

void initializeGLFW();

GLFWwindow* getWindow(WindowConfig config, GLFWcursorposfun mouseCallback = nullptr);

int getStride(const std::vector<VertexAttribute>& vertexAttributes);

GPUBuffers getEmptyGPUbuffers();

void loadAttributesIntoVAO(const std::vector<VertexAttribute>& vertexAttributes);
void loadVerticesIntoVBO(const std::vector<float>& vertices);
void loadIndicesIntoEBO(const std::vector<unsigned int>& indices);
void loadTexture(const std::string& path, const GLuint& buffer);

void terminate(GPUBuffers& buffers);

void clearWindow(
  GLclampf red = 0.08f,
  GLclampf green = 0.08f,
  GLclampf blue = 0.08f,
  GLclampf alpha = 1.0f
);
