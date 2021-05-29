/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/05/29 14:51:36 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

#include "Prototypes.Parsing.h"

#include <stdlib.h>

/*
** Do not touch thoses variables
*/
#define REALLOC_SIZE 500

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

typedef struct s_parser
{
	size_t		v_count;
	size_t		vn_count;
	size_t		vt_count;
	t_vec3f		*v;
	t_vec3f		*vn;
	t_vec2f		*vt;
}				t_parser;

typedef struct s_scop
{
	
}				t_scop;

#endif