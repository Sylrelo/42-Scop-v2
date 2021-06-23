/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vecs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:00 by slopez            #+#    #+#             */
/*   Updated: 2021/06/23 12:04:30 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libmatvec.h"
#include <math.h>

t_vec3f		vec_add(t_vec3f u, t_vec3f v)
{
	u.x = u.x + v.x;
	u.y = u.y + v.y;
	u.z = u.z + v.z;

    return (u);
}

t_vec3f		vec_sub(t_vec3f u, t_vec3f v)
{
	u.x = u.x - v.x;
	u.y = u.y - v.y;
	u.z = u.z - v.z;

    return (u);
}

t_vec3f		vec_multf(t_vec3f u, float f)
{
	u.x *= f;
	u.y *= f;
	u.z *= f;

    return (u);
}

t_vec3f		vec_cross(t_vec3f u, t_vec3f v)
{
	t_vec3f	result;

    result.x = u.y * v.z - u.z * v.y;
    result.y = u.z * v.x - u.x * v.z;
    result.z = u.x * v.y - u.y * v.x;

    return (result);
}

float		vec_length(t_vec3f u)
{
	return (sqrtf(u.x * u.x + u.y * u.y + u.z * u.z));
}

t_vec3f		vec_normalize(t_vec3f u)
{
	float	l;

	l = 1 / vec_length(u);
	u.x *= l;
	u.y *= l;
	u.z *= l;
	return (u);
}

float		vec_dot(t_vec3f u, t_vec3f v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}