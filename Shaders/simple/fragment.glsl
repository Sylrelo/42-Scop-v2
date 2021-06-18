#version 400 core
  
out vec4            FragColor;

in vec3             normal;
in vec2             tex_coords;
in vec3             color;

uniform mat4	    Model;
uniform int         textured;
uniform vec3        kd;


uniform sampler2D   ourTexture;



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
    else
        FragColor = vec4(color * d, 1);
}