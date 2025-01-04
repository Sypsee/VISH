#version 430 core

layout(location = 0) out vec4 g_Albedo;

uniform samplerCube u_Tex;

in vec3 uvw;

void main()
{
    g_Albedo = texture(u_Tex, uvw);
}