#pragma once

#include <GL/gl.h>

#include <iostream>
#include <string>

GLuint compileShader(GLenum type, const std::string& src);

GLuint linkProgram(GLuint vert, GLuint frag);

GLuint getGPUprogram(std::string vertSrcPath, std::string fragSrcPath);
