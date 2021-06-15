#version 400 core
  
out vec4    FragColor;

uniform int     textured;

//  
in vec3      oNormal;
in vec3      fragmentPosition;
// in vec3      oLightPos;
// in vec3      oLightColor;
//

//flat in int     outTextured;
// flat in vec4     vertexColor;
// in vec2             TexCoord;

uniform sampler2D ourTexture;

void main()
{

//    vec3 norm      = normalize(oNormal);
//    vec3 lightDir  = normalize(oLightPos - fragmentPosition);  
//    float d        = max(dot(norm, lightDir), 0.0);

//     vec3 diffuse  = d * oLightColor;


    float r, g, b;
    r = (gl_PrimitiveID % 256) / 255.0f;
    g = ((gl_PrimitiveID / 256) % 256) / 255.0f;
    b = ((gl_PrimitiveID / (256 * 256)) % 256) / 255.0f;

    // FragColor = vertexColor;

    if (textured == 0)
        FragColor = vec4(r, g, b, 1);
        // FragColor = vec4(vertexColor.xyz , 1);
        // FragColor = vec4(vertexColor.xyz * diffuse, 1);
    // if (textured == 1)
        // FragColor = texture(ourTexture, TexCoord);
} 