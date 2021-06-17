/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_projection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:16 by slopez            #+#    #+#             */
/*   Updated: 2021/06/17 13:09:46 by slopez           ###   ########.fr       */
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

t_mat4		m4_orthogonal(float near, float far, float left, float right, float bottom, float top)
{
	t_mat4 result 		= m4_init();

	// float r = 0;
	// float t = 0;

	result.value[0][0] = 0.5 * (right - left);
	result.value[0][3] = -(right + left) / (right - left);
	
	result.value[1][1] = 0.5 * (top - bottom);
	result.value[1][3] = -(top + bottom) / (top - bottom);

	result.value[2][2] = (2) / (far - near);
	result.value[2][3] = -(far + near) / (far - near);

	// result.value[0][0] = 1 / r;
	// result.value[1][1] = 1 / t;
	// result.value[2][2] = (-2) / (far - near);
	// result.value[2][3] = -((far + near) / (far - near));

	// float tan_half_fov 	= tanf(fov * .5f);;
	
	// result.value[0][0] = 1.0f / (aspect * tan_half_fov);
	// result.value[1][1] = 1.0f / (tan_half_fov);
	// result.value[2][2] = (far + near) / (far - near) * -1;
	// result.value[2][3] = -1.0f;
	// result.value[3][2] = (2.0f * far * near) / (far - near) * -1;

	return (result);
}

t_mat4		m4_look_at(t_vec3f from, t_vec3f to, t_vec3f upv)
{
	t_vec3f tmp 	= upv;
	t_vec3f forward = vec_normalize(vec_sub(to, from));
	t_vec3f right 	= vec_normalize(vec_cross(tmp, forward));
	t_vec3f up 		= vec_cross(forward, right);

	t_mat4	result = m4_init();

	result.value[0][0] =  right.x;
	result.value[0][1] =  up.x;
	result.value[0][2] = -forward.x;

	result.value[1][0] =  right.y;
	result.value[1][1] =  up.y;
	result.value[1][2] = -forward.y;

	result.value[2][0] =  right.z;
	result.value[2][1] =  up.z;
	result.value[2][2] = -forward.z;

	// return (result);
	return (m4_mult(m4_translate(-from.x, -from.y, -from.z), result));
}
