#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4    shadowMatrices[6];
uniform mat4    Persp;


out vec4 FragPos;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = (Persp * shadowMatrices[face]) * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  
