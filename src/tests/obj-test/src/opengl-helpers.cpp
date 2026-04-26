#define STB_IMAGE_IMPLEMENTATION
#include "opengl-helpers.hpp"

#include "globals.hpp"

void initialize() {
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    std::exit(1);
  }

  auto config = getConfig();
  GLFWwindow* window = getWindow(config.window);
  glfwSetCursorPosCallback(window, moveMouse);

  GPUbuffers& gpuBuffers = getEmptyGPUbuffers();

  gpuBuffers.program =
    getGPUprogram(config.triangle.shaders.vertex, config.triangle.shaders.fragment);

  DrawingContext& ctx = Globals::drawingContext;
  ctx.colorLoc = glGetUniformLocation(gpuBuffers.program, "color");
}

GLFWwindow* getWindow(WindowConfig config) {
  GLFWwindow*& window = Globals::window;
  if (window)
    return window;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);

  if (!window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    std::exit(1);
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

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

GPUbuffers& getEmptyGPUbuffers() {
  GPUbuffers& buffers = Globals::gpuBuffers;
  if (buffers.VAO)
    return buffers;

  glGenVertexArrays(1, &buffers.VAO);
  glGenBuffers(1, &buffers.VBO);
  glGenBuffers(1, &buffers.EBO);
  glGenTextures(1, &buffers.texture);
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

void loadTexture(const std::string& path, const GLuint& buffer) {
  int width, height, channels;

  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (!data) {
    std::cerr << "Failed to load texture: " << path << "\n";
    std::exit(1);
  }

  glBindTexture(GL_TEXTURE_2D, buffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

void terminate() {
  GPUbuffers& buffers = Globals::gpuBuffers;
  glDeleteVertexArrays(1, &buffers.VAO);
  glDeleteBuffers(1, &buffers.VBO);
  glDeleteBuffers(1, &buffers.EBO);
  glDeleteTextures(1, &buffers.texture);
  glDeleteProgram(buffers.program);
  glfwTerminate();
}

void clearWindow(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void drawMesh() {
  DrawingContext& ctx = Globals::drawingContext;
  glUniform4f(ctx.colorLoc, ctx.rgba.r, ctx.rgba.g, ctx.rgba.b, ctx.rgba.a);
  glPolygonMode(GL_FRONT_AND_BACK, ctx.mode);
  glDrawElements(GL_TRIANGLES, ctx.indexCount, GL_UNSIGNED_INT, 0);
}
