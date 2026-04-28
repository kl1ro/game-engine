#include "render.hpp"

#include "globals.hpp"

void render() {
  Globals::time.refresh();

  clearWindow();
  processKeyboard();

  glUseProgram(Globals::program);

  Camera& camera = Globals::camera;
  camera.refreshMVP();
  camera.uploadMVPtoGPU(Globals::program);

  glUniform3f(glGetUniformLocation(Globals::program, "lightPos"), 100.0f, 100.0f, 300.0f);
  glUniform3f(glGetUniformLocation(Globals::program, "lightColor"), 1.0f, 1.0f, 1.0f);

  glUniform3f(
    glGetUniformLocation(Globals::program, "viewPos"),
    camera.pos.x,
    camera.pos.y,
    camera.pos.z
  );

  DrawingContext& ctx = Globals::drawingContext;
  ctx.mode = GL_FILL;
  ctx.rgba = {0.5f, 0.5f, 0.5f, 1.0f};
  drawMesh();

  glfwSwapBuffers(Globals::window);
  glfwPollEvents();
}

void drawMesh() {
  DrawingContext& ctx = Globals::drawingContext;
  glPolygonMode(GL_FRONT_AND_BACK, ctx.mode);

  for (auto& mesh : Globals::meshes)
    for (auto& submesh : mesh.submeshes) drawSubmesh(submesh);
}

void drawSubmesh(const Submesh& submesh) {
  DrawingContext& ctx = Globals::drawingContext;

  bool materialExists =
    submesh.materialIndex > -1 && submesh.materialIndex < (int)Globals::materials.size();

  if (ctx.mode == GL_LINE || !materialExists)
    drawSubmeshWithContext(submesh);
  else
    drawSubmeshWithMaterial(submesh);
}

void drawSubmeshWithContext(const Submesh& submesh) {
  DrawingContext& ctx = Globals::drawingContext;
  glUniform1i(ctx.useTextureLoc, false);
  glUniform4f(ctx.colorLoc, ctx.rgba.r, ctx.rgba.g, ctx.rgba.b, 1.0f);
  glBindVertexArray(submesh.buffers.VAO);
  glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void drawSubmeshWithMaterial(const Submesh& submesh) {
  DrawingContext& ctx = Globals::drawingContext;
  Material& material = Globals::materials[submesh.materialIndex];

  if (material.diffuseTexture) {
    glUniform1i(ctx.useTextureLoc, true);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.diffuseTexture);
  } else {
    glUniform1i(ctx.useTextureLoc, false);
    glm::vec3& rgb = material.diffuseColor;
    glUniform4f(ctx.colorLoc, rgb.r, rgb.g, rgb.b, 1.0f);
  }

  glBindVertexArray(submesh.buffers.VAO);
  glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void clearWindow(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
