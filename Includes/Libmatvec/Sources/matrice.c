/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrice.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:05 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 01:15:19 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libmatvec.h"
#include <math.h>

t_mat4		m4_init(void)
{
	t_mat4 out;

	out.value[0][0] = 1.f;
	out.value[0][1] = 0.f;
	out.value[0][2] = 0.f;
	out.value[0][3] = 0.f;
	out.value[1][0] = 0.f;
	out.value[1][1] = 1.f;
	out.value[1][2] = 0.f;
	out.value[1][3] = 0.f;
	out.value[2][0] = 0.f;
	out.value[2][1] = 0.f;
	out.value[2][2] = 1.f;
	out.value[2][3] = 0.f;
	out.value[3][0] = 0.f;
	out.value[3][1] = 0.f;
	out.value[3][2] = 0.f;
	out.value[3][3] = 1.f;

	return (out);
}

t_mat4 	    m4_mult(t_mat4 a, t_mat4 b)
{
	t_mat4 	res;
	int		i;
	int		j;
	int		k; 
	
    for (i = 0; i < 4; i++) 
    { 
        for (j = 0; j < 4; j++) 
        { 
            res.value[i][j] = 0; 
            for (k = 0; k < 4; k++) 
                res.value[i][j] += a.value[i][k] * b.value[k][j]; 
        } 
    }

	return (res);
}


t_mat4		m4_mult3(t_mat4 rotation, t_mat4 scale, t_mat4 translate)
{
	return (m4_mult(m4_mult(scale, translate), rotation));
}
