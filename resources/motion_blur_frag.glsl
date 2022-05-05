#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform mat4 VPInverse;
uniform mat4 prevVPInverse;

// code is entirely based on this NVIDIA guide: 
// https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect
vec2 GetVelocity()
{
    // Get the depth buffer value at this pixel.    
    float zOverW = texture(depthTexture, TexCoord).r;
    // H is the viewport position at this pixel in the range -1 to 1.    
    vec4 H = vec4(TexCoord.x * 2 - 1, (1 - TexCoord.y) * 2 - 1, zOverW, 1); 
    // Transform by the view-projection inverse.    
    vec4 D = VPInverse * H;
    // Divide by w to get the world position.    
    vec4 worldPos = D / D.w; 


    // Current viewport position    
    vec4 currentPos = H; 
    // Use the world position, and transform by the previous view-    
    // projection matrix.    
    vec4 previousPos = prevVPInverse * worldPos;
    // Convert to nonhomogeneous points [-1,1] by dividing by w. 
    previousPos /= previousPos.w; 
    // Use this frame's position and last frame's to compute the pixel    
    // velocity.    
    vec2 velocity = ((currentPos - previousPos)/2.0f).xy;
    return velocity;
}


void main()
{
    vec2 velocity = GetVelocity();
    // Get the initial color at this pixel.    
    vec4 blurred_color = texture(screenTexture, TexCoord);
    vec2 uv = TexCoord;
    uv += velocity; 
    for(int i = 1; i < 3; ++i, uv += velocity) 
    {   
        // Sample the color buffer along the velocity vector.    
        vec4 currentColor = texture(screenTexture, uv);   
        // Add the current color to our color sum.   
        blurred_color += currentColor; 
    } // Average all of the samples to get the final blur color.    
    color = blurred_color / 3; 
}