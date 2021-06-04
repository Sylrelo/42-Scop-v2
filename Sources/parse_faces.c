
#include "Scop.h"
#include "Prototypes.Parsing.h"
#include <stdio.h>
#include <strings.h>

static ssize_t get_material_id(t_mat *materials, size_t material_count, const char *material_name)
{
    size_t i = 0;

    while (i < material_count)
    {
        if (!strncmp(material_name, materials[i].material_name, strlen(material_name)))
            return i;
        i++;
    }
    return -1;
}


static void push_buffer(t_mat *material, float *buffer, size_t buffer_size)
{
    if (material->tmp_allocated == 0)
    {
        material->gl_buffer = realloc(NULL, sizeof(float) * buffer_size * 2500);
        material->tmp_allocated = 5000;
    }
    else if (material->tmp_allocated + 1 > material->tmp_allocated)
    {
        material->tmp_allocated += 5000;
        material->gl_buffer = realloc(material->gl_buffer, sizeof(float) * buffer_size * material->tmp_allocated);
    }
    if (!material->gl_buffer) 
    {
        printf("Realloc failed for push_buffer\n");
        return ;
    }

    _floatncat(material->gl_buffer, buffer, material->gl_buffer_size, buffer_size);
    material->gl_buffer_size += buffer_size;
}

void parse_face(t_parser *parser, t_mat *materials, size_t material_count, char *last_mtl, char *line) 
{
    const ssize_t material_id = get_material_id(materials, material_count, last_mtl);
	const ssize_t sides_count = strcountchr(line, ' ');
	uint16_t face_type = 0x0;
	size_t	index_v = 0;
	size_t	index_vt = 0;
	size_t	index_vn = 0;
    float   buffer[8];

    _floatset(buffer, 0.00f, 8);

    if (!last_mtl || material_id == -1)
    {
		printf("Face does not have a material, skipping (for now)\n");
        return ;
    }

	if (sides_count > 3)
	{
		printf("Face is not triangle, triangulation is not yet available, skipping\n");
		return ;
	}

	if (sscanf(line, "%zd/%zd/%zd", &index_v, &index_vt, &index_vn) == 3)
		face_type = VERTICE | TEXTURE | NORMAL;
	else if (sscanf(line, "%zd//%zd", &index_v, &index_vn) == 2)
		face_type = VERTICE | NORMAL;
	else if (sscanf(line, "%zd/%zd", &index_v, &index_vt) == 2)
		face_type = VERTICE | TEXTURE;
	else if (sscanf(line, "%zd", &index_v) == 1)
		face_type = VERTICE;
	
    if (((face_type & VERTICE) && index_v >= parser->v_count) 
        || ((face_type & TEXTURE) && index_vt >= parser->vt_count) 
        || ((face_type & NORMAL) && index_vn >= parser->vn_count))
    {
        printf("Error in parsing order, skipping for now\n");
        return ;
    }

    if (face_type & VERTICE)
    {
        buffer[0] = parser->v[index_v].x;
        buffer[1] = parser->v[index_v].y;
        buffer[2] = parser->v[index_v].z;
    }
    if (face_type & NORMAL)
    {
        buffer[3] = parser->v[index_vn].x;
        buffer[4] = parser->v[index_vn].y;
        buffer[5] = parser->v[index_vn].z;
    }
    if (face_type & TEXTURE)
    {
        buffer[6] = parser->v[index_vt].x;
        buffer[7] = parser->v[index_vt].y;
    }

    // printf("%.2f %.2f %.2f - %.2f %.2f %.2f - %.2f %.2f\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
    push_buffer(&materials[material_id], buffer, 8);

    //printf("%s buffer size = %zu\n", last_mtl, materials[material_id].gl_buffer_size);

    // printf("%f %f %f\n", parser->v[index_v].x, parser->v[index_v].y, parser->v[index_v].z);
    // (void)parser;
	//printf("[%s] %zu %zu %s", last_mtl, sides_count, strcountchr(line, '/'), line);
}