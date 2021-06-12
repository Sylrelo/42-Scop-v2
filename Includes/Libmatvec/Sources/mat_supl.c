/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_supl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:14 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 01:15:14 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libmatvec.h"
#include <math.h>
#include <stdio.h>

t_mat4		m4_viewmat(float x, float y, float z, t_mat4 translate)
{
	t_mat4	res = m4_mult3(m4_rotation(0, y, 0), m4_scale(1, 1, 1), translate);
	return(m4_mult(res, m4_mult3(m4_scale(1, 1, 1), m4_init(), m4_rotation(x, 0, z))));
}

void		m4_print(t_mat4 mat)
{
	int			i;

	i = -1;
	while (++i < 4)
	{
		printf("%.2f %.2f %.2f %.2f\n", mat.value[i][0], mat.value[i][1], mat.value[i][2], mat.value[i][3]);
	}
	printf("\n");

}
