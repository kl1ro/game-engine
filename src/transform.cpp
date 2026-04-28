#include "transform.hpp"

void Transform::setPosition(glm::vec3 pos) {
  position = pos;
  rebuild();
}

void Transform::setRotation(glm::vec3 rot) {
  rotation = rot;
  rebuild();
}
void Transform::setScale(glm::vec3 s) {
  scale = s;
  rebuild();
}

void Transform::rebuild() {
  matrix = glm::mat4(1.0f);
  matrix = glm::translate(matrix, position);
  matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
  matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
  matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
  matrix = glm::scale(matrix, scale);
}
