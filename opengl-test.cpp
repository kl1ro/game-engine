#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

// Window dimensions
const int WIDTH  = 800;
const int HEIGHT = 600;
const char* TITLE = "OpenGL Triangle – Test";

// ── Shaders ──────────────────────────────────────────────────────────────────

const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertColor = aColor;
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
in  vec3 vertColor;
out vec4 fragColor;

void main() {
    fragColor = vec4(vertColor, 1.0);
}
)";

// ── Helper: compile a shader and check for errors ─────────────────────────────

GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << "\n";
        std::exit(1);
    }
    return shader;
}

// ── Helper: link a program and check for errors ───────────────────────────────

GLuint linkProgram(GLuint vert, GLuint frag) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(prog, 512, nullptr, log);
        std::cerr << "Program link error:\n" << log << "\n";
        std::exit(1);
    }
    return prog;
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
		glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return 1;
    }

    // Request OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Init GLEW (loads OpenGL function pointers)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        return 1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";

    // ── Geometry: one RGB triangle in NDC ─────────────────────────────────────
    //   position (xyz)          color (rgb)
    float vertices[] = {
        // x      y     z      r     g     b
         0.0f,  0.6f, 0.0f,  1.0f, 0.2f, 0.2f,   // top    – red
        -0.5f, -0.4f, 0.0f,  0.2f, 1.0f, 0.2f,   // bottom left  – green
         0.5f, -0.4f, 0.0f,  0.2f, 0.4f, 1.0f,   // bottom right – blue
    };

    // Create VAO + VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute  (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute  (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ── Shader program ────────────────────────────────────────────────────────
    GLuint vert = compileShader(GL_VERTEX_SHADER,   vertexShaderSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint prog = linkProgram(vert, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    // ── Render loop ───────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window)) {
        // Close on Escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(prog);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(prog);
    glfwTerminate();
    return 0;
}
