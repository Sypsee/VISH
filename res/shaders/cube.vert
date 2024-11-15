#version 460

layout(location = 0) in vec3 aPosition;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(aPosition, 1.0);
}