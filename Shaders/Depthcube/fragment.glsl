#version 400 core

in vec4 FragPos;

uniform vec3    light_pos;
uniform float   far_plane;

void main()
{
    float light_dis  = length(FragPos.xyz - light_pos);
    light_dis       = light_dis / far_plane;
    gl_FragDepth    = light_dis;
}  