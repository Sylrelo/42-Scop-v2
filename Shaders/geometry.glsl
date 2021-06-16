#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4	View;
uniform mat4	Persp;
uniform mat4	Model;

uniform float   glfw_time;
uniform int     glfw_options;

out float       current_step;

out vec3        geomOutNormal;

in      vData
{
    vec3 normal;
    vec2 texture;
}       vertices[];

out     fData
{
    vec3 normal;
    vec2 texture;
}       frag;

// out vec4        fragPosition[3];

void main(void)
{

    int i;  
    int len = gl_in.length();

    vec3 center;
    vec3 vmin = vec3(intBitsToFloat(2139095039), intBitsToFloat(2139095039), intBitsToFloat(2139095039));
    vec3 vmax = vec3(-intBitsToFloat(2139095039), -intBitsToFloat(2139095039), -intBitsToFloat(2139095039));

    for (i = 0; i < len; i++)
    {
        vmax.x = max(vmax.x, gl_in[i].gl_Position.x);
        vmax.y = max(vmax.y, gl_in[i].gl_Position.y);
        vmax.z = max(vmax.z, gl_in[i].gl_Position.z);
        vmin.x = min(vmin.x, gl_in[i].gl_Position.x);
        vmin.y = min(vmin.y, gl_in[i].gl_Position.y);
        vmin.z = min(vmin.z, gl_in[i].gl_Position.z);
    }
    center = (vmin + vmax) * .5;

    const float timespan = .5f;
    float w = 1 - fract(glfw_time / timespan);

    for (i = 0; i < len; i++)
    {
        if (glfw_options == 1)
        {
            
            gl_Position = (Persp * View * Model) * (gl_in[i].gl_Position + vec4(center * w, 0));
        }
        else
            gl_Position = (Persp * View * Model) * (gl_in[i].gl_Position);

        current_step            = w;
        frag.normal     = vertices[i].normal;
        frag.texture    = vertices[i].texture;

        // fragPosition[i]        = (Persp * Model) * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}