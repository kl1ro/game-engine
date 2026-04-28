#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
  glm::mat4 matrix = glm::mat4(1.0f);

  void setPosition(glm::vec3 pos);
  void setRotation(glm::vec3 rot);
  void setScale(glm::vec3 s);
  void rebuild();
};
