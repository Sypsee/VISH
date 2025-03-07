#version 330 core

layout(location = 0) out vec4 fragColor;

in vec3 fragPos;
in vec2 uv;

#define MAX_LIGHTS 10

struct Light
{
    vec3 pos;
    vec3 color;
};

uniform sampler2D g_Albedo;
uniform sampler2D g_Position;
uniform sampler2D g_Normal;

uniform Light[MAX_LIGHTS] u_Lights;
uniform int u_LightsSize;
uniform vec3 u_ViewPos;

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
	vec4 albedo = texture(g_Albedo, uv);
    vec3 objPos = texture(g_Position, uv).xyz;
    vec3 normal = texture(g_Normal, uv).xyz;

	vec3 viewDir = normalize(u_ViewPos - objPos);
    
    vec3 color = albedo.xyz;
    for (int i = 0; i < u_LightsSize; i++)
    {
        vec3 lightDir = normalize(u_Lights[i].pos - fragPos);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo.xyz * u_Lights[i].color;
        color += diffuse;
    }

	fragColor = vec4(acesApprox(color), albedo.a);
}