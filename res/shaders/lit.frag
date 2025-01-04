#version 330 core

layout(location = 0) out vec4 g_Albedo;
layout(location = 1) out vec4 g_Position;
layout(location = 2) out vec4 g_Normal;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

uniform bool u_HasTex = false;
uniform sampler2D u_Tex;

void main()
{
    vec4 albedoTexture = texture(u_Tex, uv);
    vec3 albedo = u_HasTex ? albedoTexture.rgb : vec3(1.0, 0.0, 0.0);

    vec3 norm = normalize(normal);
    g_Normal = vec4(normal, 1.0);
    g_Position = vec4(fragPos, 1.0);

    g_Albedo = vec4(albedo, 1.0);
}