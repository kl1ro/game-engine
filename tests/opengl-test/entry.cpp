#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

const int WIDTH  = 800;
const int HEIGHT = 600;

std::string readFile(const std::string& path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << path << "\n";
		std::exit(1);
	}

	std::stringstream buf;
	buf << file.rdbuf();
	return buf.str();
}

GLuint compileShader(GLenum type, const std::string& src) {
	const char* cstr = src.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &cstr, nullptr);
	glCompileShader(shader);
	GLint ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);

	if (!ok) {
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cerr << "Shader error: " << log << "\n";
		std::exit(1);
	}

	return shader;
}

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
		std::cerr << "Link error: " << log << "\n";
		std::exit(1);
	}

	return prog;
}

int main() {
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

	if (!glfwInit()) {
		std::cerr << "Failed to init GLFW\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Triangle", nullptr, nullptr);

	if (!window) {
		std::cerr << "Failed to create window\n";
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";

	float vertices[] = {
		// position          // color
		 0.0f,  0.6f, 0.0f,  1.0f, 0.2f, 0.2f,
		-0.5f, -0.4f, 0.0f,  0.2f, 1.0f, 0.2f,
		 0.5f, -0.4f, 0.0f,  0.2f, 0.4f, 1.0f,
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Load shaders from files
	std::string vertSrc = readFile("tests/opengl-test/vertex.glsl");
	std::string fragSrc = readFile("tests/opengl-test/fragment.glsl");

	GLuint vert = compileShader(GL_VERTEX_SHADER,   vertSrc);
	GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
	GLuint prog = linkProgram(vert, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

	double lastTime   = glfwGetTime();
	int    frameCount = 0;

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		double now     = glfwGetTime();
		double elapsed = now - lastTime;
		frameCount++;

		if (elapsed >= 1.0) {
			double fps = frameCount / elapsed;
			std::string title = "OpenGL Triangle  |  FPS: " + std::to_string((int)fps);
			glfwSetWindowTitle(window, title.c_str());
			frameCount = 0;
			lastTime   = now;
		}

		glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(prog);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(prog);
	glfwTerminate();
	return 0;
}
