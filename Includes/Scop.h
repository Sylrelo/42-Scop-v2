/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/05/29 12:15:03 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

#include "Prototypes.Parsing.h"

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
	t_vec3f		v;
	t_vec3f		n;
	t_vec2f		vt;
}				t_parser;

typedef struct s_scop
{
	
}				t_scop;

#endif