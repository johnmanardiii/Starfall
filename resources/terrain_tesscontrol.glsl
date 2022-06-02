#version 410 core

layout (vertices = 3) out;

uniform vec3 campos;
in vec3 vertex_pos[];
void main(void)
{
    vec3 cp = campos; 
    //cp = vec3(0,0,0);
    float df=50.;
    float dist = df-length(vertex_pos[gl_InvocationID] - cp);
    dist/=df;
    dist = pow(dist,3);
    float tessfact = dist*16;
    clamp(tessfact, 1, 16);
    tessfact = max(1, tessfact);
    gl_TessLevelInner[0] = tessfact;
    gl_TessLevelOuter[0] = tessfact;
    gl_TessLevelOuter[1] = tessfact;
    gl_TessLevelOuter[2] = tessfact;
    // Everybody copies their input to their output
    gl_out[gl_InvocationID].gl_Position =  gl_in[gl_InvocationID].gl_Position;
}