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
const float IMAGE_WIDTH_PIX = 1002, IMAGE_HEIGHT_PIX = 571; //truly horrible numbers
//width and height of a single sprite in pixels
const int SPRITE_WIDTH_PIX = 100, SPRITE_HEIGHT_PIX = 100;
//horizontal and vertical spacing that exists inbetween each sprite
const int WIDTH_SPACING_PIX = 28, HEIGHT_SPACING_PIX = 27; //seriously, why? they don't even add up to the image size!

//texture space offsets.
const float WIDTH_OFF_TEX = (SPRITE_WIDTH_PIX + WIDTH_SPACING_PIX) / IMAGE_WIDTH_PIX;
const float HEIGHT_OFF_TEX = (SPRITE_HEIGHT_PIX + HEIGHT_SPACING_PIX) / IMAGE_HEIGHT_PIX;

//the scale to convert each to texture space;
const float   WIDTH_SCALE = 1/IMAGE_WIDTH_PIX, 
            HEIGHT_SCALE = 1/IMAGE_HEIGHT_PIX;



//given a row and a column representing the sprite image to render, convert
//a gl_PointCoord to just map to the sprite image in the sheet to render.
vec2 selectSprite(int row, int col){
    vec2 texCoord = vec2(gl_PointCoord.x, gl_PointCoord.y); //this is the texture coordinate s[0,1] t[0,1].  - 22/571
    texCoord.x = (texCoord.x /8) + WIDTH_OFF_TEX * col;
    texCoord.y = texCoord.y /5 + HEIGHT_OFF_TEX * row;
    return texCoord;
}

//float mix3(float v1, float v2, float v3, float spriteTime){
//    float w1 = 1 - spriteTime;
//    float w3 = 0;//clamp((spriteTime),0,1);
//    float w2 = 3 - (w1 + w3);
//    return ((v1 * w1) + (v2 * w2) + (v3 + w3))/3.0f;
//}

void main()
{
    //interpolate between next, current, and previous texture images.
    vec2 texCoordPrev = selectSprite(Row.x,Column.x); 
    vec2 texCoordCurr = selectSprite(Row.y,Column.y);
    vec2 texCoordNext = selectSprite(Row.z,Column.z);
    //Based on the total Time, we need to select a sprite.

	float alphaPrev = texture(alphaTexture, texCoordPrev).g;
    float alphaCurr = texture(alphaTexture, texCoordCurr).g;
    float alphaNext = texture(alphaTexture, texCoordNext).g;

    //4 second lifetime, meaning totalTime is [0-4), frame is [0-40)
    //one current frame lasts 0.4s.
    float spriteTime = 2.5f * (mod(totalTime, 0.4)); //[0-1) 
    //mix based on this information.
    float alpha = alphaCurr / 2.0f;//(alphaPrev + alphaCurr + alphaNext)/3.0f; //mix(alphaCurr, alphaNext, totalTime);
    
    //float alpha = alphaPrev + alphaCurr + alphaNext;
	vec3 color = vec3(0.761f, 0.698f, .502f);
    
	//alpha fadeout with distance - this should match the implementation in height_frag.glsl
    float len = length(vertex_pos.xz - campos.xz);
    len -= 41;
    len /= 8.0f;
    len = clamp(len, 0, 1);
    float a = 1 - len;

	outColor = vec4(color, alpha * a);
}
