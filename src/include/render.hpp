#pragma once
#include "keyboard.hpp"
#include "opengl-helpers.hpp"
#include "ui.hpp"

void render();
void drawMesh();
void drawSubmesh(const Submesh& submesh);
void drawSubmeshWithContext(const Submesh& submesh);
void drawSubmeshWithMaterial(const Submesh& submesh);

void clearWindow(
  GLclampf red = 0.08f,
  GLclampf green = 0.08f,
  GLclampf blue = 0.08f,
  GLclampf alpha = 1.0f
);
