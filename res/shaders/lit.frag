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

vec3 brdf(vec3 lightDir, vec3 viewDir, float roughness, vec3 normal, vec3 albedo, float metallic, vec3 reflectance)
{
    float alpha = pow(roughness, 2);
    
    vec3 H = normalize(lightDir + viewDir);

    float NdotV = max(dot(normal, viewDir), 0.001);
    float NdotL = max(dot(normal, lightDir), 0.001);
    float NdotH = max(dot(normal, H), 0.001);
    float VdotH = max(dot(viewDir, H), 0.001);

    vec3 F0 = reflectance;
    vec3 fresnelReflectance = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    vec3 rhoD = albedo;
    rhoD *= (vec3(1.0) - fresnelReflectance);
    rhoD *= (1-metallic);

    float k = alpha/2;
    float geometry = (NdotL / (NdotL*(1-k)+k)) * (NdotV / (NdotV*(1-k)+k));

    float lowerTerm = pow(NdotH, 2) * (pow(alpha, 2) - 1.0) + 1.0;
    float normalDistributionFunctionGGX = pow(alpha, 2) / (3.14159 * pow(lowerTerm, 2));

    vec3 phongDiffuse = rhoD;
    vec3 cookTorrance = (fresnelReflectance * normalDistributionFunctionGGX * geometry);
    vec3 BRDF = (phongDiffuse + cookTorrance) * NdotL;
    
    return BRDF;
}

const float ambient = 0.15;
const float roughness = 0.4;
const float metallic = 0.1;
const vec3 reflectance = vec3(0.1);

void main()
{
    vec3 albedo = u_HasTex ? texture(u_Tex, uv).rgb : vec3(0.6, 0.2, 0.4);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    float lightFragDistance = distance(lightPos, fragPos);
    float attenuation = 1/(lightFragDistance * lightFragDistance);
    vec3 brdf = brdf(lightDir, viewDir, roughness, norm, albedo, metallic, reflectance);
    vec3 color = albedo * ambient + brdf * lightColor;
    color *= max(attenuation, ambient);
    color *= 2.0;   // BRIGHTER IMAGE!

    vec3 finalColor = pow(color, vec3(1/2.2));
    FragColor = vec4(finalColor, 1.0);
}