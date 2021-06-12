/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIbmatvec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:15:25 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 01:15:26 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LIBMATVEC_H
# define LIBMATVEC_H

typedef struct 	s_mat4
{
	float		value[4][4];
}				t_mat4;

typedef struct s_vec3i
{
	int		x;
	int		y;
	int		z;
}				t_vec3i;

typedef struct s_vec3f
{
	float	x;
	float	y;
	float	z;
}				t_vec3f;

typedef struct s_vec2f
{
	float	x;
	float	y;
}				t_vec2f;


// Vectors Utils

t_vec3f		vec_add(t_vec3f u, t_vec3f v);
t_vec3f		vec_sub(t_vec3f u, t_vec3f v);
t_vec3f		vec_cross(t_vec3f u, t_vec3f v);
t_vec3f		vec_multf(t_vec3f u, float f);
t_vec3f		vec_normalize(t_vec3f u);
float		vec_length(t_vec3f u);

// Matrices Utils

// matrice.c
t_mat4		m4_init(void);
t_mat4 	    m4_mult(t_mat4 a, t_mat4 b);
t_mat4		m4_mult3(t_mat4 rotation, t_mat4 scale, t_mat4 translate);

// mat_projection.c
t_mat4		m4_perspective(float fov, float aspect, float near, float far);
t_mat4		m4_look_at(t_vec3f from, t_vec3f to);

// mat_transform.c
t_mat4		m4_rotation(float x, float y, float z);
t_mat4		m4_rotation_around_center(t_vec3f center, float x, float y, float z);
t_mat4		m4_scale(float x, float y, float z);
t_mat4		m4_translate(float x, float y, float z);

// mat_vec4f.c
t_vec3f	m4_mult_vec3f(t_mat4 mat, t_vec3f vec);

// mat_supl.c
t_mat4		m4_viewmat(float x, float y, float z, t_mat4 translate);
void		m4_print(t_mat4 mat);

#endif