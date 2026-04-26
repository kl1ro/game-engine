#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct Material {
  GLuint program;

  GLuint diffuseTexture;
  GLuint specularTexture;
  GLuint normalTexture;
  GLuint emissiveTexture;
  GLuint ambientTexture;
  GLuint opacityTexture;

  glm::vec3 diffuseColor;
  glm::vec3 specularColor;
  glm::vec3 ambientColor;
  glm::vec3 emissiveColor;
  float shininess;
  float opacity;
  float refraction;
};
