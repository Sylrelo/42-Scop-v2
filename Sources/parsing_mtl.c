/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_mtl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 11:18:58 by slopez            #+#    #+#             */
/*   Updated: 2021/06/21 16:14:42 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

static void realloc_mtl(int *count, size_t *alloc, void **materials)
{
	if (*alloc > (size_t) *count + 1)
		return ;

	*alloc += 10;
	if (!(*materials = realloc(*materials, sizeof(t_mat) * *alloc)))
		die("Erreor realloc");
}

static void generate_filepath(char filepath[256], char path[256], char *file)
{
    file = _strtrim(file);
	memset(filepath, 0, 256);
    strcat(filepath, path);
    strcat(filepath, file);
    filepath[strlen(path) + strlen(file)] = 0;
}

void        init_mat_default_values(t_mat *material)
{
    material->tex_id            = -1;
    material->tmp_allocated     = 0;
    material->gl_buffer_size    = 0;
    material->gl_buffer         = NULL;
    material->kd                = (t_vec3f) {0.7, 0.7, 0.7};
    material->ka                = (t_vec3f) {0.1, 0.1, 0.1};
    material->ks                = (t_vec3f) {.5, .5, .5};
    material->tf                = (t_vec3f) {1.0, 1.0, 1.0};
    material->ns                = 32;
}

int        parser_mtl_start(t_scop *scop, char path[256], char *file)
{
    t_objects   *obj        = &scop->objects[scop->objects_count];
	char	    *line 		= 0;
	ssize_t	    read 		= 0;
	size_t	    len 		= 0;
    int         tmp_nb_mats = -1;
    char        filepath[256];
    t_mat       *material;
    FILE 	    *fp;
    struct stat st;

    generate_filepath(filepath, path, file);

	if (stat(filepath, &st) || !(fp = fopen(filepath, "r")) || (st.st_mode & S_IFDIR) == S_IFDIR)
    {
		printf("\033[0;31m      Error while parsing MTL file \033[1m%s\033[0m\033[0;31m, using default material.\033[0m\n", filepath);
        return (0);
    }

    printf ("    MTL file \033[1m%s\033[0m loaded.\n", filepath);
    obj->nb_mats   = 5;
    obj->materials = calloc(obj->nb_mats, sizeof(t_mat));

    if (!obj->materials) {
        die ("obj->materials calloc failed");
    }

	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
        if (!strncmp(line, "newmtl ", 7))
        {
            tmp_nb_mats++;
            material = &obj->materials[tmp_nb_mats];
            realloc_mtl(&tmp_nb_mats, &obj->nb_mats, (void *) &obj->materials);
            strcpy(obj->materials[tmp_nb_mats].material_name, _strtrim(line + 7));
            init_mat_default_values(&obj->materials[tmp_nb_mats]);
        }
        else if (!strncmp(line, "Kd ", 3))
            sscanf(line, "Kd %f %f %f", &material->kd.x, &material->kd.y, &material->kd.z);
        else if (!strncmp(line, "Ks ", 3))
            sscanf(line, "Ks %f %f %f", &material->ks.x, &material->ks.y, &material->ks.z);
        else if (!strncmp(line, "Ns ", 1))
            sscanf(line, "Ns %f", &material->ns);
        else if (!strncmp(line, "Ka ", 3))
            sscanf(line, "Ka %f %f %f", &material->ka.x, &material->ka.y, &material->ka.z);
        else if (!strncmp(line, "Tf ", 3))
            sscanf(line, "Tf %f %f %f", &material->tf.x, &material->tf.y, &material->tf.z);
        else if (!strncmp(line, "map_Kd ", 7))
            parse_texture(scop, material, path, _strtrim(line + 7));
		
        if (material->ns <= 0)
            material->ns = 1;
        if (material->ns > 1000)
            material->ns = 1000;
	}

	free(line);
    if (!(obj->materials = realloc(obj->materials, sizeof(t_mat) * (tmp_nb_mats + 1))))
        die ("Material final realloc failed");
    obj->nb_mats = tmp_nb_mats + 1;
    fclose(fp);
    return (1);
}