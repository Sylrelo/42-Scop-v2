#version 400 core
#define PI 3.1415926538

out vec4            FragColor;

in vec3             normal;
in vec2             tex_coords;
in vec3             color;
in vec4             frag_position;
in vec4             frag_world_pos;

uniform sampler2D   basic_texture;
uniform sampler2D   ourTexture;
uniform mat4	    Model;
uniform vec3        kd;
uniform vec3        ka;
uniform vec3        ks;
uniform vec3        obj_center;
uniform vec3        view_pos;
uniform vec2        tex_size;
uniform int         mapping;
uniform int         textured;
uniform int         glfw_options;
uniform int         object_selected;
uniform float       obj_max_y;
uniform float       ns;
uniform float       glfw_time;

in vec4             frag_light_pos;
uniform sampler2D   shadow_map;

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


float   in_shadow()
{
    float depth;
    float shadow        = 0.0;
    float bias          = 0.0001;
    vec3 coords         = (frag_light_pos.xyz / frag_light_pos.w) * 0.5 + 0.5;
    vec2 texelSize      = 1.0 / textureSize(shadow_map, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            depth = texture(shadow_map, coords.xy + vec2(x, y) * texelSize).r; 
            shadow += coords.z - bias > depth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

vec4    get_colors(int texture_mode, float diffuse, vec3 specular)
{
    vec4 final_color        = vec4(0, 0, 0, 1);

    float shadow = in_shadow();

    if (texture_mode == 0)
    {
        final_color = vec4(((ka * 0.2) + diffuse + specular) * kd, 1);
    }
    else if (texture_mode == 1)
    {
        final_color = vec4((texture(ourTexture, tex_coords).xyz + specular ) * diffuse, 1);
    }
    else if (texture_mode == 2)
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
        final_color *= diffuse;
    }
    else if (texture_mode == 4)
    {
        final_color = vec4((diffuse + specular) * color, 1);
    }
    else
    {
        vec3 ncolor = (diffuse + specular) * color;
        float avg   = (ncolor.x + ncolor.y + ncolor.z) * 0.33;
        final_color = vec4(avg, avg, avg, 1);
    }

    return (final_color  * (1 - shadow));
}


void main()
{
    const float timespan    = 1.0f;
    vec4 final_color        = vec4(0, 0, 0, 1);
    float mix_value         = 0.f;

    vec3 model_normal       = normalize(vec3(Model * vec4(normal, 0)));
    vec3 light_direction    = normalize(vec3(1, 1.f, 2.0f));


    vec3 view_dir           = normalize(view_pos - frag_world_pos.xyz);
    vec3 reflect_dir        = reflect(light_direction, model_normal); 

    float spec              = pow(max(dot(view_dir, reflect_dir), 0.0), ns);
    vec3 specular           = 0.8f * spec * ks;  

    float d                 = max(dot(model_normal, light_direction), 0.0);

    if (glfw_options == 1)
        mix_value = fract(glfw_time / timespan);
    else 
        mix_value = 1.0f;

    if (textured > 0 && glfw_options == 1)
        final_color = mix(get_colors(textured - 1, d, specular), get_colors(textured, d, specular), mix_value);
    else if (textured == 0 && glfw_options == 1)
        final_color = mix(get_colors(4, d, specular), get_colors(0, d, specular), mix_value);
    else
        final_color = get_colors(textured, d, specular);

    if (object_selected == 1)
        FragColor = vec4(final_color.xyz, 1);
    else if (object_selected == 0)
        FragColor = vec4(final_color.xyz * .5, 1);
    else
        FragColor = vec4(final_color.xyz, 1);

}