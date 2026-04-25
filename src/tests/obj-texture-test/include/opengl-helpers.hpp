#pragma once
#define GL_GLEXT_PROTOTYPES
#include <iostream>
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include "config.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "file.hpp"
#include "vertex-buffers.hpp"

struct RGBA {
	float r, g, b, a;
};

struct DrawingContext {
	RGBA rgba;
	GLint colorLoc;
	GLint useTextureLoc;
	GLsizei indexCount;
	GLenum mode;
};

void initialize();

GLFWwindow* getWindow(WindowConfig config);

int getStride(const std::vector<VertexAttribute>& vertexAttributes);

VertexBuffers getEmptyVertexBuffers();

void loadAttributesIntoVAO(const std::vector<VertexAttribute>& vertexAttributes);
void loadVerticesIntoVBO(const std::vector<float>& vertices);
void loadIndicesIntoEBO(const std::vector<unsigned int>& indices);
void loadTexture(const std::string& path, const GLuint& buffer);
void terminate();
