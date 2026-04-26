#include "opengl-helpers.hpp"

void initializeGLFW() {
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    std::exit(1);
  }
}

UniqueGlfwWindow getWindow(WindowConfig config) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  UniqueGlfwWindow window(
    glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr)
  );

  if (!window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    std::exit(1);
  }

  glfwMakeContextCurrent(window.get());
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Renderer:       " << glGetString(GL_RENDERER) << std::endl;
  return window;
}

int getStride(const std::vector<VertexAttribute>& vertexAttributes) {
  int stride = 0;
  for (auto& attribute : vertexAttributes)
    stride += attribute.size;
  return stride;
}

GPUBuffers getEmptyGPUbuffers() {
  GPUBuffers buffers;

  glGenVertexArrays(1, &buffers.VAO);
  glGenBuffers(1, &buffers.VBO);
  glGenBuffers(1, &buffers.EBO);
  glBindVertexArray(buffers.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.EBO);

  return buffers;
}

void loadAttributesIntoVAO(const std::vector<VertexAttribute>& vertexAttributes) {
  auto stride = getStride(vertexAttributes);

  for (auto& attribute : vertexAttributes) {
    glVertexAttribPointer(
      attribute.index,
      attribute.size,
      GL_FLOAT,
      GL_FALSE,
      stride * sizeof(float),
      (void*)(attribute.offset * sizeof(float))
    );

    glEnableVertexAttribArray(attribute.index);
  }
}

void loadVerticesIntoVBO(const std::vector<float>& vertices) {
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

void loadIndicesIntoEBO(const std::vector<unsigned int>& indices) {
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    indices.size() * sizeof(unsigned int),
    indices.data(),
    GL_STATIC_DRAW
  );
}

void terminate(GPUBuffers& buffers) {
  glDeleteVertexArrays(1, &buffers.VAO);
  glDeleteBuffers(1, &buffers.VBO);
  glDeleteBuffers(1, &buffers.EBO);
  glDeleteProgram(buffers.program);
  glfwTerminate();
}

void clearWindow(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GlfwWindowDestroyer::operator()(GLFWwindow* window) const {
  glfwDestroyWindow(window);
}
