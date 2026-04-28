#version 330 core

in vec2 vertUV;
uniform sampler2D tex;
out vec4 fragColor;

void main() {
  fragColor = texture(tex, vertUV);
}
