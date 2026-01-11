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

uniform vec3 TintColor;

void main()
{
    FragColor = texture(Albedo, uv);
}