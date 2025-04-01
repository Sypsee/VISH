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

struct Wave 
{
    float waveSpeed;
    float octaves;
    float lacunarity;
    float gain;
    vec2 direction;
};

const int NUMBER_OF_WAVES = 5;
Wave waves[NUMBER_OF_WAVES] = Wave[](
    Wave(1.0, 5, 1.2, 0.15, vec2(1.0, 0.0)),
    Wave(1.5, 3, 1.5, 0.1, vec2(1.0, 1.0)),
    Wave(0.5, 2, 1.1, 0.05, vec2(0.0, 1.0)),
    Wave(1.5, 4, 1.3, 0.1, vec2(0.5, 0.5)),
    Wave(1.0, 5, 1.6, 0.02, vec2(-1.0, 0.0))
);

void main()
{
    vec3 position = aPosition;

    float tempN = 0;
    
    for (int i=0; i < NUMBER_OF_WAVES; i++)
    {
        float frequency = 1.0;
        float amplitude = 0.2;

        for (int j = 0; j < waves[i].octaves; j++)
        {
            position.y += amplitude * sin(dot(vec2(position.x, position.z), waves[i].direction) * frequency + u_Time * waves[i].waveSpeed);
            frequency *= waves[i].lacunarity;
            amplitude *= waves[i].gain;

            tempN += frequency * amplitude * cos(dot(vec2(position.x, position.z), waves[i].direction) * frequency + u_Time * waves[i].waveSpeed);
        }
    }

    normal = normalize(vec3(-tempN, -tempN, 1));
    
    fragPos = vec3(model * vec4(position, 1.0));
    uv = aUV;
    gl_Position = proj * view * model * vec4(fragPos, 1.0);
}