/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_projection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:16 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 01:15:16 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libmatvec.h"
#include <math.h>

t_mat4		m4_perspective(float fov, float aspect, float near, float far)
{
	t_mat4 result 		= m4_init();
	float tan_half_fov 	= tanf(fov * .5f);;
	
	result.value[0][0] = 1.0f / (aspect * tan_half_fov);
	result.value[1][1] = 1.0f / (tan_half_fov);
	result.value[2][2] = (far + near) / (far - near) * -1;
	result.value[2][3] = -1.0f;
	result.value[3][2] = (2.0f * far * near) / (far - near) * -1;

	return (result);
}

t_mat4		m4_look_at(t_vec3f from, t_vec3f to)
{
	t_vec3f tmp = (t_vec3f){0, 1, 0};
	t_vec3f forward = vec_normalize(vec_sub(from, to));
	t_vec3f right = vec_cross(vec_normalize(tmp), forward);
	t_vec3f up = vec_cross(forward, right);
	t_mat4	result;
	
	result.value[0][0] = right.x;
    result.value[0][1] = right.y;
    result.value[0][2] = right.z;
    result.value[1][0] = up.x;
    result.value[1][1] = up.y;
    result.value[1][2] = up.z;
    result.value[2][0] = forward.x;
    result.value[2][1] = forward.y;
    result.value[2][2] = forward.z;
    result.value[3][0] = from.x;
    result.value[3][1] = from.y;
    result.value[3][2] = from.z;
	
	return (result);
}
