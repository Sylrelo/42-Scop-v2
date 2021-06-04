/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_mtl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 11:18:58 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 12:37:16 by slopez           ###   ########lyon.fr   */
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
    if (*count == 0 && *alloc == 0) {
	    if (!(*materials = calloc(sizeof(t_mat), 5)))
		    die("Erreor realloc");
        *alloc = 5;
        return;
    }

    printf("%zu %zu\n", *alloc, *count);
	if (*alloc > *count)
		return ;

	*alloc += 10;
	if (!(*materials = realloc(*materials, sizeof(t_mat) * *alloc)))
		die("Erreor realloc");
}

void    parser_mtl_start(t_scop *scop, t_parser *parser, char path[256], char *file)
{
    char    filepath[256];
    FILE 	*fp;
	char	*line 		= 0;
	ssize_t	read 		= 0;
	size_t	len 		= 0;
    size_t  tmp_nb_mats = 0;
    
    file[strlen(file) - 1] = 0;
    
    strcat(filepath, path);
    strcat(filepath, file);
    filepath[strlen(path) + strlen(file)] = 0;
	fp = fopen(filepath, "r");
    
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
        else if (!strncmp(line, "newmtl ", 7)) {
            _realloc_mtl(&tmp_nb_mats, &scop->nb_mats, (void *) &scop->materials);
            line[strlen(line) - 1] = 0;
            strcpy(scop->materials[tmp_nb_mats].material_name, line + 7);
            tmp_nb_mats++;
        }
        // printf("%s", line);
	}
    
	free(line);

    for (int i = 0; i < tmp_nb_mats; i++) {
        printf("- %s\n", scop->materials[i].material_name);
        printf("  Kd %.2f %.2f %.2f\n", scop->materials[i].kd.x, scop->materials[i].kd.y, scop->materials[i].kd.z);
        printf("  Ks %.2f %.2f %.2f\n", scop->materials[i].ks.x, scop->materials[i].ks.y, scop->materials[i].ks.z);
        printf("  Tf %.2f %.2f %.2f\n", scop->materials[i].tf.x, scop->materials[i].tf.y, scop->materials[i].tf.z);
    }
    
    printf("nb mats %zu\n", tmp_nb_mats);
    printf("nb mats %zu\n", scop->nb_mats);
    exit (1);
    
}