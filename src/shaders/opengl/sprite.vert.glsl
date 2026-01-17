#version 330 core

//vertex/point
layout(location = 0) in vec2 apos;
layout(location = 1) in vec2 auv;

//entité/objet
uniform mat4 model;

//vue/ecran
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main()
{
    vec4 pos = vec4(apos, 0.0, 1.0);

    //on passe les coordonées uv au fragment
    uv = auv;

    //garder cet ordre
    gl_Position = projection * view * model * pos;
}