#version 330

out vec4 fragColor;

in vec3 fragPos;
in vec2 uv;

uniform sampler2D u_ScreenTex;

void main()
{
	fragColor = texture(u_ScreenTex, uv);
}