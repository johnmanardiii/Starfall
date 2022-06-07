#version 330 core

/* write out depth to show transforms working */
out vec4 Outcolor;
uniform int castShadows;

void main() {
	Outcolor = vec4(1, gl_FragCoord.z, gl_FragCoord.z, 1.0);
	Outcolor = vec4(gl_FragCoord.z, castShadows, gl_FragCoord.z, 1.0);
}

