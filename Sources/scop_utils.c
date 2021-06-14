/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:14:21 by slopez            #+#    #+#             */
/*   Updated: 2021/06/14 22:13:28 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>

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
    if(scop->nb_mats)
	{
		free(scop->materials);
		scop->materials = NULL;
	}
	if (scop->textures_count)
	{
		free(scop->textures);
		scop->textures = NULL;
	}
	free(scop);
	scop = NULL;
}

void	die(char *string)
{
	printf("%s\n", string);
	exit (1);
}
