#version 400 core
#define PI 3.1415926538

out vec4            FragColor;

in vec3             normal;
in vec2             tex_coords;
in vec3             color;
in vec4             frag_position;
in vec4             frag_world_pos;

uniform mat4	    Model;

uniform vec3        kd;
uniform vec3        ka;
uniform vec3        ks;
uniform float       ns;

uniform int         mapping;
uniform int         textured;
uniform sampler2D   basic_texture;
uniform sampler2D   ourTexture;

uniform vec2        tex_size;
uniform float       obj_max_y;
uniform vec3        obj_center;

uniform int         object_selected;

uniform vec3        view_pos;

vec2    sphere_mapping(float x, float y, float z)
{
    float   u;
    float   v;

    u = .5 + (atan(x - obj_center.x, z - obj_center.z) / (2.0f * PI));
    v = .5 - (asin(y - obj_center.y) / PI);

    return (vec2(u, v));
}

vec2    cylinder_mapping(float x, float y, float z)
{
    float   u;
    float   v;

    u = atan(z - obj_center.z, x - obj_center.x) / (PI * 2.f) + .5;
    v = ((y - obj_center.y) / obj_max_y) + .5;

    v *= -1;
    return (vec2(u, v));
}

vec2    position_mapping(float x, float y, float z)
{
    return (vec2(x , y));
}

void main()
{
    vec4 final_color;

    vec3 model_normal       = normalize(vec3(Model * vec4(normal, 0)));
    vec3 light_direction    = normalize(vec3(0, 5.5f, 10.0f));


    vec3 view_dir           = normalize(view_pos - frag_world_pos.xyz);
    vec3 reflect_dir        = reflect(light_direction, model_normal); 

    float spec              = pow(max(dot(view_dir, reflect_dir), 0.0), ns);
    vec3 specular           = .7 * spec * ks;  

    float d                 = max(dot(model_normal, light_direction), 0.0);

    if (textured == 0)
    {
        // final_color = vec4( (ka + vec3(d, d, d) + specular) * kd, 1);
        final_color = vec4(((ka * 0.2) + d + specular) * kd, 1);
    }
    else if (textured == 1)
    {
        final_color = vec4((texture(ourTexture, tex_coords).xyz + specular )* d, 1);
    }
    else if (textured == 2)
    {
        switch (mapping)
        {
            case 0:
                final_color = vec4(texture(basic_texture, tex_coords).xyz, 1);
                break ;
            case 1:
                final_color = vec4(texture(basic_texture, cylinder_mapping(frag_position.x, frag_position.y, frag_position.y)).xyz, 1);
                break ;
            case 2:
                final_color = vec4(texture(basic_texture, sphere_mapping(frag_position.x, frag_position.y, frag_position.y)).xyz, 1);
                break ;
            case 3:
                final_color = vec4(texture(basic_texture, position_mapping(frag_position.x, frag_position.y, frag_position.y)).xyz, 1);
                break ;
            default :
                final_color = vec4(color, 1);
        }
        final_color *= d;
    }
    else if (textured == 4)
    {
        final_color = vec4((d + specular) * color, 1);
    }
    else
    {
        vec3 ncolor = (d + specular) * color;
        float avg = (ncolor.x + ncolor.y + ncolor.z) * 0.33;
        final_color = vec4(avg, avg, avg, 1);
    }

    if (object_selected == 1)
        FragColor = vec4(final_color.xyz, 1);
    else if (object_selected == 0)
        FragColor = vec4(final_color.xyz * .5, 1);
    else
        FragColor = vec4(final_color.xyz, 1);

}