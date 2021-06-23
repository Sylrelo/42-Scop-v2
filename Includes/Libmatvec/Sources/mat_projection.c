/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_projection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:16 by slopez            #+#    #+#             */
/*   Updated: 2021/06/23 12:07:27 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libmatvec.h"
#include <math.h>
#include <stdio.h>

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

t_mat4		m4_orthogonal(float nearz, float farz, float left, float right, float bottom, float top)
{
	t_mat4 result 		= m4_init();
	const float rl		= 1.0f / (right - left);
	const float tb		= 1.0f / (top - bottom);
	const float fn		= -1.0f / (farz - nearz);

	result.value[0][0] = 2.0f * rl;
	result.value[1][1] = 2.0f * tb;
	result.value[2][2] = 2.0f * fn;
	result.value[3][0] = -(right + left) * rl;
	result.value[3][1] = -(top + bottom) * tb;
	result.value[3][2] = (farz + nearz) * fn;
	result.value[3][3] = 1.0f;

	return (result);
}

t_mat4		m4_look_at(t_vec3f from, t_vec3f to, t_vec3f upv)
{
	t_mat4	result;
	t_vec3f forward = vec_normalize(vec_sub(to, from));
	t_vec3f side 	= vec_normalize(vec_cross(forward, upv));
	t_vec3f up 		= vec_cross(side, forward);

	result.value[0][0] = side.x;
	result.value[0][1] = up.x;
	result.value[0][2] = -forward.x;
	result.value[1][0] = side.y;
	result.value[1][1] = up.y;
	result.value[1][2] = -forward.y;
	result.value[2][0] = side.z;
	result.value[2][1] = up.z;
	result.value[2][2] = -forward.z;
	result.value[3][0] = -vec_dot(side, from);
	result.value[3][1] = -vec_dot(up, from);
	result.value[3][2] = vec_dot(forward, from);
	result.value[0][3] = 0.0f;
	result.value[1][3] = 0.0f;
	result.value[2][3] = 0.0f;
	result.value[3][3] = 1.0f;

	return (result);
}
