#pragma once
#include "keyboard.hpp"
#include "object.hpp"
#include "opengl-helpers.hpp"
#include "ui.hpp"

void render();
void drawScene(const Scene& scene);
void drawObject(const Object& object);
void drawMesh(const Mesh& mesh);
void drawSubmesh(const Submesh& submesh);
void drawSubmeshWithContext(const Submesh& submesh);
void drawSubmeshWithMaterial(const Submesh& submesh);

void clearWindow(
  GLclampf red = 0.08f,
  GLclampf green = 0.08f,
  GLclampf blue = 0.08f,
  GLclampf alpha = 1.0f
);
