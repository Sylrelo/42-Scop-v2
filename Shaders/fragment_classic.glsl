#version 400 core
out vec4 FragColor;
  


flat in int outTextured;
flat in vec4     vertexColor;
in vec2     TexCoord;

uniform sampler2D ourTexture;

void main()
{

    float r, g, b;
    r = (gl_PrimitiveID % 256) / 255.0f;
    g = ((gl_PrimitiveID / 256) % 256) / 255.0f;
    b = ((gl_PrimitiveID / (256 * 256)) % 256) / 255.0f;

    FragColor = vertexColor;

    // if (outTextured == 0)
    //     FragColor = vertexColor;
    // if (outTextured == 1)
    //     FragColor = texture(ourTexture, TexCoord);
} 