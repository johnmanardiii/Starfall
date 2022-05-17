#version 410 core

uniform sampler2D alphaTexture;
uniform sampler2D rainbowTexture;
in vec3 partCol;

out vec4 outColor;

uniform float alphaMult;
uniform float totalTime;

void main()
{
	float alpha = texture(alphaTexture, gl_PointCoord).r;
	if (alpha < 0.7) discard;
	vec3 color = texture(rainbowTexture, partCol.yz * 8).xyz;
	outColor = vec4((0.2 * totalTime * totalTime) + color, alpha * alphaMult);
	outColor.rgb = outColor.rgb * 1.4;
}
