#version 400 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

// Material uniforms
uniform vec3    kd;
uniform vec3    ka;

uniform vec3    lightPos;  
uniform vec3    lightColor;  

// Matrices uniforms
uniform mat4	View;
uniform mat4	Persp;
uniform mat4	Model;
uniform int     textured;


out vec3        fragmentPosition;
flat out int    outTextured;
flat out vec4   vertexColor; 
out vec2        TexCoord;

out vec3        oNormal;
out vec3        oLightPos;
out vec3        oLightColor;

void main()
{
    oNormal     = aNormals; //normalize(vec3((Model) * vec4(aNormals, 1)));
    oLightColor = lightColor;
    oLightPos   = lightPos;

    fragmentPosition = vec3(Model * vec4(aPos, 1.0));

    gl_Position     = (Persp * View * Model) * vec4(aPos, 1.0f);
    TexCoord        = aTexCoord;
    outTextured     = textured;
    vertexColor = vec4(kd, 1.0);
    vertexColor     = vec4(aPos.y * 0.4f + 0.4f, aPos.z * 0.1 + aPos.y * 0.4f + 0.1f, 0.2f, 1.0f);
}