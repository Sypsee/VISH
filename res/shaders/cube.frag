#version 460

out vec4 FragColor;

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool u_HasTex;
uniform sampler2D u_Tex;

const float ambient = 0.1;
const float specularStrength = 1.0;
const float alpha = 8;

void main()
{
    vec3 base_color = u_HasTex ? texture(u_Tex, uv).rgb : vec3(0.6, 0.2, 0.4);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * lightColor;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), alpha);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 color = (ambient + diffuseColor + specular) * base_color;

    vec3 finalColor = pow(color, vec3(1/2.2));
    FragColor = vec4(finalColor, 1.0);
}