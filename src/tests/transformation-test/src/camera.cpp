#include "camera.hpp"

void Camera::rotate(float xoffset, float yoffset) {
  yaw += xoffset;
  pitch += yoffset;
  pitch = glm::clamp(pitch, -89.0f, 89.0f);

  glm::vec3 dir;
  dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  dir.y = sin(glm::radians(pitch));
  dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(dir);
}

void Camera::refreshMVP() {
  model = glm::mat4(1.0f);

  view = glm::lookAt(this->pos, this->pos + this->front, this->up);

  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::uploadMVPtoGPU(GLuint program) {
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));

  glUniformMatrix4fv(
    glGetUniformLocation(program, "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );
}
