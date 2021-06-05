#version 400 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

uniform mat4	Transform;
uniform mat4	Persp;
uniform mat4	Model;
uniform vec3    kd;
uniform int     textured;

flat out int outTextured;

out vec2 TexCoord;
flat out vec4 vertexColor; 

void main()
{
    
    gl_Position = (Persp * Transform * Model) * vec4(aPos, 1.0f);
    TexCoord    = aTexCoord;
    outTextured = textured;
    // vertexColor = vec4(kd, 1.0);
    vertexColor = vec4(aPos.y * 0.4f + 0.4f, aPos.z * 0.1 + aPos.y * 0.4f + 0.1f, 0.2f, 1.0f);
}