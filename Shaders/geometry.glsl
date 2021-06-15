#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4	View;
uniform mat4	Persp;
uniform mat4	Model;

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

    for (i = 0; i < len; i++)
    {
        // gl_Position = (Persp * View * Model) * (gl_in[i].gl_Position + vec4(center * 4, 0));
        gl_Position = (Persp * View * Model) * (gl_in[i].gl_Position);
        EmitVertex();
    }
    EndPrimitive();
}