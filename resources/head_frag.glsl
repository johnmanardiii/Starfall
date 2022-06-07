#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform float flashAmt;
uniform sampler2D tex;
uniform vec3 flashCol;
uniform vec2 eye1Pos;
uniform float eye1Radius;
uniform float eyeOpenPct;
uniform vec3 camPos;
uniform vec3 lightDir;

const float M_PI = 3.14159265;
const float M_SQRT_2_PI = sqrt(2.0f / M_PI);

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
    float fresnel = 0;//f_fresnel_schlicks(V_H, vertex_normal_n, -H, bias, scale, shn);

    return albedo * max(0.1,(spec_dist + geom_shadow + fresnel));
}

bool isUVInEye(vec2 eyePos, vec2 uv)
{
	float yRadius = 3.0 * eye1Radius;	// eclipse scale
	yRadius *= eyeOpenPct;
	float xdist = ((uv.x - eyePos.x) * (uv.x - eyePos.x)) / (eye1Radius * eye1Radius);
	float ydist = ((uv.y - eyePos.y) * (uv.y - eyePos.y)) / (yRadius * yRadius);
	return xdist + ydist <= 1;
}

vec3 draw_eyes(vec3 base_color)
{
	float radius = eye1Radius;
	vec2 eye2Pos = vec2(1 - eye1Pos.x, eye1Pos.y);
	vec3 eye_color = vec3(.28, .88, 1) * 1.4;
	// TODO: generalize this and remove if statement
	if(isUVInEye(eye1Pos, vertex_tex) || isUVInEye(eye2Pos, vertex_tex))
	{
		return eye_color;
	}
	else
	{
		return base_color;
	}
}

void main()
{
	vec3 n = normalize(vertex_normal);
	// vec3 tcol= texture(tex, texture_uvs).rgb;
	vec3 tcol = vec3(1);	// this is the case since the head has weird UVs and needs its own individual tex.
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
    float bias = 0.0, scale = 0.15, shn = 6.0;
    float roughness = 0.1;
    vec3 viewDir = normalize(camPos - vertex_pos);
    color.rgb = brdf(tcol, n, lightDir, viewDir, roughness, bias, scale, shn);
    color.r = min(color.r, 1.0);
    color.g = min(color.g, 1.0);
    color.b = min(color.b, 1.0);
	color.rgb = draw_eyes(color.rgb);
	// color.rgb = vec3(vertex_tex / 2, 0);
	// color.rgb = vec3(vertex_tex, 0);
	color.a = 1;
}