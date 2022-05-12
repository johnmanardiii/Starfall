#version 330 core
out vec4 color;

in vec3 TexCoords;
in vec2 uv;

uniform sampler2D cloudBaseNoise;
uniform sampler2D cloudNoise;
uniform sampler2D cloudDistort;

// Stars
uniform samplerCube skybox;

// Gradient Sky
uniform vec3 dayBottomColor;
uniform vec3 dayTopColor;
uniform vec3 nightBottomColor;
uniform vec3 nightTopColor;
uniform vec3 horizonColor;

// Sun + Moon
uniform vec3 sunDir;
uniform float moonOffset;

// Clouds
uniform float cloudScale;
uniform float cloudSpeed;
uniform float cloudCutoff;
uniform float cloudFuzziness;
uniform vec3 cloudColorDayEdge;
uniform vec3 cloudColorDayMain;
uniform vec3 cloudColorNightEdge;
uniform vec3 cloudColorNightMain;


// Time 
uniform float time;

vec3 SkyGradient(vec3 texCoords, float yCoord)
{
	// gradient
	vec3 normalizedSun = normalize(sunDir);
	yCoord = clamp(yCoord, 0, 1);
	vec3 gradientDay = mix(dayTopColor, dayBottomColor, yCoord);
	vec3 gradientNight = mix(nightTopColor, nightBottomColor, yCoord);

	// mix night and day colors
	vec3 skyGradients = mix(clamp(gradientDay, 0, 1), clamp(gradientNight, 0, 1), normalizedSun.y);
	skyGradients = clamp(skyGradients, 0, 1);
	return skyGradients;
}

vec3 SunAndMoon(vec3 texCoords, float yCoord)
{
	vec3 normalizedSun = normalize(sunDir);
	float sun = distance(texCoords, normalizedSun);
	// 13 sun size hard coded for now ;u;
	float sunDisc = (1 - clamp(sun * 13, 0, 1)) * 100;

	vec3 normalizedMoon = -normalize(sunDir);
	float moon = distance(texCoords, normalizedMoon);
	float moonDisc = clamp((1 - clamp(moon * 13, 0, 1)) * 100, 0, 2);

	float moonCrest = distance(vec3(texCoords.x + moonOffset, texCoords.yz), normalizedMoon);
	float moonDiscCrest = clamp((1 - clamp(moonCrest * 13, 0, 1)) * 100, 0, 2);
	moonDisc -= moonDiscCrest;
	moonDisc = clamp(moonDisc, 0, 2);
	return vec3(sunDisc) + vec3(moonDisc);
}

vec3 HorizonColor(vec3 texCoords, float yCoord)
{
	float horizon = clamp(abs(texCoords.y), 0, 1);
	vec3 normalizedSun = normalize(sunDir);
	float horizonGlow = clamp((1 - horizon) * clamp(normalizedSun.y, 0, 1), 0, 1);
	return vec3((1 - horizonGlow) * horizonColor * 0.5);
}

vec3 Clouds(vec3 texCoords, float yCoord)
{
	vec3 normalizedSun = normalize(sunDir);
	vec2 skyUV = texCoords.xz /texCoords.y;

	vec3 baseNoise = texture(cloudBaseNoise, (skyUV - time * cloudSpeed) * cloudScale).rgb;
	vec3 noise1 = texture(cloudDistort, (skyUV + baseNoise.r - time * cloudSpeed) * cloudScale).rgb;
	vec3 noise2 = texture(cloudNoise, (skyUV + noise1.r - time * cloudSpeed) * cloudScale).rgb;

	float finalNoise = clamp(noise1.r * noise2.r, 0, 1) * 1 - clamp((texCoords.y + 0.3) * 1.5, 0, 1);
	float clouds = clamp(smoothstep(cloudCutoff, cloudCutoff + cloudFuzziness, finalNoise), 0, 1);

	vec3 cloudsColoredDay = mix(cloudColorDayEdge,  cloudColorDayMain , clouds) * clouds;
	vec3 cloudsColoredNight = mix(cloudColorNightEdge,  cloudColorNightMain , clouds) * clouds;

	vec3 cloudMix = mix(clamp(cloudsColoredDay, 0, 1), clamp(cloudsColoredNight, 0, 1), normalizedSun.y);
	cloudMix = clamp(cloudMix, 0, 1);
	return cloudMix;
}

void main() {
	vec3 normTexCoords = normalize(TexCoords);
	float yCoord = ((normTexCoords.y + 0.5) * 1.5);
	vec3 normalizedSun = normalize(sunDir);

	// Stars
	vec4 stars = texture(skybox, TexCoords);
	stars *= clamp(clamp(-yCoord + 0.8, 0, 1) * 0.5 * normalizedSun.y, 0, 1);
	// Gradient Sky
	vec3 skyColor = SkyGradient(normTexCoords, yCoord);
	// Sun + Moon
	vec3 sunAndMoonColor = SunAndMoon(normTexCoords, yCoord);
	// Horizon color
	vec3 horizon = HorizonColor(normTexCoords, yCoord);
	vec3 clouds = Clouds(normTexCoords, yCoord);


	vec3 combined = skyColor + sunAndMoonColor + stars.xyz + horizon + clouds;
	combined = clamp(combined, vec3(0), vec3(1));
	color = vec4(combined, 1);
}
