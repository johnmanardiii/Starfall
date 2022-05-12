#version 420 core

uniform sampler2D alphaTexture;

in vec3 partCol;

out vec4 outColor;

uniform float totalTime;

void main()
{
	float alpha = texture(alphaTexture, gl_PointCoord).g;
	if (alpha < 0.7) discard;
	vec3 color = vec3(1.0f, 1.0f, 1.0f);
	outColor = vec4(color, alpha);

}