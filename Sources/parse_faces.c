/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_faces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:13:09 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 12:09:37 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <strings.h>

static ssize_t  get_material_id(t_mat *materials, size_t material_count, const char *material_name)
{
    size_t i = 0;

    while (i < material_count)
    {
        if (!strncmp(material_name, materials[i].material_name, strlen(materials[i].material_name)))
            return i;
        i++;
    }
    return -1;
}

void        mat_push_buffer(t_mat *material, float *buffer, size_t buffer_size)
{
    if (material->tmp_allocated == 0)
    {
        material->gl_buffer = realloc(NULL, sizeof(float) * buffer_size * MAT_GL_BUFFER_REALLOC_VALUE);
        material->tmp_allocated = MAT_GL_BUFFER_REALLOC_VALUE;
    }
    else if (material->tmp_allocated + 1 > material->tmp_allocated)
    {
        material->tmp_allocated += MAT_GL_BUFFER_REALLOC_VALUE;
        material->gl_buffer = realloc(material->gl_buffer, sizeof(float) * buffer_size * material->tmp_allocated);
    }
    if (!material->gl_buffer) 
        die("Realloc failed for mat_push_buffer");
    _floatncat(material->gl_buffer, buffer, material->gl_buffer_size, buffer_size);
    material->gl_buffer_size += buffer_size;
}

void            calculate_missing_normal(t_mat *material)
{
    size_t i = 0;
    size_t j = 0;
    float   *cbuffer;
    t_vec3f	pt[3];
    t_vec3f normal;

    if (material->gl_buffer_size % (8 * 3) == 0)
    {
        while (i < 8 * 3)
        {
            j = 0;
            cbuffer = &material->gl_buffer[material->gl_buffer_size - (8 * 3) + i];
            pt[j] = (t_vec3f){*cbuffer, *(cbuffer + 1), *(cbuffer + 2)};
            i += 8;
            j++;
        }
        normal = vec_cross(vec_sub(pt[1], pt[0]), vec_sub(pt[2], pt[0]));
        i = 0;
        while (i < 8 * 3)
        {
            cbuffer = &material->gl_buffer[material->gl_buffer_size - (8 * 3) + i];
            *(cbuffer + 3) = normal.x;
            *(cbuffer + 4) = normal.y;
            *(cbuffer + 5) = normal.z;
            i += 8;
        }
    }
}

void            update_minmax(t_vec3f *min, t_vec3f *max, float buffer[8])
{
    if (buffer[0] > max->x)
        max->x = buffer[0];
    if (buffer[0] < min->x)
        min->x = buffer[0];
    if (buffer[1] > max->y)
        max->y = buffer[1];
    if (buffer[1] < min->y)
        min->y = buffer[1];
    if (buffer[2] > max->z)
        max->z = buffer[2];
    if (buffer[2] < min->z)
        min->z = buffer[2];
}

static size_t   parse_vertex(t_parser *parser, t_mat *material, char *line)
{
    uint16_t face_type = 0x0;
	size_t	index_v = 0;
	size_t	index_vt = 0;
	size_t	index_vn = 0;
    float   buffer[8];

    _floatset(buffer, 0.00f, 8);

    if (sscanf(line, "%zd/%zd/%zd", &index_v, &index_vt, &index_vn) == 3)
		face_type = VERTICE | TEXTURE | NORMAL;
	else if (sscanf(line, "%zd//%zd", &index_v, &index_vn) == 2)
		face_type = VERTICE | NORMAL;
	else if (sscanf(line, "%zd/%zd", &index_v, &index_vt) == 2)
		face_type = VERTICE | TEXTURE;
	else if (sscanf(line, "%zd", &index_v) == 1)
		face_type = VERTICE;
	else
    {
        printf("Error face definition\n");
        return (0);
    }

    if (((face_type & VERTICE) && index_v - 1 >= parser->v_count) 
        || ((face_type & TEXTURE) && index_vt - 1 >= parser->vt_count) 
        || ((face_type & NORMAL) && index_vn - 1 >= parser->vn_count))
    {
        printf("Error in parsing order, skipping for now\n");
        return (0);
    }

    if (face_type & VERTICE)
    {
        buffer[0] = parser->v[index_v - 1].x;
        buffer[1] = parser->v[index_v - 1].y;
        buffer[2] = parser->v[index_v - 1].z;
        update_minmax(&parser->min, &parser->max, buffer);
    }
    if (face_type & NORMAL)
    {
       buffer[3] = parser->vn[index_vn - 1].x;
       buffer[4] = parser->vn[index_vn - 1].y;
       buffer[5] = parser->vn[index_vn - 1].z;
    }
    if (face_type & TEXTURE)
    {
       buffer[6] = parser->vt[index_vt - 1].x;
       buffer[7] = parser->vt[index_vt - 1].y;
    }

    mat_push_buffer(material, buffer, 8);
    if ((face_type & NORMAL) != NORMAL)
        calculate_missing_normal(material);
    return (1);
}

static size_t   triangulate(t_parser *parser, t_mat *material, size_t sides_count, char *line)
{
    char    *token = NULL;
    char    **faces = NULL;
    size_t  i = 0;

    if (!(faces = calloc(sides_count, sizeof(char **))))
        die ("Error calloc triangulation");

    token = strtok(line, " ");
	while (token != NULL) {
        if (!(faces[i] = strdup(token)))
            die ("Error strdup triangulation");
        i++;
		token = strtok(NULL, " ");
	}

    i = 0;
    while (i < sides_count - 2)
    {
        parse_vertex(parser, material, _strtrim(faces[0]));
        parse_vertex(parser, material, _strtrim(faces[i + 1]));
        parse_vertex(parser, material, _strtrim(faces[i + 2]));
        i++;
    }

    while (i--)
       free(faces[i]);
    free(faces);
    return (0);
}

size_t          parse_face(t_parser *parser, t_mat **materials, size_t *material_count, char *last_mtl, char *line) 
{
	const ssize_t sides_count   = _strcountchr(line, ' ') + 1;
    ssize_t material_id         = get_material_id(*materials, *material_count, last_mtl);
    char *token = NULL;

    if (!last_mtl || material_id == -1)
    {
        if (material_id == -1 && *material_count == 0)
        {
            *material_count = 1;
            if (!(*materials = calloc(1, sizeof(t_mat))))
                die ("Error creating default material");
            init_mat_default_values(&(*materials)[0]);
            strcpy((*materials)[0].material_name, "DEFAULT_NO_MATERIAL");
            material_id = 0;
        }
        else 
            return (0);
    }

	if (sides_count > 3)
        return (triangulate(parser, &(*materials)[material_id], sides_count, line));

    token = strtok(line, " ");
	while (token != NULL) {
        parse_vertex(parser, &(*materials)[material_id], _strtrim(token));
		token = strtok(NULL, " ");
	}
    // print_matlist(*material_count, *materials);
    return (1);
}