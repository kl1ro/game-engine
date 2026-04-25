#pragma once
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <string>
#include <iostream>
#include "config.hpp"

GLuint compileShader(GLenum type, const std::string& src);

GLuint linkProgram(GLuint vert, GLuint frag);

GLuint getGPUprogram(const ShadersConfig& shaders);
