#version 410 core

uniform sampler2D alphaTexture;

in vec3 partCol;
in vec3 vertex_pos;

uniform vec3 campos;
uniform vec3 centerPos;

uniform ivec3 Row;
uniform ivec3 Column;

out vec4 outColor;

uniform float totalTime;

//constants used for texturing the specific sprite sheet.
//total image width and height in pixels
const int IMAGE_WIDTH_PIX = 1002, IMAGE_HEIGHT_PIX = 571; //truly horrible numbers
//width and height of a single sprite in pixels
const int SPRITE_WIDTH_PIX = 100, SPRITE_HEIGHT_PIX = 100;
//horizontal and vertical spacing that exists inbetween each sprite
const int WIDTH_SPACING_PIX = 28, HEIGHT_SPACING_PIX = 27; //seriously, why


const float WIDTH_OFF_TEX = (SPRITE_WIDTH_PIX + WIDTH_SPACING_PIX) / IMAGE_WIDTH_PIX;
const int HEIGHT_OFF_TEX = (SPRITE_HEIGHT_PIX + HEIGHT_SPACING_PIX) / IMAGE_HEIGHT_PIX;

void main()
{
    vec2 texCoord = vec2(gl_PointCoord.x,gl_PointCoord.y); //this is the texture coordinate s[0,1] t[0,1].
    //Based on the total Time, we need to select a sprite.

	float alpha = texture(alphaTexture, texCoord).g;
	vec3 color = vec3(0.761f, 0.698f, .502f);
    
	//alpha fadeout with distance - this should match the implementation in height_frag.glsl
    float len = length(vertex_pos.xz - campos.xz);
    len -= 41;
    len /= 8.0f;
    len = clamp(len, 0, 1);
    float a = 1 - len;

	outColor = vec4(color, alpha * a);

}
