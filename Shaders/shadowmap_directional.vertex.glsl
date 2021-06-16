#version 400 core

layout (location = 0) in vec3 aPos;

uniform mat4 Ortho;
uniform mat4 View;
uniform mat4 Model;

void main()
{
    gl_Position = (Ortho * View * Model) * vec4(aPos, 1.0);
} 