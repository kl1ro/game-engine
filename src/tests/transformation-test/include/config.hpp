#pragma once
#include <string>
#include <fstream>
#include <GL/gl.h>
#include <nlohmann/json.hpp>

struct WindowConfig {
	int width;
	int height;
	std::string title;
};

struct ShadersConfig {
	std::string vertex;
	std::string fragment;
};

struct VertexAttribute {
	GLuint index;
	GLint  size;
	GLint  offset;
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(VertexAttribute, index, size, offset)
};

struct TriangleConfig {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<VertexAttribute> attributes;
	ShadersConfig shaders;
	std::string textureSrc;
};

struct Config {
	WindowConfig window;
	TriangleConfig triangle;
};

Config getConfig(std::string path = "config.json");
