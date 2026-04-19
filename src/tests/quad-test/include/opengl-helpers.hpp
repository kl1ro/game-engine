#define GL_GLEXT_PROTOTYPES
#include <iostream>
#include <GLFW/glfw3.h>
#include "config.hpp"
#include "shader.hpp"

struct GPUBuffers {
  GLuint VAO, VBO, EBO, program;
};

struct GlfwWindowDestroyer {
	void operator()(GLFWwindow* window) const;
};

using UniqueGlfwWindow = std::unique_ptr<GLFWwindow, GlfwWindowDestroyer>;

void initializeGLFW();

UniqueGlfwWindow getWindow(WindowConfig config);

int getStride(const std::vector<VertexAttribute>& vertexAttributes);

GPUBuffers getEmptyGPUbuffers();

void loadAttributesIntoVAO(const std::vector<VertexAttribute>& vertexAttributes);
void loadVerticesIntoVBO(const std::vector<float>& vertices);
void loadIndicesIntoEBO(const std::vector<unsigned int>& indices);

void terminate(GPUBuffers& buffers);

void clearWindow(
	GLclampf red = 0.08f,
	GLclampf green = 0.08f,
	GLclampf blue = 0.08f,
	GLclampf alpha = 1.0f
);
