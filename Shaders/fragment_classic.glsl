#version 400 core
  
out vec4    FragColor;

uniform int     textured;
uniform vec3    kd;

//  
// in vec3      oNormal;
// in vec3      fragmentPosition;
// in vec3      oLightPos;
// in vec3      oLightColor;
//

//flat in int     outTextured;
// flat in vec4        vertexColor;
// in vec2             TexCoord;

in float        current_step;

uniform int     glfw_options;

// uniform sampler2D   ourTexture;
uniform samplerCube depthMap;

uniform mat4	Persp;
uniform mat4	Model;

in      fData
{
    vec3    normal;
    vec2    texture;
    vec3    frag_pos;
}       frag;


void main()
{
    float step      = current_step;

    vec3 ModelNorm  = vec3(((Model)) * vec4(frag.normal, 1));

    vec3 norm       = normalize(ModelNorm);
    vec3 lightDir   = normalize(vec3(-2.5f, 2.5f, 10.0f));

    float d        = max(dot(norm, lightDir), 0.0);

    if (glfw_options == 0)
        step = 0;
    
    if (textured == 0)
        FragColor = vec4(kd * d, 1 - step);
    // if (textured == 1)
        // FragColor = vec4(texture(ourTexture, frag.texture).xyz * d, 1 - step);

    vec3 fragToLight = frag.frag_pos - vec3(0, 5, -15);
    float closestDepth = texture(depthMap, fragToLight).r;

    FragColor = vec4(vec3(closestDepth / 25.0f), 1.0);  


} 