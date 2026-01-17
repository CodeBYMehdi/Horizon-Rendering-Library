#version 330 core

out vec4 FragColor;

in vec2 uv;

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

void main()
{
    FragColor = texture(Albedo, uv);
}