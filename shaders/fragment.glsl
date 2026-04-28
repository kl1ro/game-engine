#version 330 core

uniform sampler2D tex;
uniform int displayMode;
uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

const int MODE_WIREFRAME = 0;
const int MODE_TEXTURED  = 1;
const int MODE_SHADED    = 2;

const float AMBIENT_STRENGTH  = 0.2;
const float SPECULAR_STRENGTH = 0.5;
const float SHININESS         = 32.0;

vec3 calcLighting(vec3 baseRGB) {
	// Ambient
	vec3 ambient = AMBIENT_STRENGTH * lightColor;

	// Diffuse
	vec3 norm     = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff    = max(dot(norm, lightDir), 0.0);
	vec3 diffuse  = diff * lightColor;

	// Specular (Blinn-Phong)
	vec3 viewDir  = normalize(viewPos - FragPos);
	vec3 halfDir  = normalize(lightDir + viewDir);
	float spec    = pow(max(dot(norm, halfDir), 0.0), SHININESS);
	vec3 specular = SPECULAR_STRENGTH * spec * lightColor;

	return (ambient + diffuse + specular) * baseRGB;
}

void main() {
	if (displayMode == MODE_WIREFRAME) {
		FragColor = color;
		return;
	}

	vec4 baseColor = (displayMode != MODE_WIREFRAME) ? texture(tex, TexCoord) : color;

	if (displayMode == MODE_TEXTURED) {
		FragColor = baseColor;
		return;
	}

	FragColor = vec4(calcLighting(baseColor.rgb), baseColor.a);
}
