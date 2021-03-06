/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_faces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:13:09 by slopez            #+#    #+#             */
/*   Updated: 2021/06/24 11:50:01 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <strings.h>
#include <math.h>

static ssize_t  get_material_id(t_mat *materials, size_t material_count, const char *material_name)
{
    size_t i = 0;
    size_t mat_len = strlen(material_name);

    while (i < material_count)
    {
        if (mat_len == strlen(materials[i].material_name) && !strncmp(material_name, materials[i].material_name, mat_len))
            return i;
        i++;
    }
    return -1;
}

static void     mat_push_buffer(t_mat *material, float *buffer)
{
    if (material->tmp_allocated == 0)
    {
        material->gl_buffer = calloc(BUFFER_COMPONENT * MAT_GL_BUFFER_REALLOC_VALUE, sizeof(float));
        material->tmp_allocated = BUFFER_COMPONENT * MAT_GL_BUFFER_REALLOC_VALUE;
    }
    else if (material->gl_buffer_size + BUFFER_COMPONENT > material->tmp_allocated)
    {
        material->tmp_allocated += BUFFER_COMPONENT * MAT_GL_BUFFER_REALLOC_VALUE;
        material->gl_buffer = realloc(material->gl_buffer, sizeof(float) * material->tmp_allocated);
    }
    if (!material->gl_buffer)
        die("\nRealloc failed for mat_push_buffer");
    _floatncat(material->gl_buffer, buffer, material->gl_buffer_size, BUFFER_COMPONENT);
    material->gl_buffer_size += BUFFER_COMPONENT;
}

static void     calculate_missing_normal(t_mat *material)
{
    size_t i = 0;
    size_t j = 0;
    float   *cbuffer;
    t_vec3f	pt[3];
    t_vec3f normal;

    if (material->gl_buffer_size % (BUFFER_COMPONENT * 3) == 0)
    {
        while (i < BUFFER_COMPONENT * 3)
        {
            cbuffer = &material->gl_buffer[material->gl_buffer_size - (BUFFER_COMPONENT * 3) + i];
            pt[j] = (t_vec3f){*cbuffer, *(cbuffer + 1), *(cbuffer + 2)};
            i += BUFFER_COMPONENT;
            j++;
        }
        normal = vec_cross(vec_sub(pt[1], pt[0]), vec_sub(pt[2], pt[0]));
        i = 0;
        while (i < BUFFER_COMPONENT * 3)
        {
            cbuffer = &material->gl_buffer[material->gl_buffer_size - (BUFFER_COMPONENT * 3) + i];
            *(cbuffer + 3) = normal.x;
            *(cbuffer + 4) = normal.y;
            *(cbuffer + 5) = normal.z;
            i += BUFFER_COMPONENT;
        }
    }
}

static void     calculate_missing_uvcoords(t_mat *material)
{
    float   *cbuffer;

    if (material->gl_buffer_size % (BUFFER_COMPONENT * 3) != 0)
        return ;
 
    cbuffer = &material->gl_buffer[material->gl_buffer_size - (BUFFER_COMPONENT * 3)];
    *(cbuffer + 6 + (0 * BUFFER_COMPONENT)) = 1;
    *(cbuffer + 7 + (0 * BUFFER_COMPONENT)) = 1;
    *(cbuffer + 6 + (1 * BUFFER_COMPONENT)) = 1;
    *(cbuffer + 7 + (1 * BUFFER_COMPONENT)) = 0;
    *(cbuffer + 6 + (2 * BUFFER_COMPONENT)) = 0;
    *(cbuffer + 7 + (2 * BUFFER_COMPONENT)) = 0;
}

static void     generate_random_color(t_mat *material)
{
    size_t  i = 0;
    float   *cbuffer;
    t_vec3f rgb;

    if (material->gl_buffer_size % (BUFFER_COMPONENT * 3) == 0)
    {
        rgb.x = random_float(20, 90, 0.01);
        rgb.y = random_float(25, 90, 0.01);
        rgb.z = random_float(55, 100, 0.01);
        while (i < BUFFER_COMPONENT * 3)
        {
            cbuffer = &material->gl_buffer[material->gl_buffer_size - (BUFFER_COMPONENT * 3) + i];
            *(cbuffer + 8) = rgb.x;
            *(cbuffer + 9) = rgb.y;
            *(cbuffer + 10) = rgb.z;
            i += BUFFER_COMPONENT;
        }
    }
}

void            update_minmax(t_vec3f *min, t_vec3f *max, float buffer[BUFFER_COMPONENT])
{
    max->x = fmax(max->x, buffer[0]);
    max->y = fmax(max->y, buffer[1]);
    max->z = fmax(max->z, buffer[2]);
    min->x = fmin(min->x, buffer[0]);
    min->y = fmin(min->y, buffer[1]);
    min->z = fmin(min->z, buffer[2]);
}

static size_t   parse_vertex(t_parser *parser, t_mat *material, char *line)
{
    uint16_t face_type  = 0x0;
	size_t	index_v     = 0;
	size_t	index_vt    = 0;
	size_t	index_vn    = 0;
    float   buffer[BUFFER_COMPONENT];

    _floatset(buffer, 0.00f, BUFFER_COMPONENT);

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

    if (((face_type & VERTICE) && (index_v - 1 >= parser->v_count || index_v - 1 < 0)) 
        || ((face_type & TEXTURE) && (index_vt - 1 >= parser->vt_count || index_vt - 1 < 0)) 
        || ((face_type & NORMAL) && (index_vn - 1 >= parser->vn_count || index_vn - 1 < 0)))
    {
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

    mat_push_buffer(material, buffer);
    generate_random_color(material);
    if ((face_type & TEXTURE) != TEXTURE)
        calculate_missing_uvcoords(material);
    if ((face_type & NORMAL) != NORMAL)
        calculate_missing_normal(material);
    return (1);
}

static size_t   triangulate(t_parser *parser, t_mat *material, size_t sides_count, char *line)
{
    char    *token  = NULL;
    char    **faces = NULL;
    size_t  i       = 0;

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

    i = 0;
    while (i < sides_count)
    {
        free(faces[i]);
        faces[i] = NULL;
        i++;
    }
    free(faces);
    faces = NULL;
    return (0);
}

size_t          parse_face(t_parser *parser, t_mat **materials, size_t *material_count, char *last_mtl, char *line) 
{
	const ssize_t sides_count   = _strcountchr(line, ' ') + 1;
    ssize_t material_id         = get_material_id(*materials, *material_count, last_mtl);
    char *token                 = NULL;


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
    return (1);
}