#version 330 core

uniform sampler2D tex;
uniform bool useTexture;
uniform vec4 color;

in vec2 TexCoord;
out vec4 FragColor;

void main() {
	if (useTexture) {
		FragColor = texture(tex, TexCoord);
	} else {
		FragColor = color;
	}
}