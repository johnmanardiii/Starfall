#version 330 core
out vec4 color;
in vec3 vertex_normal_n;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 playerPos;
uniform sampler2D starTexture;
uniform vec3 centerPos;
uniform vec3 campos;
uniform vec3 lights[20];

uniform mat4 M;

const float M_PI = 3.14159265;
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
    return clamp(dot(v1,v2),0.001,1.0);
}
//simple phong diffuse calc.
float diffuse(vec3 vertex_normal_n, vec3 lightDir){
    return clamp(dot(vertex_normal_n, lightDir),0.4f,1.0f);
}
//GGX brdf. Takes results from a specular distribution function, a fresnel function, and a geometrix shadowing function,
//as well as N dot L and N dot V.
float f_combined(float spec_dist, float fresnel, float geom_shadow, float N_L, float N_V){
    return clamp( (spec_dist * fresnel * geom_shadow) / (4 * N_L * N_V), 0.2, 1);
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

float f_fresnel_schlicks(float V_H, vec3 vertex_normal_n, vec3 viewDir, float bias, float scale, float shn){
    if(dot(-viewDir, vertex_normal_n) < 0){
        return 0;
    }
    float r = max(0, min(1, bias + scale * pow((1.0 + dot(-viewDir,vertex_normal_n)),shn)));
    return r;
}

//Kelemen approximation of Cook-Torrance Geometric Shadowing Function (GSF).
float f_geom_shadow(float N_V, float N_L, float roughness){
    float k = square(roughness) * M_SQRT_2_PI;
    float gH = N_V * k + (1-k);
    return (square(gH) * N_L);
}

vec3 brdf(vec3 albedo, vec3 vertex_normal_n, vec3 lightDir, vec3 viewDir, float rough, float bias, float scale, float shn){
    
    float N_L = md0(vertex_normal_n, lightDir);
    float N_V = md0(vertex_normal_n, viewDir);
    
    vec3 H = normalize(viewDir + lightDir);
    float N_H = md0(vertex_normal_n, H);
    float V_H = md0(viewDir,H);
    float V_L = dot(vertex_normal_n, lightDir);
    
    
    float roughness = rough * rough;
    float spec_dist = f_spec_dist(roughness, N_H);
    float geom_shadow = f_geom_shadow(N_V,N_L, roughness);
    float fresnel = f_fresnel_schlicks(V_H, vertex_normal_n, -H, bias, scale, shn);

    return albedo * max(0.1,(spec_dist + geom_shadow + fresnel));
}

float luminance(vec3 rgb)
{
    // Algorithm from Chapter 10 of Graphics Shaders.
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    return dot(rgb, W);
}


void main()
{
    vec3 lightPos = (M * vec4(lights[0],1.0f)).xyz;
    
    
    vec3 tcol = texture(starTexture, vertex_tex).rgb;

    //alpha fadeout with distance - this should match the implementation in height_frag.glsl
    float len = length(vertex_pos.xz-playerPos.xz);
	len = abs(len) / 150.0f;
	len=clamp(len,0,1);
	len = pow(len, 5.0f);
    float a = 1 - len;

    //use this to make into specific colors.
    vec3 randCol = vec3(
      abs(0.85f * rand(centerPos.xy)),
      abs(0.85f * rand(centerPos.yz)),
      abs(0.85f * rand(centerPos.xz)));

    randCol /= luminance(randCol);

    //for star fragments, fake roughness.
    float roughness = 0.4; //choose a value between (0 and 1] or use a texture. 
    
    vec3 lightDir = normalize(normalize(lightPos) - normalize(vertex_pos));
    vec3 viewDir = normalize(normalize(campos) - normalize(vertex_pos));
    //bias is a flat adjustment, scale is a linear adjustment, and shn is an exponential adjustment.
    float bias = 0.0, scale = 0.1, shn = 6.0;
    color = vec4(brdf(randCol, vertex_normal_n, lightDir, viewDir, roughness, bias, scale, shn), a);
    //color = vec4(lightDir,a);
}
