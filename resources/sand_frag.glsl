#version 420 core

uniform sampler2D alphaTexture;

in vec3 partCol;
in vec3 vertex_pos;
uniform vec3 campos;
uniform vec3 centerPos;
out vec4 outColor;

uniform float totalTime;

void main()
{
	float alpha = texture(alphaTexture, gl_PointCoord).g;
	if (alpha < 0.7) discard;
	vec3 color = vec3(0.761f, 0.698f, .502f);
    
	//alpha fadeout with distance - this should match the implementation in height_frag.glsl
    float len = length(vertex_pos.xz - campos.xz);
    len -= 41;
    len /= 8.0f;
    len = clamp(len, 0, 1);
    float a = 1 - len;

	outColor = vec4(color, alpha * a);

}
