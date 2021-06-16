/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_projection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:16 by slopez            #+#    #+#             */
/*   Updated: 2021/06/17 00:19:37 by slopez           ###   ########.fr       */
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

t_mat4		m4_look_at(t_vec3f from, t_vec3f to)
{
	t_vec3f tmp 	= (t_vec3f){0, 1, 0};;

	t_vec3f forward = vec_normalize(vec_sub(to, from));
	t_vec3f right 	= vec_normalize(vec_cross(tmp, forward));
	t_vec3f up 		= vec_cross(forward, right);

	t_mat4	result = m4_init();

// t_vec3f forwardf = (vec_sub(to, from));

	// result.value[0][0] = right.x;
    // result.value[0][1] = right.y;
    // result.value[0][2] = right.z;

    // result.value[1][0] = up.x;
    // result.value[1][1] = up.y;
    // result.value[1][2] = up.z;

    // result.value[2][0] = forward.x;
    // result.value[2][1] = forward.y;
    // result.value[2][2] = forward.z;

    // result.value[3][0] = from.x;
    // result.value[3][1] = from.y;
    // result.value[3][2] = from.z;

	result.value[0][0] =  right.x;
	result.value[0][1] =  up.x;
	result.value[0][2] = -forward.x;

	result.value[1][0] =  right.y;
	result.value[1][1] =  up.y;
	result.value[1][2] = -forward.y;

	result.value[2][0] =  right.z;
	result.value[2][1] =  up.z;
	result.value[2][2] = -forward.z;

	// result.value[3][0] =  0.f;
	// result.value[3][1] =  0.f;
	// result.value[3][2] =  0.f;

	return (m4_mult(m4_translate(-from.x, -from.y, -from.z), result));
}


// #define A	m[0]=s.x;m[4]=s.y;m[8]=s.z;m[1]=u.x;m[5]=u.y;m[9]=u.z;m[2]=-f.x
// #define B	m[6]=-f.y;m[10]=-f.z;m[12]=-vec3_dot(&s, eye)
// #define C	m[13]=-vec3_dot(&u, eye);m[14]=vec3_dot(&f, eye)
