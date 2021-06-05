#version 330 core

layout (location = 0) in vec3 aPos; 

uniform mat4	Transform;
uniform mat4	Persp;
uniform mat4	Model;
uniform vec3    kd;

out vec4 vertexColor; 

void main()
{
   // gl_Position = vec4(aPos, 1.0);
    gl_Position =   (Persp * Transform * Model) * vec4(aPos, 1.0f);

    vertexColor = vec4(kd, 1.0);
}