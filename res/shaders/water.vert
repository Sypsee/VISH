#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform float u_Time;

out vec3 normal;
out vec2 uv;
out vec3 fragPos;

const float waveSpeed = 1.5;

void main()
{
    normal = aNormal;
    vec3 position = aPosition;

    float sumOfSines = 0;
    float frequency = 1.0;
    float amplitude = 0.8;
    for (int i = 0; i < 5; i++)
    {
        sumOfSines += sin((position.x + position.z) * frequency + u_Time * waveSpeed) * amplitude;
        frequency += frequency / 4;
        amplitude -= amplitude / 2;
    }

    position.y += sumOfSines;

    fragPos = vec3(model * vec4(position, 1.0));
    uv = aUV;
    gl_Position = proj * view * model * vec4(fragPos, 1.0);
}