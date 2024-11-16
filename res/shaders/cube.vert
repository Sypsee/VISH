#version 460

layout(location = 0) in vec3 aPosition;
layout(location = 0) in vec3 aNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;
out vec3 fragPos;

void main()
{
    normal = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * vec4(aPosition, 1.0));
    gl_Position = proj * view * model * vec4(fragPos, 1.0);
}