#version 330 core

layout(location = 0) out vec4 gAlbedo;

uniform vec3 u_CamPos;

in vec3 fragPos;

const float gridMinPixelsBetweenCells = 2.0;
const float gridCellSize = 1.0;
const vec4 gridColorThin = vec4(0.15, 0.15, 0.15, 1.0);
const vec4 gridColorThick = vec4(0.0, 0.0, 0.0, 1.0);

float log10(float x)
{
	return log(x) / log(10.0);
}

void main()
{
	vec2 dvx = vec2(dFdx(fragPos.x), dFdy(fragPos.x));
	vec2 dvy = vec2(dFdx(fragPos.z), dFdy(fragPos.z));
	
	vec2 dudv = vec2(length(dvx), length(dvy));
	float l = length(dudv);
	dudv *= 4.0;

	float lod = max(0.0, log10(l * gridMinPixelsBetweenCells / gridCellSize) + 1.0);
	
	float gridCellSizeLod0 = gridCellSize * pow(10.0, floor(lod));
	float gridCellSizeLod1 = gridCellSizeLod0 * 10.0;
	float gridCellSizeLod2 = gridCellSizeLod0 * 10.0;

	vec2 lod0aVec2 = vec2(1.0) - abs(clamp(mod(fragPos.xz, gridCellSizeLod0) / dudv, 0.0, 1.0) * 2.0 - vec2(1.0));
	float lod0a = max(lod0aVec2.x, lod0aVec2.y);

	vec2 lod1aVec2 = vec2(1.0) - abs(clamp(mod(fragPos.xz, gridCellSizeLod1) / dudv, 0.0, 1.0) * 2.0 - vec2(1.0));
	float lod1a = max(lod1aVec2.x, lod1aVec2.y);

	vec2 lod2aVec2 = vec2(1.0) - abs(clamp(mod(fragPos.xz, gridCellSizeLod2) / dudv, 0.0, 1.0) * 2.0 - vec2(1.0));
	float lod2a = max(lod2aVec2.x, lod2aVec2.y);

	float lodFade = fract(lod);

	vec4 finalColor;
	
	if (lod2a > 0.0)
	{
		finalColor = gridColorThick;
		finalColor.a *= lod2a;
	}
	else
	{
		if (lod1a > 0.0)
		{
			finalColor = mix(gridColorThick, gridColorThin, lodFade);
			finalColor.a *= lod1a;
		}
		else
		{
			finalColor = gridColorThin;
			finalColor.a *= (lod0a * (1.0 - lodFade));
		}
	}

	float opacityFalloff = (1.0 - clamp(length(fragPos.xz - u_CamPos.xz) / 100.0, 0.0, 1.0));
	finalColor.a *= opacityFalloff;

	gAlbedo = finalColor;
}