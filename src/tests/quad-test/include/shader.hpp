#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <string>
#include <iostream>

GLuint compileShader(GLenum type, const std::string& src);

GLuint linkProgram(GLuint vert, GLuint frag);

GLuint getGPUprogram(std::string vertSrcPath, std::string fragSrcPath);
