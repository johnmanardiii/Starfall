#version 330 core
layout (location = 0) in vec3 vertPos;

out vec3 TexCoords;
out vec2 uv;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main() {
	TexCoords = normalize(vertPos);
	uv.x = atan(TexCoords.x, TexCoords.z) / 1;
	uv.y = asin(TexCoords.y) / 0.5;
	gl_Position = P*V*M*vec4(vertPos.xyz, 1.0);
}
