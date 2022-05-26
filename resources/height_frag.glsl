#version 410 core
out vec4 color;
in vec3 frag_pos;
in vec2 frag_tex;
in vec3 frag_norm;
in float frag_height;
in vec3 frag_col;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D noiseTex;
uniform sampler2D sandShallow;
uniform sampler2D sandSteep;

uniform vec3 camoff;
uniform vec3 campos;
uniform vec3 lightDir;
uniform mat4 V;
uniform float time;

// diffuse
float diffuseContrast = 2;
vec3 shadowColor = vec3(0.611, 0.44, 0.32);
vec3 terrainColor = vec3(1, 0.79, 0.65);
float sandStrength = 0.1;

// rim 
float rimStrength = 0.3;
float rimPower = 20;
vec3 rimColor = vec3(1, 0.9, 0.94);

// ocean spec
float oceanSpecularStrength = 0.3;
float oceanSpecularPower = 100;
vec3 oceanSpecularColor = vec3(1, 0.9, 0.94);

// sand ripples
float steepnessSharpnessPower = 20.0;
float specularHardness = 2.0;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

// like slerp
vec3 nlerp(vec3 n1, vec3 n2, float t) {
	return normalize(mix(n1, n2, t));
}

vec3 DiffuseColor(vec3 normal, vec3 ligthDir)
{
	float NdotL = max(0, diffuseContrast * dot(normal, lightDir));
	vec3 color = mix(shadowColor, terrainColor, NdotL);
	return color;
}

vec3 SandNormal(vec3 normal, vec2 texcoords)
{
	vec3 random = texture(noiseTex, texcoords * 50).rgb;
	random = normalize(random * 2 - 1); // [-1 to 1]
	vec3 Ns = nlerp(normal, random, sandStrength);
	return Ns;
}

vec3 OceanSpecular(vec3 normal, vec3 lightDir, vec3 view)
{
	// Specular blinn phong
	vec3 H = normalize(view + lightDir);
	float NdotH = max(0, dot(normal, H));
	float spec = pow(NdotH, oceanSpecularPower) * oceanSpecularStrength;
	return spec * oceanSpecularColor;
}

vec3 RimLighting(vec3 normal, vec3 viewDir)
{
	// Fresnal lighting equation
	float rim = 1.0 - clamp(dot(normal, viewDir), 0, 1);
	rim = clamp(pow(rim, rimPower) * rimStrength, 0, 1); // Strength
	rim = max(rim, 0);
	return rim * rimColor;
}

vec3 SandRipples(vec2 texcoords, vec3 lightDir, vec3 view)
{
	// Sand ripples
	float steepness = clamp(dot(frag_norm, vec3(0, 1, 0)), 0, 1);
	steepness = pow(steepness, steepnessSharpnessPower);

	vec3 shallow_x = texture(sandShallow, texcoords * 50).rgb;
	vec3 steep_x = texture(sandSteep, texcoords * 50).rgb;
	vec3 S_x = nlerp(steep_x, shallow_x, steepness);

	

	vec2 flippedTexcoords = vec2(texcoords.y, texcoords.x);
	/*
	float steepness_z = clamp(dot(frag_norm, vec3(0, 0, 1)), 0, 1);
	steepness_z = pow(steepness_x, steepnessSharpnessPower);
	
	vec3 shallow_z = texture(sandShallow, flippedTexcoords * 30).rgb;
	vec3 steep_z = texture(sandSteep, flippedTexcoords * 30).rgb;
	vec3 S_z = nlerp(steep_z, shallow_z, steepness_z);

	float dir = clamp(dot(frag_norm, vec3(1, 0, 0)), 0, 1);
	dir += clamp(dot(frag_norm, vec3(-1, 0, 0)), 0, 1);
	dir = clamp(pow(dir, 3), 0, 1);
	dir = clamp(dir * 10, 0, 1);
	
	vec3 S = nlerp(S_x, S_z, dir);
	S = S_z * dir + (S_x * (1 - dir));
	*/

	vec3 S = S_x;
	float NdotL = max(0, diffuseContrast * dot(S, lightDir));
	vec3 color = mix(shadowColor, terrainColor, NdotL);
	return color;
}


void main()
{
	vec2 texcoords=frag_tex;
	float t=1./100.;
	texcoords -= vec2(camoff.x,camoff.z)*t;

	float len = length(frag_pos.xz-campos.xz);
	len-=41;
	len/=8.;
	len=clamp(len,0,1);
	
	// Sand normals
	vec3 sandNormal = SandNormal(frag_norm, texcoords) * (1 - len);
	vec3 view = normalize(vec3(V[0][2], V[1][2], V[2][2]));

	// Specular
	vec3 rim = RimLighting(frag_norm, view);
	vec3 oceanSpecColor = OceanSpecular(frag_norm, lightDir, view);
	vec3 spec = clamp(max(rim, oceanSpecColor), vec3(0), vec3(1));

	// Combining
	vec3 sandRipplesColor = SandRipples(texcoords, lightDir, view);
	vec3 diffuseColor = DiffuseColor(sandNormal, lightDir);
	//diffuseColor = mix(diffuseColor, sandRipplesColor * 0.5, 0.4);
	color.rgb = spec + diffuseColor * 0.7 * sandRipplesColor;
	
	color.a=1-len;
	//color.rgb = frag_col;
	//color.a = 1;
	
}
