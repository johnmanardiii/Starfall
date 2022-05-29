#version 330 core
out vec4 color;
in vec3 vertex_normal_n;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform sampler2D starTexture;
uniform vec3 centerPos;
uniform vec3 campos;
uniform vec3 lights[20];

const float M_PI = 3.14159265;
const float M_1_PI = 1.0f/M_PI;
const float M_SQRT_2_PI = sqrt(2.0f / M_PI);

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
//utility function to prevent mistyping this common pattern.
float square(float val){
    return val * val;
}
//utility function to prevent mistyping this common pattern.
float md0(vec3 v1, vec3 v2){
    return max(0.0f,dot(v1,v2));
}
//simple phong diffuse calc.
float diffuse(vec3 vertex_normal_n, vec3 lightDir){
    return clamp(dot(vertex_normal_n, lightDir),0.4f,1.0f);
}
//GGX brdf. Takes results from a specular distribution function, a fresnel function, and a geometrix shadowing function,
//as well as N dot L and N dot V.
float f_combined(float spec_dist, float fresnel, float geom_shadow, float N_L, float N_V){
    return clamp( (spec_dist * fresnel * geom_shadow) / (4 * N_L * N_V), 0, 1);
}
//GGX microfacet normal distribution function (NDF)
float f_spec_dist(float roughness, float N_H){
    float r_sq = square(roughness);
    float d = ((N_H * r_sq - N_H) * N_H + 1);
    return r_sq / (square(d) * M_PI);
}
//spherical-gaussian fresnel approximation
float f_fresnel(float specular_color, float V_H){
    float power = ((-5.55473 * V_H) - 6.98316) * V_H;
    return specular_color + (1 - specular_color) * pow(2,power);
}
//Kelemen approximation of Cook-Torrance Geometric Shadowing Function (GSF).
float f_geom_shadow(float N_V, float N_L, float roughness){
    float k = square(roughness) * M_SQRT_2_PI;
    float gH = N_V * k + (1-k);
    return (square(gH) * N_L);
}

float brdf(vec3 vertex_normal_n, vec3 lightDir, vec3 viewDir, float rough){
    
    float N_L = md0(vertex_normal_n, lightDir);
    float N_V = md0(vertex_normal_n, viewDir);
    
    vec3 H = normalize(viewDir + lightDir);
    float N_H = md0(vertex_normal_n, H);
    float V_H = md0(viewDir,H);
    float L_H = md0(lightDir,H);

    float L_V = md0(lightDir,viewDir);
    float R_V = md0(reflect(-lightDir, vertex_normal_n),viewDir);

    float roughness = rough;
    float spec_dist = f_spec_dist(roughness, N_H);
    //if you want to test stuff, return individual floating point values here!
    //return spec_dist;
    //return f_fresnel(spec_dist, N_L);
    //return f_geom_shadow(N_V, N_L, roughness);
    return clamp(spec_dist + f_fresnel(spec_dist, V_H) + f_geom_shadow(N_V, N_L, roughness), 0,1.1);
    //return f_combined(spec_dist, f_fresnel(spec_dist, V_H), f_geom_shadow(N_V, N_L, roughness), N_L, N_V);
}

void main()
{
    vec3 lightPos = lights[0];
    
    
    vec3 tcol = texture(starTexture, vertex_tex).rgb;

    //alpha fadeout with distance - this should match the implementation in height_frag.glsl
    float len = length(vertex_pos.xz - campos.xz);
    len -= 41;
    len /= 8.0f;
    len = clamp(len, 0, 1);
    float a = 1 - len;
    
    //use this to make into specific colors.
    vec3 randCol = vec3(
      abs(0.65f * rand(centerPos.xy)),
      abs(0.65f * rand(centerPos.yz)),
      abs(0.65f * rand(centerPos.xz)));

    //for star fragments, fake roughness.
    float roughness = 0.1; //choose a value between (0 and 1] or use a texture. 
    
    vec3 lightDir = normalize(normalize(lightPos) - normalize(vertex_pos));
    vec3 viewDir = normalize(campos - vertex_pos);
    color = vec4(tcol * brdf(vertex_normal_n, lightDir, viewDir, roughness), a);
    //color = vec4(lightDir,a);
}
