#version 400 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 vColor;

uniform mat4	    Persp;
uniform mat4	    View;
uniform mat4	    Model;
uniform mat4        Light;

out vec3            normal;
out vec2            tex_coords;
out vec3            color;
out vec4            frag_position;
out vec4            frag_world_pos;
out vec4            frag_light_pos;

void main()
{
    color           = vColor;
    normal          = aNormals;
    tex_coords      = aTexCoord;
    
    frag_position   = vec4(aPos, 1.0f);
    frag_world_pos  = Model * vec4(aPos, 1.0f);
    frag_light_pos  = Light * Model * vec4(aPos, 1.0f);
    
    gl_Position     = (Persp * View * Model) * vec4(aPos, 1.0f);
}

