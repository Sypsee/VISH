#version 330 core

layout(location = 0) out vec4 fragColor;

in vec3 fragPos;
in vec2 uv;

uniform sampler2D u_ScreenTex;

vec3 acesApprox(vec3 x)
{
	float a = 2.51;
	float b = 0.03;
	float c = 2.43;
	float d = 0.59;
	float e = 0.14;
	return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main()
{
	vec4 screenColor = texture(u_ScreenTex, uv);
	fragColor = vec4(acesApprox(screenColor.rgb), screenColor.a);
}