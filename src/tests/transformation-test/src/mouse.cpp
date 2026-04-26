#include "mouse.hpp"

#include "globals.hpp"

void Mouse::moveTo(double xpos, double ypos) {
  if (!this->moved) {
    this->prevx = xpos;
    this->prevy = ypos;
    this->moved = true;
  }

  this->xoffset = (xpos - this->prevx) * this->sensitivity;
  this->yoffset = (this->prevy - ypos) * this->sensitivity;
  this->prevx = xpos;
  this->prevy = ypos;
}

void moveMouse(GLFWwindow*, double xpos, double ypos) {
  Mouse& mouse = Globals::mouse;
  Camera& camera = Globals::camera;

  mouse.moveTo(xpos, ypos);
  camera.rotate(mouse.xoffset, mouse.yoffset);
}
