#version 410 core
out vec4 color;
in vec3 frag_pos;
in vec2 frag_tex;
in vec3 frag_norm;
in float vertex_height;

uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D noiseTex;
uniform vec3 camoff;
uniform vec3 campos;
uniform vec3 lightDir;
uniform mat4 V;

float diffuseContrast = 2;
vec3 shadowColor = vec3(0.611, 0.44, 0.32);
vec3 terrainColor = vec3(1, 0.79, 0.65);
float sandStrength = 0.2;

float rimStrength = 0.3;
float rimPower = 20;
vec3 rimColor = vec3(1, 0.79, 0.94);


float oceanSpecularStrength = 0.3;
float oceanSpecularPower = 100;
vec3 oceanSpecularColor = vec3(1, 0.79, 0.94);


float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

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
	random = normalize(random * 2 - 1);
	vec3 Ns = nlerp(normal, random, sandStrength);
	return Ns;
}

vec3 OceanSpecular(vec3 normal, vec3 lightDir, vec3 view)
{
	vec3 H = normalize(-view + lightDir);
	float NdotH = max(0, dot(normal, H));
	float spec = pow(NdotH, oceanSpecularPower) * oceanSpecularStrength;
	return spec * oceanSpecularColor;
}

vec3 RimLighting(vec3 normal, vec3 viewDir)
{
	float rim = 1.0 - clamp(dot(normal, viewDir), 0, 1);
	rim = clamp(pow(rim, rimPower) * rimStrength, 0, 1);
	rim = max(rim, 0);
	return rim * rimColor;
}


void main()
{
	vec2 texcoords=frag_tex;
	float t=1./100.;
	texcoords -= vec2(camoff.x,camoff.z)*t;

	color.rgb = texture(tex2, texcoords*20).rgb;// * ((vertex_height + 10) / 20);
	color.a=1;

	
	vec3 sandNormal = SandNormal(frag_norm, texcoords);
	vec3 view = normalize(vec3(V[0][2], V[1][2], V[2][2]));
	vec3 rim = RimLighting(frag_norm, view);
	vec3 oceanSpecColor = OceanSpecular(frag_norm, lightDir, -view);

	vec3 spec = clamp(max(rim, oceanSpecColor), vec3(0), vec3(1));

	vec3 diffuseColor = DiffuseColor(sandNormal, lightDir);
	color.rgb = spec + diffuseColor * 0.7;


	//color.rgb = vec3(1, 0, 0);

	
	float len = length(frag_pos.xz+campos.xz);
	len-=41;
	len/=8.;
	len=clamp(len,0,1);
	color.a=1-len;
	
}
