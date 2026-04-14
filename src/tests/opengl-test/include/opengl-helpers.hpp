#define GL_GLEXT_PROTOTYPES
#include <iostream>
#include <GLFW/glfw3.h>
#include "config.hpp"

struct GPUBuffers {
  GLuint VAO, VBO;
};

void initializeGLFW();

GLFWwindow* getWindow(WindowConfig config);

int getStride(std::vector<VertexAttribute> vertexAttributes);

GPUBuffers getGPUbuffers(std::vector<VertexAttribute> vertexAttributes);

void loadVerticesIntoVBO(std::vector<float> vertices);

void terminate(GPUBuffers buffers, GLuint program);

void clearWindow(
	GLclampf red = 0.08f,
	GLclampf green = 0.08f,
	GLclampf blue = 0.08f,
	GLclampf alpha = 1.0f
);
