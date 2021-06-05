#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

uniform mat4	Transform;
uniform mat4	Persp;
uniform mat4	Model;
uniform vec3    kd;


out vec2 TexCoord;
out vec4 vertexColor; 

void main()
{
    gl_Position = (Persp * Transform * Model) * vec4(aPos, 1.0f);
    TexCoord    = aTexCoord;
    vertexColor = vec4(kd, 1.0);
}