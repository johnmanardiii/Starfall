#version 330 core


layout(location = 0) in vec3 pColor;
layout(location = 1) in vec3 pNormal;
layout(location = 2) in vec3 pRotation;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;


uniform vec3 centerPos;
uniform float totalTime;

out vec3 partCol;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{

	// Billboarding: set the upper 3x3 to be the identity matrix
	mat4 M0 = M;

	M0[0] = vec4(1.0, 0.0, 0.0, 0.0);
	M0[1] = vec4(0.0, 1.0, 0.0, 0.0);
	M0[2] = vec4(0.0, 0.0, 1.0, 0.0);

	gl_Position = P * V * vec4(centerPos + pNormal, 1.0);

	partCol = pColor;
}