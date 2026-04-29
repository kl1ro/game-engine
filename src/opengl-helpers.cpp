#include "opengl-helpers.hpp"

#include "globals.hpp"
#include "keyboard.hpp"

void initialize() {
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    std::exit(1);
  }

  auto config = getConfig();
  GLFWwindow* window = getWindow(config.window);
  glfwSetKeyCallback(window, onKeyPress);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  GLuint& program = Globals::program;
  program = getGPUprogram(config.triangle.shaders);

  DrawingContext& ctx = Globals::drawingContext;
  ctx.colorLoc = glGetUniformLocation(program, "color");
  ctx.displayModeLoc = glGetUniformLocation(program, "displayMode");
  ctx.modelLoc = glGetUniformLocation(program, "model");
  ctx.viewLoc = glGetUniformLocation(program, "view");
  ctx.projectionLoc = glGetUniformLocation(program, "projection");
}

GLFWwindow* getWindow(WindowConfig config) {
  GLFWwindow*& window = Globals::window;
  if (window)
    return window;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);

  if (!window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    std::exit(1);
  }

  glfwMakeContextCurrent(window);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glfwSwapInterval(1);

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Renderer:       " << glGetString(GL_RENDERER) << std::endl;
  return window;
}

int getStride(const std::vector<VertexAttribute>& vertexAttributes) {
  int stride = 0;
  for (auto& attribute : vertexAttributes) stride += attribute.size;
  return stride;
}

VertexBuffers getEmptyVertexBuffers() {
  VertexBuffers buffers;

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

void loadLightsToGPU() {
  GLuint& program = Globals::program;

  glUniform1i(glGetUniformLocation(program, "numLights"), Globals::scene.lights.size());

  for (int i = 0; i < Globals::scene.lights.size(); i++) {
    std::string base = "lights[" + std::to_string(i) + "]";
    Light& light = Globals::scene.lights[i];

    glUniform3fv(
      glGetUniformLocation(program, (base + ".position").c_str()),
      1,
      glm::value_ptr(light.position)
    );

    glUniform3fv(
      glGetUniformLocation(program, (base + ".color").c_str()),
      1,
      glm::value_ptr(light.color)
    );

    glUniform1f(glGetUniformLocation(program, (base + ".intensity").c_str()), light.intensity);
  }
}

void terminate() {
  glDeleteProgram(Globals::program);

  for (auto& material : Globals::materials) glDeleteTextures(1, &material.diffuseTexture);

  for (auto& object : Globals::scene.objects) {
    for (auto& submesh : object.mesh.submeshes) {
      glDeleteVertexArrays(1, &submesh.buffers.VAO);
      glDeleteBuffers(1, &submesh.buffers.VBO);
      glDeleteBuffers(1, &submesh.buffers.EBO);
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}
