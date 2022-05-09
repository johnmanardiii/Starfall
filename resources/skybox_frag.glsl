#version 330 core
out vec4 color;

in vec3 TexCoords;
in vec2 uv;

uniform samplerCube skybox;

vec3 dayBottomColor = vec3(1, 0, 0), dayTopColor = vec3(0, 1, 0);

void main() {
  vec3 normTexCoords = normalize(TexCoords);
  color = vec4(1, TexCoords.y, 1, 1);
  vec3 mixed = mix(dayBottomColor, dayTopColor, (normTexCoords.y + 1) / 2);
  color = vec4(mixed, 1.0);
}
