#version 410 core

uniform sampler2D alphaTexture;

in vec3 partCol;
in vec3 vertex_pos;

uniform vec3 campos;
uniform vec3 centerPos;
uniform float alphaTime;
uniform vec3 Row;
uniform vec3 Column;

out vec4 outColor;

uniform float totalTime;


vec2 selectSprite(float row, float col){
    vec2 texCoord = vec2(gl_PointCoord.x, gl_PointCoord.y); //this is the texture coordinate s[0,1] t[0,1].  - 22/571
    texCoord.x = (texCoord.x /8) + col;
    texCoord.y = (texCoord.y /8) + row;
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
    //vec2 texCoordPrev = selectSprite(Row.x,Column.x); 
    vec2 texCoordCurr = selectSprite(Row.y,Column.y);
    //vec2 texCoordNext = selectSprite(Row.z,Column.z);
    //Based on the total Time, we need to select a sprite.

	//float alphaPrev = texture(alphaTexture, texCoordPrev).g;
    float alphaCurr = texture(alphaTexture, texCoordCurr).a;
    //float alphaNext = texture(alphaTexture, texCoordNext).g;

    
    float alpha = alphaCurr;
    
    //float alpha = alphaPrev + alphaCurr + alphaNext;
	vec3 color = vec3(0.761f, 0.698f, .502f);
    
	//alpha fadeout with distance - this should match the implementation in height_frag.glsl
    float len = length(vertex_pos.xz - campos.xz);
    len -= 41;
    len /= 8.0f;
    len = clamp(len, 0, 1);
    float a = 1 - len;

    
    /* debugging */
    /*
    if (gl_PointCoord.x < 0.5)
	    outColor = vec4(texCoordCurr.x,0,0, 1);
    
    else if (texCoordCurr.y > 1)
        outColor = vec4(1,1,1,1);
    else
        outColor = vec4(0,texCoordCurr.y,0,1);
    */
    outColor = vec4(color, alpha * a * alphaTime);
}
