#include "render.hpp"

#include "globals.hpp"

void render() {
  Globals::time.refresh();

  clearWindow();

  if (Globals::viewMode == ViewMode::Absolute)
    processKeyPress();

  glUseProgram(Globals::program);

  Camera& camera = Globals::camera;
  camera.refreshVP();
  camera.uploadVPtoGPU(Globals::program);

  glUniform3f(
    glGetUniformLocation(Globals::program, "viewPos"),
    camera.pos.x,
    camera.pos.y,
    camera.pos.z
  );

  DrawingContext& ctx = Globals::drawingContext;
  ctx.mode = GL_FILL;
  ctx.rgba = {0.5f, 0.5f, 0.5f, 1.0f};
  drawScene(Globals::scene);

  if (Globals::displayMode == DisplayMode::Wireframe) {
    ctx.mode = GL_LINE;
    ctx.rgba = {0.2f, 0.2f, 0.2f, 1.0f};
    drawScene(Globals::scene);
  }

  renderUI();

  glfwSwapBuffers(Globals::window);
  glfwPollEvents();
}

void drawScene(const Scene& scene) {
  loadLightsToGPU();
  for (auto& object : scene.objects) drawObject(object);
}

void drawObject(const Object& object) {
  DrawingContext& ctx = Globals::drawingContext;
  glUniformMatrix4fv(ctx.modelLoc, 1, GL_FALSE, glm::value_ptr(object.transform.matrix));
  drawMesh(object.mesh);
}

void drawMesh(const Mesh& mesh) {
  glPolygonMode(GL_FRONT_AND_BACK, Globals::drawingContext.mode);
  for (auto& submesh : mesh.submeshes) drawSubmesh(submesh);
}

void drawSubmesh(const Submesh& submesh) {
  DrawingContext& ctx = Globals::drawingContext;

  bool materialExists =
    submesh.materialIndex > -1 && submesh.materialIndex < (int)Globals::materials.size();

  glUniform1i(ctx.displayModeLoc, static_cast<int>(Globals::displayMode));

  if (Globals::displayMode == DisplayMode::Wireframe || !materialExists)
    drawSubmeshWithContext(submesh);
  else
    drawSubmeshWithMaterial(submesh);
}

void drawSubmeshWithContext(const Submesh& submesh) {
  DrawingContext& ctx = Globals::drawingContext;
  glUniform4f(ctx.colorLoc, ctx.rgba.r, ctx.rgba.g, ctx.rgba.b, 1.0f);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(submesh.buffers.VAO);
  glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void drawSubmeshWithMaterial(const Submesh& submesh) {
  DrawingContext& ctx = Globals::drawingContext;
  Material& material = Globals::materials[submesh.materialIndex];

  if (material.diffuseTexture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.diffuseTexture);
  } else {
    glm::vec3& rgb = material.diffuseColor;
    glUniform4f(ctx.colorLoc, rgb.r, rgb.g, rgb.b, 1.0f);
  }

  glBindVertexArray(submesh.buffers.VAO);
  glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void clearWindow(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
