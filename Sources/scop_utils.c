/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:14:21 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 16:50:33 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <time.h>

float	random_float(int min, int max, float prec)
{
	return ((min + rand() % max) * prec);
}

void    print_matlist(size_t nb_mat, t_mat *materials)
{
    t_mat *material;

    for (size_t i = 0; i < nb_mat; i++)
    {
        material = &materials[i];
        printf("[ %s ]\n", material->material_name);
        printf("  Kd: %.2f %.2f %.2f\n", material->kd.x, material->kd.y, material->kd.z);
        printf("  Ka: %.2f %.2f %.2f\n", material->ka.x, material->ka.y, material->ka.z);
        printf("  Ks: %.2f %.2f %.2f\n", material->ks.x, material->ks.y, material->ks.z);
        printf("  Tf: %.2f %.2f %.2f\n", material->tf.x, material->tf.y, material->tf.z);
    }
}

void    clean_exit(t_scop *scop)
{
	size_t i = 0;

	if (scop->textures)
	{
		free(scop->textures);
		scop->textures = NULL;
	}
	while (i < scop->objects_count)
	{
		for (size_t mat_i = 0; mat_i < scop->objects[i].nb_mats; mat_i++)
		{
			if (scop->objects[i].materials[mat_i].gl_buffer)
			{
				free(scop->objects[i].materials[mat_i].gl_buffer);
				scop->objects[i].materials[mat_i].gl_buffer = NULL;
			}
		}
		if (scop->objects[i].materials)
		{
			free(scop->objects[i].materials);
			scop->objects[i].materials = NULL;
		}
		i++;
	}
	if (scop->objects)
	{
		free(scop->objects);
		scop->objects = NULL;	
	}
	if (scop)
	{
		free(scop);
		scop = NULL;
	}
}

void	die(char *string)
{
	printf("%s\n", string);
	exit (1);
}
