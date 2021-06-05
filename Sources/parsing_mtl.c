/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_mtl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 11:18:58 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 16:08:23 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

static void realloc_mtl(size_t *count, size_t *alloc, void **materials)
{
	if (*alloc > *count)
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

static void init_default_values(t_mat *material)
{
    material->tmp_allocated = 0;
    material->gl_buffer_size = 0;
    material->gl_buffer = NULL;
    material->kd = (t_vec3f) {1.0, 1.0, 1.0};
    material->ks = (t_vec3f) {1.0, 1.0, 1.0};
    material->ka = (t_vec3f) {0.1, 0.1, 0.1};
    material->tf = (t_vec3f) {1.0, 1.0, 1.0};
}

void        parser_mtl_start(t_scop *scop, char path[256], char *file)
{
    char    filepath[256];
    FILE 	*fp;
	char	*line 		= 0;
	ssize_t	read 		= 0;
	size_t	len 		= 0;
    size_t  tmp_nb_mats = 0;

    generate_filepath(filepath, path, file);

	if (!(fp = fopen(filepath, "r")))
        die("Error reading mtl file");
    
    scop->nb_mats   = 0;
    scop->materials = calloc(5, sizeof(t_mat));

	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
        if (!strncmp(line, "Kd ", 3))
            sscanf(line, "Kd %f %f %f", &scop->materials[tmp_nb_mats - 1].kd.x, &scop->materials[tmp_nb_mats - 1].kd.y, &scop->materials[tmp_nb_mats - 1].kd.z);
        else if (!strncmp(line, "Ks ", 3))
            sscanf(line, "Ks %f %f %f", &scop->materials[tmp_nb_mats - 1].ks.x, &scop->materials[tmp_nb_mats - 1].ks.y, &scop->materials[tmp_nb_mats - 1].ks.z);
        else if (!strncmp(line, "Ka ", 3))
            sscanf(line, "Ka %f %f %f", &scop->materials[tmp_nb_mats - 1].ka.x, &scop->materials[tmp_nb_mats - 1].ka.y, &scop->materials[tmp_nb_mats - 1].ka.z);
        else if (!strncmp(line, "Tf ", 3))
            sscanf(line, "Tf %f %f %f", &scop->materials[tmp_nb_mats - 1].tf.x, &scop->materials[tmp_nb_mats - 1].tf.y, &scop->materials[tmp_nb_mats - 1].tf.z);
        else if (!strncmp(line, "map_Kd ", 7))
		{
			printf("Texture file found !\n");
		}
		else if (!strncmp(line, "newmtl ", 7))
        {
            realloc_mtl(&tmp_nb_mats, &scop->nb_mats, (void *) &scop->materials);
            strcpy(scop->materials[tmp_nb_mats].material_name, _strtrim(line + 7));
            init_default_values(&scop->materials[tmp_nb_mats]);
            tmp_nb_mats++;
        }
	}
	free(line);
    if (!(scop->materials = realloc(scop->materials, sizeof(t_mat) * tmp_nb_mats)))
        die ("Material final realloc failed");
    scop->nb_mats   = tmp_nb_mats;
    fclose(fp);
   // free(scop->materials);
}