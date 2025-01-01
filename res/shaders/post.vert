#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec3 fragPos;
out vec2 uv;

void main()
{
    fragPos = aPos;
    uv = aUV;

    gl_Position = vec4(aPos, 1.0);
}