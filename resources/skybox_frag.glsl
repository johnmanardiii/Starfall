#version 330 core
out vec4 color;

in vec3 TexCoords;
in vec2 uv;

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

// Time 
uniform float time;

vec3 SkyGradient(vec3 texCoords, float yCoord)
{
	// gradient
	vec3 normalizedSun = normalize(sunDir);
	yCoord = clamp(yCoord, 0, 1);
	vec3 gradientDay = mix(dayTopColor, dayBottomColor, yCoord);
	vec3 gradientNight = mix(nightTopColor, nightBottomColor, yCoord);

	vec3 skyGradients = mix(clamp(gradientDay, 0, 1), clamp(gradientNight, 0, 1), normalizedSun.y);
	skyGradients = clamp(skyGradients, 0, 1);
	return skyGradients;
}

vec3 SunAndMoon(vec3 texCoords, float yCoord)
{
	vec3 normalizedSun = normalize(sunDir);
	float sun = distance(texCoords, normalizedSun);
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
	return vec3((1 - horizonGlow) * horizonColor * 0.3);
}

void main() {
	vec3 normTexCoords = normalize(TexCoords);
	float yCoord = ((normTexCoords.y + 0.5) * 1.5);
	vec3 normalizedSun = normalize(sunDir);

	// Stars
	vec4 stars = texture(skybox, TexCoords);
	stars *= clamp(clamp(-yCoord + 0.7, 0, 1) * 0.5 * normalizedSun.y, 0, 1);
	// Gradient Sky
	vec3 skyColor = SkyGradient(normTexCoords, yCoord);
	// Sun + Moon
	vec3 sunAndMoonColor = SunAndMoon(normTexCoords, yCoord);
	// Horizon color
	vec3 horizon = HorizonColor(normTexCoords, yCoord);

	color = vec4(skyColor + sunAndMoonColor + stars.xyz + horizon, 1);
}
