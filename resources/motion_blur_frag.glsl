#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform mat4 currPVInverse;
uniform mat4 prevPV;

const int num_blur_samples = 5;

// code is entirely based on this NVIDIA guide: 
// https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect
vec2 GetVelocity()
{
    // Get the depth buffer value at this pixel.    John: this is in alpha range [0-1]
    float zOverW = texture(depthTexture, TexCoord).r;
    // H is the viewport position at this pixel in the range -1 to 1.    
    vec4 H = vec4(TexCoord.x * 2 - 1, (1 - TexCoord.y) * 2 - 1, zOverW, 1); // John: [0,1]-> [-1,1]
    // Transform by the view-projection inverse.    
    vec4 D = currPVInverse * H; // John: this brings it back into world space
    // Divide by w to get the world position (of the current frame).    John: Undo the perspective divide
    vec4 worldPos = D / D.w; 


    // Current viewport position    John:  the current value from [-1,1] stored inside the texture
    vec4 currentPos = H; 
    // Use the world position, and transform by the previous view-    
    // projection matrix.    
    vec4 previousPos = prevPV * worldPos;
    // Convert to nonhomogeneous points [-1,1] by dividing by w. (manually do the perspective divide)
    previousPos /= previousPos.w; 
    // Use this frame's position and last frame's to compute the pixel    
    // velocity (this is in clip space coordinates)
    vec2 velocity = ((currentPos - previousPos)).xy;
    return velocity;
}

// THINGS TO TRY:
// 1. SIMPLE RADIAL BLUR
// 2. ONLY TRANSLATIONAL CAMERA MOTION BLUR
// Q FOR PROFESSOR WOOD: Can I switch to adding Deferred Shading as one of my topics since I am not sure if 
//                       Instead of camera movement (I don't feel like mine has been enough work to be a tech)?
void main()
{
    vec2 velocity = GetVelocity(); // reduce velocity to lower motion blur effect here since
    // Get the initial color at this pixel.    
    vec4 blurred_color = texture(screenTexture, TexCoord);
    vec4 original_color = blurred_color;
    vec2 uv = TexCoord;
    uv += velocity; 
    float num_added_samples = 1.0f;
    for(int i = 1; i < num_blur_samples; ++i, uv += velocity) 
    {   
        // Sample the color buffer along the velocity vector.  
        if(uv.x > 0 && uv.x < 1 && uv.y > 0 && uv.y < 1)
        {
            vec4 currentColor = texture(screenTexture, uv);   
            // Add the current color to our color sum.   
            blurred_color += currentColor; 
            // add one to the total divisor
            num_added_samples += 1.0f;
        }
    } // Average all of the samples to get the final blur color.    
    blurred_color = blurred_color / num_added_samples; 

    // John Code: Lerp between the original image and the motion blurred image for 
    //            less motion blur around the center of the screen

    // QUESTION: DOES BLOOM HAPPEN ON THE MOTION BLURRED IMAGE OR LIKE THE UNBLURRED ONE?

    // calculate the percentage distance away from the center of the screen (0.5, 0.5)
    // and then divide that by a set amount of distance. (.7071 is max)
    // float pctBlurred = distance(TexCoord, vec2(.5, .5)) / .7071;
    // color = mix(original_color, blurred_color, pctBlurred);
    color = blurred_color; // enable this to make you sick enough to beleive that I put in motion blur.
}