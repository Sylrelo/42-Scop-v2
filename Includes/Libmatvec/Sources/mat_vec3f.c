/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_vec3f.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:09 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 18:38:04 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "Libmatvec.h"
#include <math.h>

t_vec3f	m4_mult_vec3f(t_mat4 mat, t_vec3f vec)
{
	t_vec3f result = (t_vec3f) {0, 0, 0};

	result.x = vec.x * mat.value[0][0] + vec.y * mat.value[0][1] + vec.z * mat.value[0][2];
	result.y = vec.x * mat.value[1][0] + vec.y * mat.value[1][1] + vec.z * mat.value[1][2];
	result.z = vec.x * mat.value[2][0] + vec.y * mat.value[2][1] + vec.z * mat.value[2][2];

	return (result);
}

t_vec4f	m4_mult_vec4f(t_mat4 mat, t_vec4f vec)
{
	t_vec4f result = (t_vec4f) {0, 0, 0, 0};

	result.x = vec.x * mat.value[0][0] + vec.y * mat.value[0][1] + vec.z * mat.value[0][2] + vec.w * mat.value[0][3];
	result.y = vec.x * mat.value[1][0] + vec.y * mat.value[1][1] + vec.z * mat.value[1][2] + vec.w * mat.value[1][3];
	result.z = vec.x * mat.value[2][0] + vec.y * mat.value[2][1] + vec.z * mat.value[2][2] + vec.w * mat.value[2][3];
	result.w = vec.x * mat.value[3][0] + vec.y * mat.value[3][1] + vec.z * mat.value[3][2] + vec.w * mat.value[3][3];

	if (result.w != 1)
	{
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return (result);
}