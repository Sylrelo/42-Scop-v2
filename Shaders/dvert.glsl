#version 400 core

layout (location = 0) in vec3 aPos;

uniform mat4    Light;
uniform mat4    Model;

void main()
{
    gl_Position = Light * Model * vec4(aPos, 1.0);
}