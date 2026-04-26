#include "file.hpp"
#include "globals.hpp"
#include "keyboard.hpp"
#include "opengl-helpers.hpp"
#include "shader.hpp"

int main() {
  initializeGLFW();

  auto config = getConfig();
  auto window = getWindow(config.window, moveMouse);
  auto gpuBuffers = getEmptyGPUbuffers();

  Time& time = Globals::time;
  Camera& camera = Globals::camera;

  loadAttributesIntoVAO(config.triangle.attributes);
  loadVerticesIntoVBO(config.triangle.vertices);
  loadIndicesIntoEBO(config.triangle.indices);
  loadTexture(config.triangle.textureSrc, gpuBuffers.texture);

  gpuBuffers.program =
    getGPUprogram(config.triangle.shaders.vertex, config.triangle.shaders.fragment);

  while (!glfwWindowShouldClose(window)) {
    time.refresh();
    clearWindow();

    processKeyboard();

    glUseProgram(gpuBuffers.program);

    camera.refreshMVP();
    camera.uploadMVPtoGPU(gpuBuffers.program);

    glBindTexture(GL_TEXTURE_2D, gpuBuffers.texture);
    glDrawElements(GL_TRIANGLES, config.triangle.indices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  terminate(gpuBuffers);
}
