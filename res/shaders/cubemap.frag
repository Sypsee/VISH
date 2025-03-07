#version 430 core

layout(location = 0) out vec4 g_Albedo;
layout(location = 1) out vec4 g_Position;
layout(location = 2) out vec4 g_Normal;

uniform samplerCube u_Tex;

in vec3 uvw;

void main()
{
    g_Normal = vec4(0.0);
    g_Position = vec4(0.0);

    g_Albedo = texture(u_Tex, uvw);
}