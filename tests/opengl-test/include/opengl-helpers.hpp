
#include "config.hpp"
#include "shader.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

struct GPUBuffers {
  GLuint VAO, VBO, program;
};

struct GlfwWindowDestroyer {
  void operator()(GLFWwindow* window) const;
};

using UniqueGlfwWindow = std::unique_ptr<GLFWwindow, GlfwWindowDestroyer>;

void initializeGLFW();

UniqueGlfwWindow getWindow(WindowConfig config);

int getStride(std::vector<VertexAttribute> vertexAttributes);

GPUBuffers getGPUbuffers(std::vector<VertexAttribute> vertexAttributes, ShadersConfig shaders);

void loadVerticesIntoVBO(std::vector<float> vertices);

void terminate(GPUBuffers buffers);

void clearWindow(
  GLclampf red = 0.08f,
  GLclampf green = 0.08f,
  GLclampf blue = 0.08f,
  GLclampf alpha = 1.0f
);
