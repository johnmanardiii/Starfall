#version 330 core

uniform sampler2D alphaTexture;

in vec3 partCol;

out vec4 outColor;


void main()
{
	float alpha = texture(alphaTexture, gl_PointCoord).r;
	if (alpha < 0.7) discard;
	outColor = vec4(partCol, alpha);
}
