#include "mouse.hpp"

#include "globals.hpp"

void Mouse::moveTo(double xpos, double ypos) {
  if (!moved) {
    prevx = xpos;
    prevy = ypos;
    moved = true;
    return;
  }

  xoffset = (xpos - prevx) * sensitivity;
  yoffset = (prevy - ypos) * sensitivity;
  prevx = xpos;
  prevy = ypos;
}

void moveMouse(GLFWwindow*, double xpos, double ypos) {
  Mouse& mouse = Globals::mouse;
  Camera& camera = Globals::camera;

  mouse.moveTo(xpos, ypos);
  camera.rotate(mouse.xoffset, mouse.yoffset);
}

void onCursorPos(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

  if (Globals::viewMode == ViewMode::Absolute)
    moveMouse(window, xpos, ypos);
}
