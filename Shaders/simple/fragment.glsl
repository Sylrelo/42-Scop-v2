#version 400 core
#define PI 3.1415926538

out vec4            FragColor;

in vec3             normal;
in vec2             tex_coords;
in vec3             color;
in vec4             frag_position;

uniform mat4	    Model;
uniform int         textured;
uniform vec3        kd;

uniform sampler2D   ourTexture;

uniform vec2        tex_size;
uniform float       obj_max_y;
uniform vec3        obj_center;

vec2 cylinder_mapping(float x, float y, float z)
{
    float   u;
    float   v;

    // u : atan : calcul de l'angle entre z, et x
    // 2pi = 360 deg
    u = (0.5 + atan(z - obj_center.z, x - obj_center.x)) / (PI * 0.5);
    v = y / obj_max_y;

    u *= tex_size.x;
    v *= tex_size.y;
    return (vec2(u, v));
}


void main()
{
    vec3 ModelNorm  = vec3( Model * vec4(normal, 0));

    vec3 norm       = normalize(ModelNorm);
    vec3 lightDir   = normalize(vec3(0, 3.5f, 10.0f));


    float d         = max(dot(norm, lightDir), 0.0);

    if (textured == 0)
        FragColor = vec4(kd * d, 1);
    else if (textured == 1)
        FragColor = vec4(texture(ourTexture, tex_coords).xyz * d, 1);
    else if (textured == 2)
       FragColor = vec4(texture(ourTexture, cylinder_mapping(frag_position.x, frag_position.y, frag_position.y)).xyz * d, 1);
    else
        FragColor = vec4(color * d, 1);
}