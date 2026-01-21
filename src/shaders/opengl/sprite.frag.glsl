#version 330 core

#define MAX_LIGHTS              32

#define HRL_PointLight			(uint(0x0011))
#define HRL_DirectionalLight	(uint(0x0012))
#define HRL_SpotLight			(uint(0x0013))

out vec4 FragColor;

//on récupere les infos passées par le vertex shader
in vec3 fragPos;
in vec2 uv;

//Textures
uniform sampler2D Albedo;
uniform sampler2D NormalMap;
uniform sampler2D Specular;
uniform sampler2D Roughness;
uniform sampler2D Metalic;
uniform sampler2D Alpha;
uniform sampler2D ShadowMap;
uniform sampler2D CubeMap;
uniform sampler2D TexCustom0;
uniform sampler2D TexCustom1;
uniform sampler2D TexCustom2;
uniform sampler2D TexCustom3;
uniform sampler2D TexCustom4;
uniform sampler2D TexCustom5;
uniform sampler2D TexCustom6;

uniform vec3 TintColor;


//lights passées par ubo
struct Light
{
    uint type;
    float intensity;
    float attenuation;
    float padding1;

    vec3 position;
    float padding2;

    vec3 rotation;
    float padding3;

    vec3 color;
    float padding4;
};
//chaque lumiere = 4 * 16 octets
layout(std140) uniform LightBlock
{
    //Nombre maximal de lights passées au shader
    Light lights[MAX_LIGHTS];
};


void main()
{
    //recuperer l'albedo / diffuse / basecolor
    vec3 albedotex = texture(Albedo, uv).rgb;

    //recuperer la normal map et la transformer de [0;1] à [-1;1]
    vec3 normaltex = texture(NormalMap, uv).rgb;
    normaltex = normalize(normaltex*2.0 - 1.0);

    vec3 result = vec3(0.0);

    //parcourir les lights
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        Light light = lights[i];

        //on skip les lumieres éteintes
        if(light.intensity <= 0.0)
        {
            continue;
        }

        if(light.type == HRL_PointLight)
        {
            vec3 lightDir = light.position - fragPos;

            //on calcule la distance entre le fragment et la lumiere
            float distance = length(lightDir);

            lightDir = normalize(lightDir);

            //on calcule l'atenuation de la lumiere
            // 1/ (1 + attenuation * distance au carré)
            float attenuation = 1.0 / (1.0 + light.attenuation * distance * distance);

            //diffuse phong (lumiere qui eclaire la surface)
            float diff = max(dot(normaltex, lightDir), 0.0);
            vec3 diffuse = diff * light.color * light.intensity;

            result += (diffuse * attenuation);
        }
    }

    result *= albedotex;

    FragColor = vec4(result, 1.0);
}