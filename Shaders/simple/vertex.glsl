#version 400 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

uniform mat4	    Persp;
uniform mat4	    View;
uniform mat4	    Model;

out vec3        normal;
out vec2             tex_coords;


void main()
{
    normal = aNormals;
    tex_coords = aTexCoord;
    gl_Position     =   (Persp * View * Model) * vec4(aPos, 1.0f);
}

