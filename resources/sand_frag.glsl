#version 420 core

uniform sampler2D alphaTexture;

in vec3 partCol;

out vec4 outColor;

uniform float alphaMult;
uniform float totalTime;

void main()
{
	float alpha = texture(alphaTexture, gl_PointCoord).r;
	if (alpha < 0.7) discard;
	vec3 color = vec3(0.0f, 0.0f, 1.0f);
	outColor = vec4((0.2 * totalTime * totalTime) + color, alpha * alphaMult);
	outColor.rgb = outColor.rgb * 1;
}