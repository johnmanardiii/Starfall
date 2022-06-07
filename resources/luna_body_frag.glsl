#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
in vec3 lTS;
in vec3 vTS;


uniform float flashAmt;
uniform sampler2D tex;
uniform sampler2D emissiveTex;
uniform sampler2D normalTex;
uniform sampler2D roughness;
uniform sampler2D metal;
uniform vec3 flashCol;

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
    float fresnel = f_fresnel_schlicks(V_H, vertex_normal_n, -H, bias, scale, shn);

    return albedo * max(0.1,(spec_dist + geom_shadow + fresnel));
}

void main()
{
	vec2 texture_uvs = vec2(vertex_tex.x, -vertex_tex.y);
	vec3 n = normalize(vertex_normal);
	
	vec3 tcol= texture(tex, texture_uvs).rgb;
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;

	vec3 normal = texture(normalTex, texture_uvs).xyz;
	normal = (normal * 2) - vec3(1.0);
	//float dCoeff = max(0.0, dot(lTS, normal));
    //vec3 half_dir = normalize(vTS + lTS);
    //float spec = pow(max(dot(normal, half_dir), 0.0), 8);
	//color *= dCoeff + spec;
    float roughness = texture(roughness, texture_uvs).r; //choose a value between (0 and 1] or use a texture. 
    float bias = 0.0, scale = 0.1;
    float shn = 12 * texture(metal, texture_uvs).r;
    color.rgb = brdf(tcol, normal, lTS, vTS, roughness, bias, scale, shn);
    color.r = min(color.r, 1.0);
    color.g = min(color.g, 1.0);
    color.b = min(color.b, 1.0);
	// emissive map
	vec3 ecol= texture(emissiveTex, texture_uvs).rgb;
	color.rgb += ecol * 8;
	// color.rgb = normal;
	color.a = 1;
}