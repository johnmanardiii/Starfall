#version 410 core

layout (vertices = 3) out;

uniform vec3 campos;
in vec3 vertex_pos[];
void main(void)
{
    vec3 cp = campos;
    float df=75.;
    float dist = df-length(vertex_pos[gl_InvocationID] - cp);
    dist/=df;
    dist = pow(dist,3);
    float tessfact = dist*8;
    clamp(tessfact, 1, 8);
    tessfact = max(1, tessfact);
    gl_TessLevelInner[0] = tessfact;
    gl_TessLevelOuter[0] = tessfact;
    gl_TessLevelOuter[1] = tessfact;
    gl_TessLevelOuter[2] = tessfact;
    // Everybody copies their input to their output
    gl_out[gl_InvocationID].gl_Position =  gl_in[gl_InvocationID].gl_Position;
}