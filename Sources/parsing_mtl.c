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

void    _realloc_mtl(size_t *count, size_t *alloc, void **materials)
{
	if (*alloc > *count)
		return ;
    printf("mt alloc %zu %zu\n", *alloc, *count);

	*alloc += 10;
	if (!(*materials = realloc(*materials, sizeof(t_mat) * *alloc)))
		die("Erreor realloc");
}

void    parser_mtl_start(t_scop *scop, char path[256], char *file)
{
    char    filepath[256];
    FILE 	*fp;
	char	*line 		= 0;
	ssize_t	read 		= 0;
	size_t	len 		= 0;
    size_t  tmp_nb_mats = 0;
    
    file[strlen(file) - 1] = 0;
    
	memset(filepath, 0, 256);
    strcat(filepath, path);
    strcat(filepath, file);
    filepath[strlen(path) + strlen(file)] = 0;
    printf("%s %s %s\n", path, file, filepath);


	if (!(fp = fopen(filepath, "r")))
        die("Error reading mtl file");
    
    scop->nb_mats = 0;
    scop->materials = calloc(5, sizeof(t_mat));

	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{

        if (!strncmp(line, "Kd ", 3))
        {
            sscanf(line, "Kd %f %f %f", &scop->materials[tmp_nb_mats - 1].kd.x, &scop->materials[tmp_nb_mats - 1].kd.y, &scop->materials[tmp_nb_mats - 1].kd.z);
        }
        else if (!strncmp(line, "Ks ", 3))
        {
            sscanf(line, "Ks %f %f %f", &scop->materials[tmp_nb_mats - 1].ks.x, &scop->materials[tmp_nb_mats - 1].ks.y, &scop->materials[tmp_nb_mats - 1].ks.z);
        }
        else if (!strncmp(line, "Tf ", 3))
        {
            sscanf(line, "Tf %f %f %f", &scop->materials[tmp_nb_mats - 1].tf.x, &scop->materials[tmp_nb_mats - 1].tf.y, &scop->materials[tmp_nb_mats - 1].tf.z);
        }
        else if (!strncmp(line, "newmtl ", 7))
        {
            _realloc_mtl(&tmp_nb_mats, &scop->nb_mats, (void *) &scop->materials);
            line[strlen(line) - 1] = 0;
            strcpy(scop->materials[tmp_nb_mats].material_name, line + 7);
            scop->materials->tmp_allocated = 0;
            scop->materials->gl_buffer_size = 0;
            scop->materials->gl_buffer = NULL;
            tmp_nb_mats++;
        }
        // printf("%s", line);
	}
    
	free(line);
    scop->materials = realloc(scop->materials, sizeof(t_mat) * tmp_nb_mats);
    scop->nb_mats = tmp_nb_mats;
    
    for (size_t i = 0; i < tmp_nb_mats; i++) {
        printf("- \033[1;36m%s\033[0m\n", scop->materials[i].material_name);
        printf("  - Kd %.2f %.2f %.2f\n", scop->materials[i].kd.x, scop->materials[i].kd.y, scop->materials[i].kd.z);
        printf("  - Ks %.2f %.2f %.2f\n", scop->materials[i].ks.x, scop->materials[i].ks.y, scop->materials[i].ks.z);
        printf("  - Tf %.2f %.2f %.2f\n", scop->materials[i].tf.x, scop->materials[i].tf.y, scop->materials[i].tf.z);
    }
    fclose(fp);
    
   // free(scop->materials);
    printf("nb mats %zu\n", tmp_nb_mats);
    printf("nb mats %zu\n", scop->nb_mats);
}