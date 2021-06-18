#version 400 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

out     vData
{
    vec3    normal;
    vec2    texture;
    vec4    frag_pos;
}       vertex;


void main()
{
    vertex.normal   = aNormals;
    vertex.texture  = aTexCoord;
    vertex.frag_pos =   vec4(aPos, 1.0f);
    gl_Position     =   vec4(aPos, 1.0f);
}



// vec2	cylinder_mapping()
// {
// 	float	u;
// 	float	v;


//     // u : atan : calcul de l'angle entre z, et x
//     // 2pi = 360 deg
// 	u = 0.5 + atan(z - center.z, x - center.x) / PI * 0.5;

//     // v : y / taille totale objet y
// 	v = y / 10.0 ;


//     //u *= taille texture x;
//     //u *= taille texture y;
// 	return (vec2(u, v));
// }

